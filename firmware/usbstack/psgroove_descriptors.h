#include "usb_ch9.h"

// ipod video is LE, yay
#define LE16(x) (x)
//#define LE16(in) ((( (in) & 0xFF) << 8) | (( (in) & 0xFF00) >> 8))

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
	.bNumConfigurations	= 0x04
};

struct {
	struct usb_config_descriptor	config;
	struct usb_interface_descriptor	interface;
	struct {
		uint8_t padding[6];
		//uint8_t data[2][8]; // not used with psgroove's payload
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
		//.data[0]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x20 },
		//.data[1]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x28 }
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
		uint8_t data[3][8]; // different values for marcan & psgroove
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
		.data[0]			= { 0xfa, 0xce, 0xb0, 0x03,	0xaa, 0xbb, 0xcc, 0xdd },
		.data[1]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x46, 0x50, 0x00 },
		.data[2]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70 }
		//.data[1]			= { 0, 0, 0, 0,	0, 0, 0, 0 },
		//.data[2]			= { 0x80, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x10, 0x18 }
	}
};

// not used in marcan's code
const uint8_t port5_device_descriptor[] = {
	0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08,
	0x4c, 0x05, 0xeb, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01,
};

uint8_t port5_config_descriptor[] = {
	// config
	0x09, 0x02, 0x20, 0x00, 0x01, 0x00, 0x00, 0x80,
	0x01,
	// interface
	0x09, 0x04, 0x00, 0x00, 0x02, 0xff, 0x00, 0x00,
	0x00,
	// endpoint
	0x07, 0x05, 0x02, 0x02, 0x08, 0x00, 0x00,
	// endpoint
	0x07, 0x05, 0x81, 0x02, 0x08, 0x00, 0x00,
};

// FINAL device
const uint8_t port6_device_descriptor[] = {
	0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08,
	0xaa, 0xaa, 0xc0, 0xde, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01,
};

const uint8_t port6_config_descriptor[] = {
	// config
	0x09, 0x02, 0x12, 0x00, 0x01, 0x01, 0x00, 0x80,
	0x01,
	// interface
	0x09, 0x04, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x02,
	0x00,
};
