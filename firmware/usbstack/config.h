/*
 * config.h -- PS3 Jailbreak payload : configuration file
 *
 * Copyright (C) Youness Alaoui (KaKaRoTo)
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 3, as published by the Free Software Foundation.
 *
 */

#ifndef PL3_CONFIG_H
#define PL3_CONFIG_H


/* Define this macro if want to use the jig method */
#define USE_JIG

/* 1. Define your PS3 Firmware. -= ONLY USE ONE! =-*/
/* The firmware version supported gets defined in the Makefile */
//#define FIRMWARE_2_76
//#define FIRMWARE_3_01
//#define FIRMWARE_3_10
#define FIRMWARE_3_15
//#define FIRMWARE_3_21
//#define FIRMWARE_3_40
//#define FIRMWARE_3_41

/* 2. Define your Target payload reg/NUS/Dev/etc -= ONLY USE ONE! =- */
//#define Payload_Regular //if you don't know what the others are-- this is what you want!
//#define Payload_NUS
#define Payload_Dev
//#define Payload_Dump_ELFS
//#define Payload_Trace_ALL_SC
//#define Payload_Trace_HVC
//#define Payload_Trace_SC
//#define Payload_Trace_VUART

#endif /* PL3_CONFIG_H */
