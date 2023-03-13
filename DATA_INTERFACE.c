#include "DATA_INTERFACE.h"


/*
receiver_mod_addr - адрес модуля приемника
receiver_chassis_addr - адрес шасси приемника
sender_mod_addr - адрес модуля отправителя
sender_chassis_addr - адрес шасси отправителя
cmd - код команды
data[] - массив с данными
device - кто отправляет данные (ПМ - 0, УМ - 1)
*/
void send_data(uint8_t receiver_mod_addr, uint8_t receiver_chassis_addr, uint8_t sender_mod_addr, uint8_t sender_chassis_addr, uint8_t cmd, uint8_t *data, uint8_t data_size, uint8_t device)
{
	uint8_t DATA[200] = {0};
	uint8_t header = 0x55; //заголовок
	uint8_t address_receiver; //адрес получателя
	uint8_t address_sender; //адрес отправителя
	uint16_t data_length; //длина пакета
	uint8_t service_byte; //сервисный байт
	uint32_t checksum; //контрольная сумма
	uint16_t end = 0xAAAA; //признак конца пакета
	
	uint8_t data_in_bytes[100] = {0};
	char array[10] = {0};
	
	address_receiver = (receiver_chassis_addr << 4) | receiver_mod_addr;
	address_sender = (sender_chassis_addr << 4) | sender_mod_addr;
	data_length = 10 + data_size;
	if (device == 0)
	{
		service_byte = 0x1; //пока взял такой - все исправно, управл по шине 1
	}
	else if (device == 1)
	{
		service_byte = 0x80; //пока взял такой - все исправно, готов к управлению
	}
	
	uint8_t buf[5 + data_size];//буффер для расчета контрольной суммы
	buf[0] = address_sender;
	buf[1] = data_length >> 8;
	buf[2] = data_length & 0xFF;
	buf[3] = service_byte;
	buf[4] = cmd;
	for (int k = 0; k < data_size; k++)
	{
		buf[k+5] = data[k];
		sprintf(array,"%x ",data[k]);//операция для преобразования строки данных в поток байтов
		strcat(data_in_bytes,array);
	}
	checksum = Find_Crc32(buf, 5 + data_size);
	
	sprintf(DATA,"%x| %x| %x| %x %x| %x| %x| %s| %x %x %x %x| %x %x",header, address_receiver, address_sender, buf[1], buf[2], service_byte, cmd, data_in_bytes, (uint8_t)(checksum>>24), (uint8_t)(checksum>>16), (uint8_t)(checksum>>8), (uint8_t)checksum, (uint8_t)(end>>8), (uint8_t)end);

	strcat(DATA, "\r");
	UART_send_string(DATA);
}

/*
buf[] - масив с данными
len - длина в байтах
например для строки "123456789" - CRC = cbf43926
*/
uint_least32_t Find_Crc32(unsigned char *buf, size_t len)
{
	uint_least32_t crc_table[256];
	uint_least32_t crc; int i, j;

	for (i = 0; i < 256; i++)
	{
		crc = i;
		for (j = 0; j < 8; j++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

		crc_table[i] = crc;
	};

	crc = 0xFFFFFFFFUL;

	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

	return crc ^ 0xFFFFFFFFUL;
}