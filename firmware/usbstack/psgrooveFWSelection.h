/*
PSGroove header to configure payload selections
*/

#if defined (FIRMWARE_3_41) || defined (FIRMWARE_3_40)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xe7, 0x20
#elif defined (FIRMWARE_3_15) || defined (FIRMWARE_3_10)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x10
#elif defined (FIRMWARE_3_01)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x32, 0x06, 0x40
#elif defined (FIRMWARE_2_76)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x31, 0x3E, 0x70
#elif defined (FIRMWARE_3_21)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xda, 0x90
#elif defined (FIRMWARE_3_30)
#       define RTOC_TABLE		0x80, 0x00, 0x00, 0x00, 0x00, 0x33, 0xdb, 0xc0

#else

#error You must specify the target firmware. \
   define a supported FIRMWARE_X_YZ in config.h and recompile.
#endif 

/* FIRMWARE_X_YZ */
#ifdef USE_JIG
       #define default_shellcode shellcode_egghunt
       #define default_shellcode_macro shellcode_egghunt_macro

#if defined (FIRMWARE_3_41)
#       define default_payload default_payload_3_41
#       define default_payload_macro default_payload_3_41_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#elif defined (FIRMWARE_3_15)
#       define default_payload default_payload_3_15
#       define default_payload_macro default_payload_3_15_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#elif defined (FIRMWARE_3_10)
#       define default_payload default_payload_3_10
#       define default_payload_macro default_payload_3_10_macro
#       define SHELLCODE_ADDR_BASE  0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#elif defined (FIRMWARE_3_01)
#       define default_payload default_payload_3_01
#       define default_payload_macro default_payload_3_01_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xFB, 0xC8
#elif defined (FIRMWARE_3_40)
#       define default_payload default_payload_3_40
#       define default_payload_macro default_payload_3_40_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xee, 0x70
#elif defined (FIRMWARE_2_76)
#       define default_payload default_payload_2_76
#       define default_payload_macro default_payload_2_76_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x1B, 0xC8
#elif defined (FIRMWARE_3_21)
#       define default_payload default_payload_3_21
#       define default_payload_macro default_payload_3_21_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x30
#elif defined (FIRMWARE_3_30)
#       define default_payload default_payload_3_30
#       define default_payload_macro default_payload_3_30_macro
#       define SHELLCODE_ADDR_BASE	0x80, 0x00, 0x00, 0x00, 0x00, 0x3d, 0xde, 0x70

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
