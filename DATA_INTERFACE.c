#include "DATA_INTERFACE.h"
#include "REGISTER_SPACE.h"

/*
Ф-ция для отправки ответа на запрос
receiver_mod_addr - адрес модуля приемника
receiver_chassis_addr - адрес шасси приемника
sender_mod_addr - адрес модуля отправителя
sender_chassis_addr - адрес шасси отправителя
cmd - код команды
data[] - массив с данными
data_size - размер в байтах
device - кто отправляет данные (ПМ - 0, УМ - 1)
*/
void MODBUS_send_data(uint8_t receiver_mod_addr, uint8_t receiver_chassis_addr, uint8_t sender_mod_addr, uint8_t sender_chassis_addr, uint8_t cmd, uint8_t *data, uint8_t data_size, uint8_t device)
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
		service_byte = 0x1; //пока взял такой - "все исправно, управл по шине 1"
	}
	else if (device == 1)
	{
		service_byte = 0x80; //пока взял такой - "все исправно, готов к управлению"
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
Ф-ция для распознавания запроса и отправки ответа
*/
uint8_t MODBUS_read_request_and_send_response(uint8_t *data, uint8_t data_size)
{
	uint8_t pos = 0; //позиция курсора в строке
	//заголовок
	uint8_t header;
	header = MODBUS_read_byte(data,pos);
	pos += 5;
	//адрес получателя
	uint8_t address_receiver; 
	address_receiver = MODBUS_read_byte(data,pos);
	pos += 5;
	//адрес отправителя
	uint8_t address_sender;
	address_sender = MODBUS_read_byte(data,pos);
	pos += 5;
	//длина пакета
	uint16_t data_length;
	for (uint8_t len = 0; len < 2; len++)
	{
		data_length = (data_length << 8) | MODBUS_read_byte(data, pos);
		pos += 5;
	}
	//сервисный байт
	uint8_t service_byte;
	service_byte = MODBUS_read_byte(data,pos);
	pos += 5;
	//код команды 
	uint8_t cmd;
	cmd = MODBUS_read_byte(data,pos);
	pos += 5;
	//поле данных
	uint8_t DATA_length = (data_size-65)/5; //кол-во байт данных
	uint8_t DATA[DATA_length]; // массив поля данных в байтах
//char array[10];
//uint8_t data_in_bytes[100] = {0};
	for (uint8_t len = 0; len < DATA_length; len++)
	{
		DATA[len] = MODBUS_read_byte(data, pos);
//sprintf(array,"%x ",DATA[len]);//операция для преобразования строки данных в поток байтов
//strcat(data_in_bytes,array);
		pos += 5;
	}
	//контрольная сумма
	uint32_t checksum;
	for (uint8_t len = 0; len < 4; len++)
	{
		checksum = (checksum << 8) | MODBUS_read_byte(data, pos);
		pos += 5;
	}
	//признак конца пакета
	uint16_t end;
	for (uint8_t len = 0; len < 2; len++)
	{
		end = (end << 8) | MODBUS_read_byte(data, pos);
		pos += 5;
	}
//	char rx_data[200] = {0};
//	sprintf(rx_data,"%x| %x| %x| %x %x| %x| %x| %s| %x %x %x %x| %x %x\r",header, address_receiver, address_sender, (uint8_t)(data_length>>8), (uint8_t)data_length, service_byte, cmd, data_in_bytes, (uint8_t)(checksum>>24), (uint8_t)(checksum>>16), (uint8_t)(checksum>>8), (uint8_t)checksum, (uint8_t)(end>>8), (uint8_t)end);

//	UART_send_string(rx_data);
	
	// вычисление контрольной суммы и сравнение ее с той, что в телеграмме
	/*
	для примера для строки данных "0x55 0x11 0x22 0x10 0x11 0x80 0x01 0x22 0x5E 0xAD 0xC9 0xC3 0xAA 0xAA " - CRC = 0x5E 0xAD 0xC9 0xC3
	*/
	uint32_t real_checksum;
	uint8_t buf[5 + DATA_length];//буффер для расчета контрольной суммы
	buf[0] = address_sender;
	buf[1] = data_length >> 8;
	buf[2] = data_length & 0xFF;
	buf[3] = service_byte;
	buf[4] = cmd;
	for (int k = 0; k < DATA_length; k++)
	{
		buf[k+5] = DATA[k];
	}
	real_checksum = Find_Crc32(buf, 5 + DATA_length);
	
	if (real_checksum != checksum)
	{
		return 0;
	}
	uint8_t SendBuf[4];
	switch (cmd)
	{
		case TYPE:
			break;
		case INIT:
			SendBuf[0] = (uint8_t)(PLC_SerialNumber>>24);
			SendBuf[1] = (uint8_t)(PLC_SerialNumber>>16);
			SendBuf[2] = (uint8_t)(PLC_SerialNumber>>8);
			SendBuf[3] = (uint8_t)(PLC_SerialNumber);
			MODBUS_send_data(address_receiver&0xF , address_receiver>>4, address_sender&0xF , address_sender>>4, INIT, SendBuf, 4, 0);
			break;
		case READ:
			break;
		case WRITE:
			break;
		case RESET:
			break;
		case CONFIG:
			break;
	}
	
	return 0;
}
/*
ф-ция для распознавания одного байта данных протокола (не символа, а байта - например код команды, адрес и т.п.)
*/
uint8_t MODBUS_read_byte(uint8_t *data, uint8_t pos)
{
	uint8_t byte = 0;
	char array[10] = {0};
	int numb = 0;
	while (data[pos] != ' ')
	{
		array[numb] = data[pos];
		pos++;
		numb++;
	}
	sscanf(array, "%x",&byte);
	return byte;
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