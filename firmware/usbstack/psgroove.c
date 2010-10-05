// IMPORTANT: Enabling DEBUGF logs will probably slow down transfers enough
// that the exploit will not work. If enabling DEBUGF, make sure to limit the
// amount of log spam.

#include <stdlib.h>
#include "file.h"
#include "string.h"
#include "system.h"
#include "timer.h"
#include "usb_core.h"
#include "usb_drv.h"
#include "usb_ch9.h"

#define LOGF_ENABLE
#include "logf.h"

#include "psgroove.h"
#include "psgroove_descriptors.h"

// Used for discarding JIG challenge
extern int usb_drv_recv_blocking(int endpoint, void* ptr, int length);

// PP502x-only regs...
#define REG_ENDPTCTRL(_x_)			(*(volatile unsigned int *)(USB_BASE+0x1c0+4*(_x_)))
#define EPCTRL_TX_DATA_TOGGLE_RST	(0x00400000) // in
#define EPCTRL_RX_DATA_TOGGLE_RST	(0x00000040) // out

// USB HUB spec
#define PORT_EMPTY		0x0100 /* powered only */
#define PORT_FULL		0x0103 /* connected, enabled, powered, full-speed */
#define C_PORT_CONN		0x0001 /* connection */
#define C_PORT_RESET	0x0010 /* reset */
#define C_PORT_NONE		0x0000 /* no change */
static uint16_t port_status[6] = { PORT_EMPTY, PORT_EMPTY, PORT_EMPTY, PORT_EMPTY, PORT_EMPTY, PORT_EMPTY };
static uint16_t port_change[6] = { C_PORT_NONE, C_PORT_NONE, C_PORT_NONE, C_PORT_NONE, C_PORT_NONE, C_PORT_NONE };

// psgroove state machine status
enum { 
	init,
	wait_hub_ready,
	hub_ready,
	p1_wait_reset,
	p1_wait_enumerate,
	p1_ready,
	p2_wait_reset,
	p2_wait_enumerate,
	p2_ready,
	p3_wait_reset,
	p3_wait_enumerate,
	p3_ready,
	p2_wait_disconnect,
	p4_wait_connect,
	p4_wait_reset,
	p4_wait_enumerate,
	p4_ready,
	p5_wait_reset,
	p5_wait_enumerate,
	p5_challenged,
	p5_responded,
	p3_wait_disconnect,
	p3_disconnected,
	p5_wait_disconnect,
	p5_disconnected,
	p4_wait_disconnect,
	p4_disconnected,
	p1_wait_disconnect,
	p1_disconnected,
	p6_wait_reset,
	p6_wait_enumerate,
	done,
} volatile state = init;

static const char* state_names[] =
{
	"init",
	"wait_hub_ready",
	"hub_ready",
	"p1_wait_reset",
	"p1_wait_enumerate",
	"p1_ready",
	"p2_wait_reset",
	"p2_wait_enumerate",
	"p2_ready",
	"p3_wait_reset",
	"p3_wait_enumerate",
	"p3_ready",
	"p2_wait_disconnect",
	"p4_wait_connect",
	"p4_wait_reset",
	"p4_wait_enumerate",
	"p4_ready",
	"p5_wait_reset",
	"p5_wait_enumerate",
	"p5_challenged",
	"p5_responded",
	"p3_wait_disconnect",
	"p3_disconnected",
	"p5_wait_disconnect",
	"p5_disconnected",
	"p4_wait_disconnect",
	"p4_disconnected",
	"p1_wait_disconnect",
	"p1_disconnected",
	"p6_wait_reset",
	"p6_wait_enumerate",
	"done"
};
#define state_name state_names[state]

volatile uint8_t hub_int_response = 0x00;
volatile uint8_t hub_int_force_data0 = 0;
volatile int last_port_conn_clear = 0;
volatile int last_port_reset_clear = 0;

volatile int8_t port_addr[7] = { -1, -1, -1, -1, -1, -1, -1 };
volatile int8_t port_cur = -1;

int stage2_file = 0, stage2_size = 0;

// TODO find a better way to size this
static unsigned char response_data[sizeof(port1_config_descriptor) + sizeof(default_payload)] USB_DEVBSS_ATTR;

volatile uint8_t expire = 0;

volatile int ep_in = 0, ep_out = 0;

static struct event_queue psgroove_queue;
static unsigned int psgroove_thread_entry = 0;
static long psgroove_stack[DEFAULT_STACK_SIZE/sizeof(long)];
static const char psgroove_thread_name[] = "psgroove";

enum
{
	PSGROOVE_TASK_HUB,
	PSGROOVE_TASK_JIG,
	PSGROOVE_TIMER_EXPIRED,
	PSGROOVE_CLR_FTR_CONN,
	PSGROOVE_CLR_FTR_RST,
	PSGROOVE_DONE,
};

#if 0
static const char* event_names[] = {
	"TASK_HUB",
	"TASK_JIG",
	"TIMER_EXPIRED",
	"CLR_FTR_CONN",
	"CLR_FTR_RST",
	"DONE",
};
#endif

static void timer_cb(void)
{
	if (expire)
	{
		if (--expire == 0)
			queue_post(&psgroove_queue, PSGROOVE_TIMER_EXPIRED, 0);
	}
}

// Should be in target-specific file?
static inline void Endpoint_ResetDataToggle(int ep)
{
	if (EP_DIR(ep) == DIR_TX)
		REG_ENDPTCTRL(ep) |= EPCTRL_TX_DATA_TOGGLE_RST;
	else if (EP_DIR(ep) == DIR_RX)
		REG_ENDPTCTRL(ep) |= EPCTRL_RX_DATA_TOGGLE_RST;
}

static inline void Endpoint_Write_Byte(int ep, const uint8_t Byte)
{
	memcpy(&response_data[0], &Byte, sizeof(Byte));
	usb_drv_send(ep, response_data, sizeof(Byte));
}

static inline void send_hub_info(const uint16_t wHubStatus, const uint16_t wHubChange)
{
	uint32_t data = (wHubChange << 16) | wHubStatus;
	
	memcpy(&response_data[0], &data, sizeof(data));

	DEBUGF("send_hub_info %02x%02x%02x%02x",
		*(uint8_t*)&response_data[0], *(uint8_t*)&response_data[1],
		*(uint8_t*)&response_data[2], *(uint8_t*)&response_data[3]);

	usb_drv_recv(EP_CONTROL, NULL, 0);
	usb_drv_send(EP_CONTROL, response_data, sizeof(data));
}

static inline void Endpoint_Discard_Stream(int ep, uint16_t Length)
{
	usb_drv_recv_blocking(ep, response_data, Length);

	DEBUGF("%02x%02x%02x%02x%02x%02x%02x%02x",
		*(uint8_t*)&response_data[0],
		*(uint8_t*)&response_data[1],
		*(uint8_t*)&response_data[2],
		*(uint8_t*)&response_data[3],
		*(uint8_t*)&response_data[4],
		*(uint8_t*)&response_data[5],
		*(uint8_t*)&response_data[6],
		*(uint8_t*)&response_data[7]);
}

static inline void Endpoint_Write_PStream_LE(int ep, void* Buffer, uint16_t Length)
{
	memcpy(&response_data[0], Buffer, Length);

	DEBUGF("%02x%02x%02x%02x",
		*(uint8_t*)&response_data[0], *(uint8_t*)&response_data[1],
		*(uint8_t*)&response_data[2], *(uint8_t*)&response_data[3]);

	usb_drv_send(ep, response_data, Length);
}

void psgroove_usb_set_address(uint8_t Address)
{
	uint8_t addr = Address & 0x7f;
	port_addr[port_cur] = addr;
	usb_drv_set_address(addr);
}

static void switch_port(int8_t port)
{
	if (port_cur == port) return;
	port_cur = port;
	if (port_addr[port] < 0)
		port_addr[port] = 0;
	usb_drv_set_address(port_addr[port] & 0x7f);
}

static void connect_port(int port)
{
	last_port_reset_clear = 0;
	hub_int_response = (1 << port);
	port_status[port - 1] = PORT_FULL;
	port_change[port - 1] = C_PORT_CONN;
	queue_post(&psgroove_queue, PSGROOVE_TASK_HUB, 0);
}

static void disconnect_port(int port)
{
	last_port_conn_clear = 0;
	hub_int_response = (1 << port);
	port_status[port - 1] = PORT_EMPTY;
	port_change[port - 1] = C_PORT_CONN;
	queue_post(&psgroove_queue, PSGROOVE_TASK_HUB, 0);
}

static void HUB_Task(void)
{
	if (hub_int_response) {
		DEBUGF("hub_task %d %02x %d", port_cur, hub_int_response, hub_int_force_data0);
		if (hub_int_force_data0) {
			Endpoint_ResetDataToggle(ep_in);
			hub_int_force_data0 = 0;
		}
		Endpoint_Write_Byte(ep_in, hub_int_response);
		hub_int_response = 0x00;
	}
}

static void JIG_Task(void)
{
	unsigned int bytes_out = 0, bytes_in = 0;

	for (;;)
	{
		if (state < p5_challenged)
		{
			Endpoint_Discard_Stream(ep_out, 8);
			bytes_out += 8;
			if (bytes_out >= sizeof(jig_response)) {
				state = p5_challenged;
				expire = 50; // was 90
			}
		}
		else if (state != p5_responded)
		{
			if (expire == 0)
			{
				Endpoint_Write_PStream_LE(ep_in, (void*)&jig_response[bytes_in], 8);
				bytes_in += 8;
				if (bytes_in >= sizeof(jig_response)) {
					state = p5_responded;
					expire = 15;
					return;
				}
			}
		}

		sleep(10);
	}
}

static void psgroove_reset(void)
{
	unsigned int i;
	for (i = 0; i < sizeof(port_status)/sizeof(port_status[0]); i++)
		port_status[i] = PORT_EMPTY;
	for (i = 0; i < sizeof(port_change)/sizeof(port_change[0]); i++)
		port_change[i] = C_PORT_NONE;

	hub_int_response = 0x00;
	hub_int_force_data0 = 0;
	last_port_conn_clear = 0;
	last_port_reset_clear = 0;
	memset((void*)&port_addr[0], -1, sizeof(port_addr));
	port_cur = -1;
	expire = 0;
	ep_in = 0, ep_out = 0;

	queue_clear(&psgroove_queue);
	psgroove_thread_entry = 0;
}

static void psgroove_thread(void)
{
	(void)state_names; // hey compiler - shutup.
	struct queue_event ev;

	state = init;
	switch_port(0);

	long int ticks = current_tick;
	int secs = ticks / HZ;
	int ms = ticks - secs * HZ;
	logf("psgroove %s %d.%d", state_name, secs, ms);

	for (;;)
	{
		queue_wait(&psgroove_queue, &ev);

#if 0 // informative, but sloooww
		ticks = current_tick;
		secs = ticks / HZ;
		ms = ticks - secs * HZ;
		logf("%s %s %d.%d",
			((unsigned)ev.id <= PSGROOVE_DONE) ? event_names[ev.id] : "DISCONNECT",
			state_name, secs, ms);
#endif

		switch (ev.id)
		{
		case PSGROOVE_TASK_HUB:
			HUB_Task();
			break;

		case PSGROOVE_TASK_JIG:
			JIG_Task();
			break;

		case PSGROOVE_TIMER_EXPIRED:
			switch (state)
			{
			case hub_ready:
				connect_port(1);
				state = p1_wait_reset;
				break;
			case p1_ready:
				switch_port(0);
				connect_port(2);
				state = p2_wait_reset;
				break;
			case p2_ready:
				switch_port(0);
				connect_port(3);
				state = p3_wait_reset;
				break;
			case p3_ready:
				switch_port(0);
				disconnect_port(2);
				state = p2_wait_disconnect;
				break;
			case p4_wait_connect:
				connect_port(4);
				state = p4_wait_reset;
				break;
			case p4_ready:
				switch_port(0);
				/* When first connecting port 5, we need to
					have the wrong data toggle for the PS3 to
					respond */
				hub_int_force_data0 = 1;
				connect_port(5);
				state = p5_wait_reset;
				break;
			case p5_responded:
				switch_port(0);
				/* Need wrong data toggle again */
				hub_int_force_data0 = 1;
				disconnect_port(3);
				state = p3_wait_disconnect;
				break;
			case p3_disconnected:
				/* If not using JIG mode, then no need to unplug the JIG, since we'll
					need to keep it in memory so we can find its address from an lv2 dump
				*/
				#ifdef USE_JIG
				switch_port(0);
				disconnect_port(5);
				#endif
				state = p5_wait_disconnect;
				break;
			case p5_disconnected:
				switch_port(0);
				disconnect_port(4);
				state = p4_wait_disconnect;
				break;
			case p4_disconnected:
				switch_port(0);
				disconnect_port(1);
				state = p1_wait_disconnect;
				break;
			case p1_disconnected:
				switch_port(0);
				connect_port(6);
				state = p6_wait_reset;
				break;
			default:
				DEBUGF("TIMER_EXPIRED %s", state_name);
				break;
			}
			break;

		case PSGROOVE_CLR_FTR_CONN:
			if (state == p2_wait_disconnect && last_port_conn_clear == 2)
			{
				state = p4_wait_connect;
				expire = 15;
			}
			else if (state == p3_wait_disconnect && last_port_conn_clear == 3)
			{
				state = p3_disconnected;
				expire = 45;
			}
			else if (state == p5_wait_disconnect && last_port_conn_clear == 5)
			{
				state = p5_disconnected;
				expire = 20;
			}
			else if (state == p4_wait_disconnect && last_port_conn_clear == 4)
			{
				state = p4_disconnected;
				expire = 20;
			}
			else if (state == p1_wait_disconnect && last_port_conn_clear == 1)
			{
				state = p1_disconnected;
				expire = 20;
			}
			else
				DEBUGF("CLR_FTR_CONN %s %d", state_name, last_port_conn_clear);
			break;

		case PSGROOVE_CLR_FTR_RST:
			#define HANDLE_CLR_FTR_RST(x)									\
			if (state == p##x##_wait_reset && last_port_reset_clear == x)	\
			{																\
				switch_port(x);												\
				state = p##x##_wait_enumerate;								\
			}																\
			else
			HANDLE_CLR_FTR_RST(1)
			HANDLE_CLR_FTR_RST(2)
			HANDLE_CLR_FTR_RST(3)
			HANDLE_CLR_FTR_RST(4)
			HANDLE_CLR_FTR_RST(5)
			HANDLE_CLR_FTR_RST(6)
				DEBUGF("CLR_FTR_RST %s %d", state_name, last_port_reset_clear);
			break;

		case PSGROOVE_DONE:			
			ticks = current_tick;
			secs = ticks / HZ;
			ms = ticks - secs * HZ;
			logf("psgroove %s %d.%d", state_name, secs, ms);

			cpu_boost(0);
			timer_unregister();
			psgroove_reset();
			return;
			break;

		case SYS_USB_DISCONNECTED:
			cpu_boost(0);
			timer_unregister();
			// User has disconnected usb cable while psgroove was running.
			// No way to recover without hard-rebooting ps3 (and rockbox device)
			return;
			break;
		}
	}
}

void psgroove_proc_init(void)
{
	if (psgroove_thread_entry == 0)
	{
		cpu_boost(1);

		queue_init(&psgroove_queue, true);
		
		#ifdef MARCAN_STYLE
		// open stage2
		stage2_file = open("/.rockbox/stage2.bin", O_RDONLY);
		if (stage2_file < 0) {
			logf("unable to load stage2 - dying");
			return;
		} else {
			stage2_size = filesize(stage2_file);
			logf("stage2 size is %dB", stage2_size);
		}
		#endif

		// 10 millisec timer
		timer_register(1, NULL, TIMER_FREQ * .010, timer_cb IF_COP(, CPU));

		psgroove_thread_entry = create_thread(psgroove_thread, psgroove_stack,
			sizeof(psgroove_stack), 0, psgroove_thread_name
			IF_PRIO(, PRIORITY_SYSTEM) IF_COP(, CPU));
	}
}

//////////////////////////////////////////////////////////////////////////
// Rockbox usb driver interface
//////////////////////////////////////////////////////////////////////////
static int usb_interface;

// 0: control
// 1: int in
// 2: int out
int psgroove_request_endpoints(struct usb_class_driver *drv)
{
	if ((ep_in = usb_core_request_endpoint(USB_ENDPOINT_XFER_INT, USB_DIR_IN, drv)) < 0)
		return -1;
	if ((ep_out = usb_core_request_endpoint(USB_ENDPOINT_XFER_INT, USB_DIR_OUT, drv)) < 0)
		return -1;
	
	DEBUGF("endpoints in:%d out:%d", EP_NUM(ep_in), EP_NUM(ep_out));

	return 0;
}

int psgroove_set_first_interface(int interface)
{
	usb_interface = interface;

	return interface + 1;
}

int psgroove_get_config_descriptor(unsigned char *dest, int max_packet_size)
{
	(void)dest, (void)max_packet_size;
	return 0;
}

// replaces core's void request_handler_device_get_descriptor(struct usb_ctrlrequest* req)
void psgroove_request_handler_device_get_descriptor(struct usb_ctrlrequest* req)
{
	const uint8_t  DescriptorType   = (req->wValue >> 8);
	const uint8_t  DescriptorNumber = (req->wValue & 0xFF);
	const uint16_t  wLength = req->wLength;

	void*          Address = NULL;
	uint16_t       Size    = 0;

	switch (DescriptorType)
	{
	case USB_DT_DEVICE:
		switch (port_cur) {
		case 0:
			Address = (void*)&HUB_Device_Descriptor;
			Size    = sizeof(HUB_Device_Descriptor);
			break;
		case 1:
			Address = (void*)&port1_device_descriptor;
			Size    = sizeof(port1_device_descriptor);
			break;
		case 2:
			Address = (void*)&port2_device_descriptor;
			Size    = sizeof(port2_device_descriptor);
			break;
		case 3:
			Address = (void*)&port3_device_descriptor;
			Size    = sizeof(port3_device_descriptor);
			break;
		case 4:
			Address = (void*)&port4_device_descriptor;
			Size    = sizeof(port4_device_descriptor);
			break;
		case 5:
			Address = (void*)&port5_device_descriptor;
			Size    = sizeof(port5_device_descriptor);
			break;
		case 6:
			Address = (void*)&final_device_descriptor;
			Size    = sizeof(final_device_descriptor);
			break;
		}
		break;
	case USB_DT_CONFIG:
		switch (port_cur) {
		case 0:
			HUB_Config_Descriptor.endpoint.bEndpointAddress = ep_in;
			Address = (void*)&HUB_Config_Descriptor;
			Size    = sizeof(HUB_Config_Descriptor);
			break;
		case 1:
			if (DescriptorNumber < PORT1_NUM_CONFIGS) {
				if (wLength > USB_DT_CONFIG_SIZE) {
					port1_config_descriptor.config.wTotalLength = LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE);
				} else {
					port1_config_descriptor.config.wTotalLength = LE16(sizeof(port1_config_descriptor) + sizeof(default_payload));
				}

				if (DescriptorNumber == (PORT1_NUM_CONFIGS-1) && wLength > USB_DT_CONFIG_SIZE) {
						state  = p1_ready;
						expire = 10;
				}
				
				// tack on payload and send wLength amount, instead of MIN()
				memcpy(&response_data[0], (void*)&port1_config_descriptor, sizeof(port1_config_descriptor));
				memcpy(&response_data[0] + sizeof(port1_config_descriptor), default_payload, sizeof(default_payload));

				usb_drv_recv(EP_CONTROL, NULL, 0);
				usb_drv_send(EP_CONTROL, response_data, wLength);
				return;
			}
			break;
		case 2:
			// only 1 config
			Address = (void*)&port2_config_descriptor;
			Size    = sizeof(port2_config_descriptor);
			state   = p2_ready;
			expire  = 15;
			break;
		case 3:
			if (DescriptorNumber == 1 && wLength > USB_DT_CONFIG_SIZE) {
				state  = p3_ready;
				expire = 10;
			}
			
			memcpy(&response_data[0], (void*)&port3_config_descriptor, sizeof(port3_config_descriptor));

			int i = sizeof(port3_config_descriptor);
			while (i < wLength) { // TODO perhaps generate this in psgroove_init instead
				memcpy(&response_data[i], (void*)&port3_padding, sizeof(port3_padding));
				i += sizeof(port3_padding);
			}
			
			usb_drv_recv(EP_CONTROL, NULL, 0);
			usb_drv_send(EP_CONTROL, response_data, wLength);
			return;
			break;
		case 4:
			// 3 configurations
			if (DescriptorNumber == 0) {
				Address = (void*)&port4_config_descriptor_1;
				Size    = sizeof(port4_config_descriptor_1);
			} else if (DescriptorNumber == 1) {
				if (wLength > USB_DT_CONFIG_SIZE) {
					port4_config_descriptor_2.config.wTotalLength = LE16(0);
				} else {
					port4_config_descriptor_2.config.wTotalLength = LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE);
				}
				
				memcpy(&response_data[0], (void*)&port4_config_descriptor_2, sizeof(port4_config_descriptor_2));
				
				usb_drv_recv(EP_CONTROL, NULL, 0);
				usb_drv_send(EP_CONTROL, response_data, wLength);
				return;
			} else if (DescriptorNumber == 2) {
				Address = (void*)&port4_config_descriptor_3;
				Size    = sizeof(port4_config_descriptor_3);
				if (wLength > USB_DT_CONFIG_SIZE) {
					state = p4_ready;
					expire = 20;  // longer seems to help this one?
				}
			}
			break;
		case 5:
			// 1 config
			port5_config_descriptor.endpoint_out.bEndpointAddress = ep_out;
			port5_config_descriptor.endpoint_in.bEndpointAddress = ep_in;
			Address = (void*)&port5_config_descriptor;
			Size    = sizeof(port5_config_descriptor);
			break;
		case 6:
			// 1 config
			Address = (void*)&final_config_descriptor;
			Size    = sizeof(final_config_descriptor);
			#ifndef MARCAN_STYLE
			if (wLength > USB_DT_CONFIG_SIZE) {
				// pl3 does not send 0xaa control request, so this is the last communication we do
				// marcan's stage1 needs to load stage2 yet
				state = done;
				queue_post(&psgroove_queue, PSGROOVE_DONE, 0);
			}
			#endif
			break;
		}
		break;
	case 0x29: // HUB descriptor (always to port 0 we'll assume)
		switch (port_cur) {
		case 0:
			Address = (void*)HUB_Hub_Descriptor;
			Size    = sizeof(HUB_Hub_Descriptor);
			break;
		}
		break;
	}

	Size = MIN(wLength, Size);
	memcpy(&response_data[0], Address, Size);

	usb_drv_recv(EP_CONTROL, NULL, 0);
	usb_drv_send(EP_CONTROL, response_data, Size);
}

void psgroove_init_connection(void) {}
void psgroove_init(void) {}
void psgroove_disconnect(void) {}
void psgroove_transfer_complete(int ep, int dir, int status, int length)
{
	(void)ep, (void)dir, (void)status, (void)length;
}

bool psgroove_control_request(struct usb_ctrlrequest* req, unsigned char* dest)
{
	(void)dest;
	
	DEBUGF("%d %s %02x %02x %04x %04x", port_cur, state_name, req->bRequest, req->bRequestType, req->wValue, req->wIndex);
	
	if (port_cur == 6 && req->bRequest == 0xAA) {
		// pl3 does not send this...
		usb_drv_recv(EP_CONTROL, NULL, 0);
		usb_drv_send(EP_CONTROL, NULL, 0);
		state = done;
		queue_post(&psgroove_queue, PSGROOVE_DONE, 0);
		return true;
	}

	if (port_cur == 5 && req->bRequest == USB_REQ_SET_INTERFACE)
	{
		// Just ack the actual event...
		usb_drv_send(EP_CONTROL, NULL, 0);

		// But now we kickoff the JIG :)
		queue_post(&psgroove_queue, PSGROOVE_TASK_JIG, 0);
		return true;
	}

	if (port_cur == 0 &&
		req->bRequestType == 0xA0 &&
		req->bRequest == 0x00 &&  // GET HUB STATUS
		req->wValue == 0x00 &&
		req->wIndex == 0x00 &&
		req->wLength == 0x04) {
			send_hub_info(0, 0);
			return true;
	}

	if (port_cur == 0 &&
		req->bRequestType == 0xA3 &&  
		req->bRequest == 0x00 &&   //  GET PORT STATUS
		req->wValue == 0x00 &&
		req->wLength == 0x04) {
			uint8_t p = req->wIndex;
			if (p < 1 || p > 6) return false;

			send_hub_info(port_status[p - 1], port_change[p - 1]);
			return true;
	}

	if (port_cur == 0 &&
		req->bRequestType == 0x23 &&
		req->bRequest == 0x03 && // SET_FEATURE
		req->wLength == 0x00) {
			uint8_t p = req->wIndex;
			if (p < 1 || p > 6) return false;

			usb_drv_recv(EP_CONTROL, NULL, 0);
			usb_drv_send(EP_CONTROL, NULL, 0);

			switch(req->wValue) {
			case 0x0008: // PORT_POWER
				if (p == 6 && state == init) {
					/* after the 6th port is powered, wait a bit and continue */
					state = hub_ready;
					expire = 15;
				}
				break;
			case 0x0004: // PORT_RESET
				hub_int_response = (1 << p);
				port_change[p - 1] |= C_PORT_RESET;
				queue_post(&psgroove_queue, PSGROOVE_TASK_HUB, 0);
				break;
			}
			return true;
	}

	if (port_cur == 0 &&
		req->bRequestType == 0x23 &&
		req->bRequest == 0x01 && // CLEAR_FEATURE
		req->wLength == 0x00) {
			uint8_t p = req->wIndex;
			if (p < 1 || p > 6) return false;

			usb_drv_recv(EP_CONTROL, NULL, 0);
			usb_drv_send(EP_CONTROL, NULL, 0);

			switch(req->wValue) {
			case 0x0010: // C_PORT_CONNECTION
				port_change[p - 1] &= ~C_PORT_CONN;
				last_port_conn_clear = p;
				queue_post(&psgroove_queue, PSGROOVE_CLR_FTR_CONN, 0);
				break;
			case 0x0014: // C_PORT_RESET
				port_change[p - 1] &= ~C_PORT_RESET;
				last_port_reset_clear = p;
				queue_post(&psgroove_queue, PSGROOVE_CLR_FTR_RST, 0);
				break;
			}
			return true;
	}
	
	#ifdef MARCAN_STYLE
	enum {
		REQ_PRINT = 1,
		REQ_GET_STAGE2_SIZE,
		REQ_READ_STAGE2_BLOCK,
		TYPE_HOST2DEV = 0x43,
		TYPE_DEV2HOST = 0xc3
	};
	
	if (req->bRequestType == TYPE_HOST2DEV && req->bRequest == REQ_PRINT) {
		if (req->wLength > 0x400) {
			logf("lv2 print too large!");
			return false;
		}
		// with marcan's loader, we don't know if we'll keep getting prints or not
		// so it's hard to tell if we can kill the psgroove_thread :/
		usb_drv_recv(EP_CONTROL, response_data, req->wLength);
		usb_drv_send(EP_CONTROL, NULL, 0);
		response_data[req->wLength] = '\0';
		logf(response_data);
		return true;
	}
	
	if (req->bRequestType == TYPE_DEV2HOST && req->bRequest == REQ_GET_STAGE2_SIZE) {
		uint32_t stage2_size_ = htobe32(stage2_size);
		memcpy(response_data, &stage2_size_, sizeof(stage2_size_));
		usb_drv_recv(EP_CONTROL, NULL, 0);
		usb_drv_send(EP_CONTROL, response_data, sizeof(stage2_size_));
		return true;
	}
	
	if (req->bRequestType == TYPE_DEV2HOST && req->bRequest == REQ_READ_STAGE2_BLOCK) {
		int offset = req->wIndex << 12;
		int available = stage2_size - offset;
		int length = req->wLength;
		logf("read_stage2_block(offset=0x%x,len=0x%x)", offset, length);
		if (available < 0)
			available = 0;
		if (length > available) {
			logf("warning: length exceeded, want 0x%x, avail 0x%x", length, available);
			length = available;
		}
		lseek(stage2_file, offset, SEEK_SET);
		read(stage2_file, response_data, length);
		usb_drv_recv(EP_CONTROL, NULL, 0);
		usb_drv_send(EP_CONTROL, response_data, length);
		
		if (available - length == 0)
			close(stage2_file);
		return true;
	}
	#endif

	return false;
}
