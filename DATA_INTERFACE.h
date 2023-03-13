#ifndef __DATA_INTERFACE_H
#define __DATA_INTERFACE_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define TYPE 0x00
#define INIT 0x01
#define READ 0x02
#define WRITE 0x03
#define RESET 0x04
#define CONFIG 0x05


void send_data(uint8_t receiver_mod_addr, uint8_t receiver_chassis_addr, uint8_t sender_mod_addr, uint8_t sender_chassis_addr, uint8_t cmd, uint8_t *data, uint8_t data_size, uint8_t device);
uint_least32_t Find_Crc32(unsigned char *buf, size_t len);


#endif /*__DATA_INTERFACE_H */