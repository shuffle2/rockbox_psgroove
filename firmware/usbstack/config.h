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

/* Define this macro if want to use the jig exploit method */
#define USE_JIG

/* Will only do jig authentication, not usb exploit */
//#define JIG_AUTH
//#define HAVE_MASTER_KEY

/* The firmware version supported gets defined in the Makefile */
//#define FIRMWARE_2_70
//#define FIRMWARE_2_76
//#define FIRMWARE_3_01
//#define FIRMWARE_3_10
//#define FIRMWARE_3_15
//#define FIRMWARE_3_21
//#define FIRMWARE_3_40
#define FIRMWARE_3_41
//#define FIRMWARE_3_41_KIOSK

/* Which pl3 payload to statically compile in */
#define Payload_Regular
//#define Payload_NUS
//#define Payload_Dev
//#define Payload_Dump_ELFS
//#define Payload_Trace_ALL_SC
//#define Payload_Trace_HVC
//#define Payload_Trace_SC
//#define Payload_Trace_VUART

#endif /* PL3_CONFIG_H */
