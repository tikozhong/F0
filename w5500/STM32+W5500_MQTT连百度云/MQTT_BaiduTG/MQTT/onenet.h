#ifndef _ONENET_H_
#define _ONENET_H_
#include "types.h"




void  OneNet_DevLink(void);

void OneNet_Subscribe(const char *topics[], unsigned char topic_cnt);

void OneNet_Publish(const char *topic, const char *msg);

void OneNet_RevPro(unsigned char *cmd);
void MQTT_UnSubscribe(const char *topics[], uint8 topic_cnt);

extern unsigned char publish_buf[64];
#endif
