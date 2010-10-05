#ifndef _PSGROOVE_DESC_
#define _PSGROOVE_DESC_

#include "cpu.h"
#include "usb_ch9.h"

#include "pl3/config.h"
#include "pl3/shellcode_egghunt.h"
#include "pl3/default_payload_3_41.h"
#include "pl3/default_payload_3_01.h"
#include "pl3/default_payload_3_10.h"
#include "pl3/default_payload_3_15.h"
#include "pl3/dump_lv2.h"

#ifdef ROCKBOX_LITTLE_ENDIAN
#define LE16(x) (x)
#else
#define LE16(x) ((( (x) & 0xFF) << 8) | (( (x) & 0xFF00) >> 8))
#endif

#define MAGIC_NUMBER		'P', 'S', 'F', 'r', 'e', 'e', 'd', 'm'

#if defined (FIRMWARE_3_41)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xe7, 0x20
#elif defined (FIRMWARE_3_15) || defined (FIRMWARE_3_10)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x10
#elif defined (FIRMWARE_3_01)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x32, 0x06, 0x40
#else
#error You must specify the target firmware. \
   define a supported FIRMWARE_X_YZ in config.h and recompile.
#endif /* FIRMWARE_X_YZ */

#ifdef USE_JIG
#define default_shellcode shellcode_egghunt
#define default_shellcode_macro shellcode_egghunt_macro

#if defined (FIRMWARE_3_41)
#define default_payload default_payload_3_41
#define default_payload_macro default_payload_3_41_macro
#define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#elif defined (FIRMWARE_3_15)
#define default_payload default_payload_3_15
#define default_payload_macro default_payload_3_15_macro
#define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#elif defined (FIRMWARE_3_10)
#define default_payload default_payload_3_10
#define default_payload_macro default_payload_3_10_macro
#define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#elif defined (FIRMWARE_3_01)
#define default_payload default_payload_3_01
#define default_payload_macro default_payload_3_01_macro
#define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xFB, 0xC8
#endif /* FIRMWARE_X_YZ */

#define SHELLCODE_PAGE		0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
#define SHELLCODE_DESTINATION	SHELLCODE_ADDR_BASE
#define SHELLCODE_PTR 		SHELLCODE_ADDR_BASE + 0x08
#define SHELLCODE_ADDRESS	SHELLCODE_ADDR_BASE + 0x18

#define PORT1_NUM_CONFIGS	4

#else /* USE_JIG */

#define default_shellcode shellcode_egghunt
#define default_shellcode_macro shellcode_egghunt_macro
#define default_payload dump_lv2
#define default_payload_macro dump_lv2_macro

#define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x00

#define SHELLCODE_PAGE		SHELLCODE_ADDR_BASE
#define SHELLCODE_DESTINATION	SHELLCODE_ADDR_BASE + 0x20
#define SHELLCODE_PTR 		SHELLCODE_ADDR_BASE + 0x28
#define SHELLCODE_ADDRESS	SHELLCODE_ADDR_BASE + 0x38

#define PORT1_NUM_CONFIGS	100

#endif /* USE_JIG */


const struct usb_device_descriptor HUB_Device_Descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= USB_CLASS_HUB,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x01,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0xCCCC),
	.bcdDevice			= LE16(0x0100),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x01
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct usb_endpoint_descriptor	endpoint;
} __attribute__ ((packed))
HUB_Config_Descriptor = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE + USB_DT_ENDPOINT_SIZE),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER | USB_CONFIG_ATT_WAKEUP,
		.bMaxPower			= 50
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 1,
		.bInterfaceClass	= USB_CLASS_HUB,
		.bInterfaceSubClass	= 0,
		.bInterfaceProtocol	= 0,
		.iInterface			= 0
	},
	{
		.bLength			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType	= USB_DT_ENDPOINT,
		.bEndpointAddress	= 0, // filled in at run-time
		.bmAttributes		= USB_ENDPOINT_XFER_INT,
		.wMaxPacketSize		= LE16(8),
		.bInterval			= 12
	}
};

const uint8_t HUB_Hub_Descriptor[] = {
	0x09,		// size
	0x29,		// type: hub
	0x06,		// num ports
	0xa9, 0x00,	// attribs: lies
	50,			// pwron to pwrgood
	100,		// max hub current (epic fail vs. above)
	0,			// none removable
	0xff		// pwrctrlmask
};

static const uint8_t jig_response[64] = {
	#ifdef USE_JIG
	SHELLCODE_PTR,
	SHELLCODE_ADDRESS,
	RTOC_TABLE,
	default_shellcode_macro
	#else
	'J', 'I', 'G', ' ', 'R', 'E', 'S', 'P',
	'O', 'N', 'S', 'E', ' ', 'B', 'U', 'F',
	'F', 'E', 'R', ' ', ' ', ' ', ' ', ' '
	#endif
};

const struct usb_device_descriptor port1_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0x5555),
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= PORT1_NUM_CONFIGS
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct {
		uint8_t padding[6];
		#ifdef USE_JIG
		uint8_t data[1][8];
		#else
		uint8_t data[4][8];
		#endif
	} __attribute__ ((packed)) extra;
} __attribute__ ((packed))
port1_config_descriptor = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(0xf00), // filled in at run-time
		.bNumInterfaces		= 1,
		.bConfigurationValue= 0,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 250
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	},
	{
		.padding			= { 0, 0, 0, 0, 0, 0 },
		.data[0]			= { MAGIC_NUMBER },
		#ifndef USE_JIG
		.data[1]			= { SHELLCODE_PTR },
		.data[2]			= { SHELLCODE_ADDRESS },
		.data[3]			= { RTOC_TABLE }
		#endif
	}
};

const struct usb_device_descriptor port2_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0xBBBB),
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x01
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct {
		uint8_t bLength;
        uint8_t bDescriptorType;
		uint8_t unkData1;
		uint8_t unkData2;
	} __attribute__ ((packed)) extra;
} __attribute__ ((packed))
const port2_config_descriptor = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE + 4),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 1,
		.bInterfaceClass	= USB_CLASS_HUB,
		.bInterfaceSubClass	= 0,
		.bInterfaceProtocol	= 0,
		.iInterface			= 0
	},
	{	// ???
		.bLength			= 4,
		.bDescriptorType	= 0x21,
		.unkData1			= 0xb4,
		.unkData2			= 0x2f
	}
};

const struct usb_device_descriptor port3_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0x5555),
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x02
};

const struct usb_config_descriptor port3_config_descriptor = {
	.bLength			= USB_DT_CONFIG_SIZE,
	.bDescriptorType	= USB_DT_CONFIG,
	.wTotalLength		= LE16(0xa4d),
	.bNumInterfaces		= 1,
	.bConfigurationValue= 1,
	.iConfiguration		= 0,
	.bmAttributes		= USB_CONFIG_ATT_ONE,
	.bMaxPower			= 1
};

const struct usb_interface_descriptor port3_padding = {
	.bLength			= USB_DT_INTERFACE_SIZE,
	.bDescriptorType	= USB_DT_INTERFACE,
	.bInterfaceNumber	= 0,
	.bAlternateSetting	= 0,
	.bNumEndpoints		= 0,
	.bInterfaceClass	= USB_CLASS_APP_SPEC,
	.bInterfaceSubClass	= 1,
	.bInterfaceProtocol	= 2,
	.iInterface			= 0
};

const struct usb_device_descriptor port4_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0x5555),
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x03
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
} __attribute__ ((packed))
const port4_config_descriptor_1 = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	}
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
} __attribute__ ((packed))
port4_config_descriptor_2 = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE), // filled in at run-time
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	}
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint8_t padding[4];
		uint8_t data[3][8];
	} __attribute__ ((packed)) extra;
} __attribute__ ((packed))
const port4_config_descriptor_3 = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(0x30),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	},
	{
		.bLength			= 0x3e,
		.bDescriptorType	= 0x21,
		.padding			= { 0, 0, 0, 0 },
		.data[0]			= { MAGIC_NUMBER },
		.data[1]			= { SHELLCODE_PAGE },
		.data[2]			= { SHELLCODE_DESTINATION }
	}
};

const struct usb_device_descriptor port5_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0x054c),
	.idProduct			= LE16(0x02eb),
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x01
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct usb_endpoint_descriptor	endpoint_out;
	struct usb_endpoint_descriptor	endpoint_in;
} __attribute__ ((packed))
port5_config_descriptor = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE + USB_DT_ENDPOINT_SIZE*2),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 2,
		.bInterfaceClass	= USB_CLASS_VENDOR_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	},
	{
		.bLength			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType	= USB_DT_ENDPOINT,
		.bEndpointAddress	= 0, // filled in at run-time
		.bmAttributes		= USB_ENDPOINT_XFER_BULK,
		.wMaxPacketSize		= LE16(8),
		.bInterval			= 0
	},
	{
		.bLength			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType	= USB_DT_ENDPOINT,
		.bEndpointAddress	= 0, // filled in at run-time
		.bmAttributes		= USB_ENDPOINT_XFER_BULK,
		.wMaxPacketSize		= LE16(8),
		.bInterval			= 0
	}
};

const struct usb_device_descriptor final_device_descriptor = {
	.bLength			= USB_DT_DEVICE_SIZE,
	.bDescriptorType	= USB_DT_DEVICE,
	.bcdUSB				= LE16(0x0200),
	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0	= 0x08,
	.idVendor			= LE16(0xAAAA),
	.idProduct			= LE16(0x3713), // pl3 no longer cares about this value
	.bcdDevice			= LE16(0x0000),
	.iManufacturer		= 0x00,
	.iProduct			= 0x00,
	.iSerialNumber		= 0x00,
	.bNumConfigurations	= 0x01
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
} __attribute__ ((packed))
const final_config_descriptor = {
	{
		.bLength			= USB_DT_CONFIG_SIZE,
		.bDescriptorType	= USB_DT_CONFIG,
		.wTotalLength		= LE16(USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE),
		.bNumInterfaces		= 1,
		.bConfigurationValue= 1,
		.iConfiguration		= 0,
		.bmAttributes		= USB_CONFIG_ATT_ONE,
		.bMaxPower			= 1
	},
	{
		.bLength			= USB_DT_INTERFACE_SIZE,
		.bDescriptorType	= USB_DT_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	}
};

#endif
