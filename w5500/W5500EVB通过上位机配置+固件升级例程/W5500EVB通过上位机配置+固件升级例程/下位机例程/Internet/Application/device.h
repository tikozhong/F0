#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "stm32f10x.h"
#include "Types.h"
#include"stdio.h"

#define gBUFPUBLIC BUFPUB;
typedef  void (*pFunction)(void);
extern uint8 reboot_flag; //defined in main.c
extern uint8 BUFPUB[2048];
extern uint8_t ip_define;
extern uint8_t ip_eeprom;

void set_default(void);
void set_network(void);
void Reset_W5500(void);
void reboot(void);
void read_config_from_eeprom(void);
void write_config_to_eeprom(void);
void write_temp_to_eeprom(void);
void do_udp_config(void);
#endif

