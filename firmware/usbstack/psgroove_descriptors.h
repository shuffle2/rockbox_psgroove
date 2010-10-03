#ifndef _PSGROOVE_DESC_
#define _PSGROOVE_DESC_

#include "usb_ch9.h"

#define LE16(x) (x)

//#define MARCAN_STYLE

#include "PL3/config.h"
#define FIRMWARE_3_41

typedef uint8_t u8;
#include "PL3/shellcode_egghunt.h"
#include "PL3/default_payload_3_41.h"
#include "PL3/default_payload_3_01.h"
#include "PL3/default_payload_3_15.h"
#include "PL3/dump_lv2.h"

//#define MAGIC_NUMBER		'P', 'S', 'F', 'r', 'e', 'e', 'd', 'm'
#define MAGIC_NUMBER		0x50, 0x53, 0x46, 0x72, 0x65, 0x65, 0x64, 0x6d

#if defined (FIRMWARE_3_41)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xe7, 0x20
#elif defined (FIRMWARE_3_15)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x10
#elif defined (FIRMWARE_3_01)
#define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x32, 0x06, 0x40
#else
#error You must specify the target firmware. \
   define a supported FIRMWARE_X_YZ in config.h and recompile.
#endif /* FIRMWARE_X_YZ */


#ifdef USE_JIG
#define default_shellcode shellcode_egghunt

#if defined (FIRMWARE_3_41)
#define default_payload default_payload_3_41
#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee
#define SHELLCODE_ADDR_LOW	0x70
#elif defined (FIRMWARE_3_15)
#define default_payload default_payload_3_15
#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde
#define SHELLCODE_ADDR_LOW	0x30
#elif defined (FIRMWARE_3_01)
#define default_payload default_payload_3_01
#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xFB
#define SHELLCODE_ADDR_LOW	0xC8
#endif /* FIRMWARE_X_YZ */

#define SHELLCODE_PAGE		0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
#define SHELLCODE_DESTINATION	SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW
#define SHELLCODE_PTR 		SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW + 0x08
#define SHELLCODE_ADDRESS	SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW + 0x18

#define PORT1_NUM_CONFIGS	4

#else /* USE_JIG */

#define default_shellcode shellcode_egghunt
#define default_payload dump_lv2

#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x00
//#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x5B, 0x00
//#define SHELLCODE_ADDR_HIGH	0x80, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00
#define SHELLCODE_ADDR_LOW	0x00

#define SHELLCODE_PAGE		SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW
#define SHELLCODE_DESTINATION	SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW + 0x20
#define SHELLCODE_PTR 		SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW + 0x28
#define SHELLCODE_ADDRESS	SHELLCODE_ADDR_HIGH, SHELLCODE_ADDR_LOW + 0x38

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
		.wMaxPacketSize		= LE16(1),
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
	SHELLCODE_PTR,
	SHELLCODE_ADDRESS,
	RTOC_TABLE,
	// for now, static egghunt shellcode
	0xe8, 0x83, 0xff, 0xf0, 0xe8, 0x63, 0xff, 0xf8,
	0xe8, 0xa3, 0x00, 0x18, 0x38, 0x63, 0x10, 0x00,
	0x7c, 0x04, 0x28, 0x00, 0x40, 0x82, 0xff, 0xf4,
	0x38, 0x63, 0xf0, 0x00, 0x38, 0xc3, 0x00, 0x20,
	0x7c, 0xc9, 0x03, 0xa6, 0x4e, 0x80, 0x04, 0x20,
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
		#ifdef MARCAN_STYLE
		uint8_t data[2][8];
		#else
		#ifdef USE_JIG
		uint8_t data[1][8];
		#else
		uint8_t data[4][8];
		#endif
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
		#ifdef MARCAN_STYLE
		.data[0]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x20 },
		.data[1]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x28 }
		#else
		.data[0]			= { MAGIC_NUMBER },
		#ifndef USE_JIG
		.data[1]			= { SHELLCODE_PTR },
		.data[2]			= { SHELLCODE_ADDRESS },
		.data[3]			= { RTOC_TABLE }
		#endif
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
		#ifdef MARCAN_STYLE
		.data[0]			= { 0, 0, 0, 0, 0, 0, 0, 0 },
		.data[1]			= { 0, 0, 0, 0, 0, 0, 0, 0 },
		.data[2]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x18 }
		#else
		//.data[0]			= { 0xfa, 0xce, 0xb0, 0x03, 0xaa, 0xbb, 0xcc, 0xdd },
		//.data[1]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x46, 0x50, 0x00 },
		//.data[2]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70 }
		.data[0]			= { MAGIC_NUMBER },
		.data[1]			= { SHELLCODE_PAGE },
		.data[2]			= { SHELLCODE_DESTINATION }
		#endif
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
	#ifdef MARCAN_STYLE
	.idProduct			= LE16(0x3713), // marcan, lol...
	#else
	.idProduct			= LE16(0xdec0),
	#endif
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

#ifndef MARCAN_STYLE
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
		.bNumEndpoints		= 0,
		.bInterfaceClass	= USB_CLASS_APP_SPEC,
		.bInterfaceSubClass	= 1,
		.bInterfaceProtocol	= 2,
		.iInterface			= 0
	},
	{
		.bLength			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType	= USB_DT_ENDPOINT,
		.bEndpointAddress	= 0, // filled in at run-time
		.bmAttributes		= USB_ENDPOINT_XFER_INT,
		.wMaxPacketSize		= LE16(1),
		.bInterval			= 12
	},
	{
		.bLength			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType	= USB_DT_ENDPOINT,
		.bEndpointAddress	= 0, // filled in at run-time
		.bmAttributes		= USB_ENDPOINT_XFER_INT,
		.wMaxPacketSize		= LE16(1),
		.bInterval			= 12
	}
};
#endif

#endif
