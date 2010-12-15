/*
PSGroove header to configure payload selections
*/

#if defined (FIRMWARE_3_41) || defined (FIRMWARE_3_41_KIOSK) || defined (FIRMWARE_3_40)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xe7, 0x20
#elif defined (FIRMWARE_3_30)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xdb, 0xc0
#elif defined (FIRMWARE_3_15) || defined (FIRMWARE_3_10)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x10
#elif defined (FIRMWARE_3_01)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x32, 0x06, 0x40
#elif defined (FIRMWARE_2_70) || defined (FIRMWARE_2_76)
#       define RTOC_TABLE       0x80, 0x00, 0x00, 0x00, 0x00, 0x31, 0x3e, 0x70
#elif defined (FIRMWARE_3_21)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x90

#else

#error You must specify the target firmware. \
   define a supported FIRMWARE_X_YZ in config.h and recompile.
#endif 

/* FIRMWARE_X_YZ */
#ifdef USE_JIG
       #define default_shellcode shellcode_egghunt
       #define default_shellcode_macro shellcode_egghunt_macro

//begin Regular Payload, if you don't know what the others are-- this is what you want!	   
#if defined Payload_Regular
#   if defined (FIRMWARE_3_41)
#       define default_payload default_payload_3_41
#       define default_payload_macro default_payload_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41_KIOSK)
#       define default_payload default_payload_3_41_kiosk
#       define default_payload_macro default_payload_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       define default_payload default_payload_3_40
#       define default_payload_macro default_payload_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       define default_payload default_payload_3_30
#       define default_payload_macro default_payload_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       define default_payload default_payload_3_21
#       define default_payload_macro default_payload_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       define default_payload default_payload_3_15
#       define default_payload_macro default_payload_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       define default_payload default_payload_3_10
#       define default_payload_macro default_payload_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       define default_payload default_payload_3_01
#       define default_payload_macro default_payload_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       define default_payload default_payload_2_76
#       define default_payload_macro default_payload_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       define default_payload default_payload_2_70
#       define default_payload_macro default_payload_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
// End Regular Payload
//begin Dev Payload... as of the current PL3 this has more than just peek and poke. If you don't know what... you want Regular.
#elif defined (Payload_Dev)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_dev_3_41.h"
#       define default_payload payload_dev_3_41
#       define default_payload_macro payload_dev_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41_KIOSK)
#       include "pl3/payload_dev_3_41_kiosk.h"
#       define default_payload payload_dev_3_41_kiosk
#       define default_payload_macro payload_dev_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_dev_3_40.h"
#       define default_payload payload_dev_3_40
#       define default_payload_macro payload_dev_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_dev_3_30.h"
#       define default_payload payload_dev_3_30
#       define default_payload_macro payload_dev_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_dev_3_21.h"
#       define default_payload payload_dev_3_21
#       define default_payload_macro payload_dev_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_dev_3_15.h"
#       define default_payload payload_dev_3_15
#       define default_payload_macro payload_dev_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_dev_3_10.h"
#       define default_payload payload_dev_3_10
#       define default_payload_macro payload_dev_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_dev_3_01.h"
#       define default_payload payload_dev_3_01
#       define default_payload_macro payload_dev_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_dev_2_76.h"
#       define default_payload payload_dev_2_76
#       define default_payload_macro payload_dev_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_dev_2_70.h"
#       define default_payload payload_dev_2_70
#       define default_payload_macro payload_dev_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Dev Payload
//begin Payload_Trace_ALL_SC Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_Trace_ALL_SC)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_all_sc_calls_3_41.h"
#       define default_payload payload_trace_all_sc_calls_3_41
#       define default_payload_macro payload_trace_all_sc_calls_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41_KIOSK)
#       include "pl3/payload_trace_all_sc_calls_3_41_kiosk.h"
#       define default_payload payload_trace_all_sc_calls_3_41_kiosk
#       define default_payload_macro payload_trace_all_sc_calls_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_trace_all_sc_calls_3_40.h"
#       define default_payload payload_trace_all_sc_calls_3_40
#       define default_payload_macro payload_trace_all_sc_calls_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_trace_all_sc_calls_3_30.h"
#       define default_payload payload_trace_all_sc_calls_3_30
#       define default_payload_macro payload_trace_all_sc_calls_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_trace_all_sc_calls_3_21.h"
#       define default_payload payload_trace_all_sc_calls_3_21
#       define default_payload_macro payload_trace_all_sc_calls_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_trace_all_sc_calls_3_15.h"
#       define default_payload payload_trace_all_sc_calls_3_15
#       define default_payload_macro payload_trace_all_sc_calls_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_trace_all_sc_calls_3_10.h"
#       define default_payload payload_trace_all_sc_calls_3_10
#       define default_payload_macro payload_trace_all_sc_calls_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_trace_all_sc_calls_3_01.h"
#       define default_payload payload_trace_all_sc_calls_3_01
#       define default_payload_macro payload_trace_all_sc_calls_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_trace_all_sc_calls_2_76.h"
#       define default_payload payload_trace_all_sc_calls_2_76
#       define default_payload_macro payload_trace_all_sc_calls_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#	elif defined (FIRMWARE_2_70)
#       include "pl3/payload_trace_all_sc_calls_2_70.h"
#       define default_payload payload_trace_all_sc_calls_2_70
#       define default_payload_macro payload_trace_all_sc_calls_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Payload_Trace_ALL_SC Payload
//begin NUS Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_NUS)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_no_unauth_syscall_3_41.h"
#       define default_payload payload_no_unauth_syscall_3_41
#       define default_payload_macro payload_no_unauth_syscall_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41_KIOSK)
#       include "pl3/payload_no_unauth_syscall_3_41_kiosk.h"
#       define default_payload payload_no_unauth_syscall_3_41_kiosk
#       define default_payload_macro payload_no_unauth_syscall_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_no_unauth_syscall_3_40.h"
#       define default_payload payload_no_unauth_syscall_3_40
#       define default_payload_macro payload_no_unauth_syscall_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_no_unauth_syscall_3_30.h"
#       define default_payload payload_no_unauth_syscall_3_30
#       define default_payload_macro payload_no_unauth_syscall_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_no_unauth_syscall_3_21.h"
#       define default_payload payload_no_unauth_syscall_3_21
#       define default_payload_macro payload_no_unauth_syscall_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_no_unauth_syscall_3_15.h"
#       define default_payload payload_no_unauth_syscall_3_15
#       define default_payload_macro payload_no_unauth_syscall_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_no_unauth_syscall_3_10.h"
#       define default_payload payload_no_unauth_syscall_3_10
#       define default_payload_macro payload_no_unauth_syscall_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_no_unauth_syscall_3_01.h"
#       define default_payload payload_no_unauth_syscall_3_01
#       define default_payload_macro payload_no_unauth_syscall_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_no_unauth_syscall_2_76.h"
#       define default_payload payload_no_unauth_syscall_2_76
#       define default_payload_macro payload_no_unauth_syscall_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_no_unauth_syscall_2_70.h"
#       define default_payload payload_no_unauth_syscall_2_70
#       define default_payload_macro payload_no_unauth_syscall_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end NUS Payload
//begin Payload_Dump_ELFS Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_Dump_ELFS)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_dump_elfs_3_41.h"
#       define default_payload payload_dump_elfs_3_41
#       define default_payload_macro payload_dump_elfs_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41_KIOSK)
#       include "pl3/payload_dump_elfs_3_41_kiosk.h"
#       define default_payload payload_dump_elfs_3_41_kiosk
#       define default_payload_macro payload_dump_elfs_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_dump_elfs_3_40.h"
#       define default_payload payload_dump_elfs_3_40
#       define default_payload_macro payload_dump_elfs_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_dump_elfs_3_30.h"
#       define default_payload payload_dump_elfs_3_30
#       define default_payload_macro payload_dump_elfs_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_dump_elfs_3_21.h"
#       define default_payload payload_dump_elfs_3_21
#       define default_payload_macro payload_dump_elfs_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_dump_elfs_3_15.h"
#       define default_payload payload_dump_elfs_3_15
#       define default_payload_macro payload_dump_elfs_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_dump_elfs_3_10.h"
#       define default_payload payload_dump_elfs_3_10
#       define default_payload_macro payload_dump_elfs_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_dump_elfs_3_01.h"
#       define default_payload payload_dump_elfs_3_01
#       define default_payload_macro payload_dump_elfs_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xFB, 0xC8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_dump_elfs_2_76.h"
#       define default_payload payload_dump_elfs_2_76
#       define default_payload_macro payload_dump_elfs_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_dump_elfs_2_70.h"
#       define default_payload payload_dump_elfs_2_70
#       define default_payload_macro payload_dump_elfs_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Payload_Dump_ELFS Payload
//begin Payload_Trace_HVC Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_Trace_HVC)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_hypercalls_3_41.h"
#       define default_payload payload_trace_hypercalls_3_41
#       define default_payload_macro payload_trace_hypercalls_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_hypercalls_3_41_kiosk.h"
#       define default_payload payload_trace_hypercalls_3_41_kiosk
#       define default_payload_macro payload_trace_hypercalls_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_trace_hypercalls_3_40.h"
#       define default_payload payload_trace_hypercalls_3_40
#       define default_payload_macro payload_trace_hypercalls_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_trace_hypercalls_3_30.h"
#       define default_payload payload_trace_hypercalls_3_30
#       define default_payload_macro payload_trace_hypercalls_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_trace_hypercalls_3_21.h"
#       define default_payload payload_trace_hypercalls_3_21
#       define default_payload_macro payload_trace_hypercalls_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_trace_hypercalls_3_15.h"
#       define default_payload payload_trace_hypercalls_3_15
#       define default_payload_macro payload_trace_hypercalls_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_trace_hypercalls_3_10.h"
#       define default_payload payload_trace_hypercalls_3_10
#       define default_payload_macro payload_trace_hypercalls_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_trace_hypercalls_3_01.h"
#       define default_payload payload_trace_hypercalls_3_01
#       define default_payload_macro payload_trace_hypercalls_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_trace_hypercalls_2_76.h"
#       define default_payload payload_trace_hypercalls_2_76
#       define default_payload_macro payload_trace_hypercalls_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_trace_hypercalls_2_70.h"
#       define default_payload payload_trace_hypercalls_2_70
#       define default_payload_macro payload_trace_hypercalls_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Payload_Trace_HVC Payload
//begin Payload_Trace_SC Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_Trace_SC)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_syscalls_3_41.h"
#       define default_payload payload_trace_syscalls_3_41
#       define default_payload_macro payload_trace_syscalls_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   if defined (FIRMWARE_3_41_KIOSK)
#       include "pl3/payload_trace_syscalls_3_41_kiosk.h"
#       define default_payload payload_trace_syscalls_3_41_kiosk
#       define default_payload_macro payload_trace_syscalls_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_trace_syscalls_3_40.h"
#       define default_payload payload_trace_syscalls_3_40
#       define default_payload_macro payload_trace_syscalls_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_trace_syscalls_3_30.h"
#       define default_payload payload_trace_syscalls_3_30
#       define default_payload_macro payload_trace_syscalls_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_trace_syscalls_3_21.h"
#       define default_payload payload_trace_syscalls_3_21
#       define default_payload_macro payload_trace_syscalls_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_trace_syscalls_3_15.h"
#       define default_payload payload_trace_syscalls_3_15
#       define default_payload_macro payload_trace_syscalls_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_trace_syscalls_3_10.h"
#       define default_payload payload_trace_syscalls_3_10
#       define default_payload_macro payload_trace_syscalls_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_trace_syscalls_3_01.h"
#       define default_payload payload_trace_syscalls_3_01
#       define default_payload_macro payload_trace_syscalls_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_trace_syscalls_2_76.h"
#       define default_payload payload_trace_syscalls_2_76
#       define default_payload_macro payload_trace_syscalls_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_trace_syscalls_2_70.h"
#       define default_payload payload_trace_syscalls_2_70
#       define default_payload_macro payload_trace_syscalls_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Payload_Trace_SC Payload
//begin Payload_Trace_VUART Payload... If you don't know what this is... you want Regular.
#elif defined (Payload_Trace_VUART)
#   if defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_vuart_3_41.h"
#       define default_payload payload_trace_vuart_3_41
#       define default_payload_macro payload_trace_vuart_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_41)
#       include "pl3/payload_trace_vuart_3_41_kiosk.h"
#       define default_payload payload_trace_vuart_3_41_kiosk
#       define default_payload_macro payload_trace_vuart_3_41_kiosk_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_40)
#       include "pl3/payload_trace_vuart_3_40.h"
#       define default_payload payload_trace_vuart_3_40
#       define default_payload_macro payload_trace_vuart_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#   elif defined (FIRMWARE_3_30)
#       include "pl3/payload_trace_vuart_3_30.h"
#       define default_payload payload_trace_vuart_3_30
#       define default_payload_macro payload_trace_vuart_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70
#   elif defined (FIRMWARE_3_21)
#       include "pl3/payload_trace_vuart_3_21.h"
#       define default_payload payload_trace_vuart_3_21
#       define default_payload_macro payload_trace_vuart_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_15)
#       include "pl3/payload_trace_vuart_3_15.h"
#       define default_payload payload_trace_vuart_3_15
#       define default_payload_macro payload_trace_vuart_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_10)
#       include "pl3/payload_trace_vuart_3_10.h"
#       define default_payload payload_trace_vuart_3_10
#       define default_payload_macro payload_trace_vuart_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#   elif defined (FIRMWARE_3_01)
#       include "pl3/payload_trace_vuart_3_01.h"
#       define default_payload payload_trace_vuart_3_01
#       define default_payload_macro payload_trace_vuart_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xfb, 0xc8
#   elif defined (FIRMWARE_2_76)
#       include "pl3/payload_trace_vuart_2_76.h"
#       define default_payload payload_trace_vuart_2_76
#       define default_payload_macro payload_trace_vuart_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   elif defined (FIRMWARE_2_70)
#       include "pl3/payload_trace_vuart_2_70.h"
#       define default_payload payload_trace_vuart_2_70
#       define default_payload_macro payload_trace_vuart_2_70_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x1b, 0xc8
#   endif
//end Payload_Trace_VUART Payload
//add future playloads here
#endif
/* FIRMWARE_X_YZ */

#define SHELLCODE_PAGE		0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
#define SHELLCODE_DESTINATION	SHELLCODE_ADDR_BASE
#define SHELLCODE_PTR 		SHELLCODE_ADDR_BASE + 0x08
#define SHELLCODE_ADDRESS	SHELLCODE_ADDR_BASE + 0x18

#define PORT1_NUM_CONFIGS	4

#else 
/* USE_JIG Not defined, dump LV2 rather than jailbreak.*/

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

#endif
 /* USE_JIG */
