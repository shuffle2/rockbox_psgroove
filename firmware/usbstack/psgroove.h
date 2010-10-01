#ifndef _PSGROOVE_H
#define _PSGROOVE_H

#include "usb_ch9.h"
#include "usb_core.h"

void psgroove_proc_init(void);

void psgroove_usb_set_address(uint8_t Address);

// Replaces Rockbox usb core function
void psgroove_request_handler_device_get_descriptor(struct usb_ctrlrequest* req);

// Rockbox interface
int psgroove_request_endpoints(struct usb_class_driver *drv);
int psgroove_set_first_interface(int interface);
int psgroove_get_config_descriptor(unsigned char *dest, int max_packet_size);
void psgroove_init_connection(void);
void psgroove_init(void);
void psgroove_disconnect(void);
void psgroove_transfer_complete(int ep, int dir, int status, int length);
bool psgroove_control_request(struct usb_ctrlrequest* req, unsigned char* dest);

#endif /* _PSGROOVE_H */
