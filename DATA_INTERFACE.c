#include "DATA_INTERFACE.h"
#include "REGISTER_SPACE.h"

/*
Ф-ция для отправки ответа на запрос
receiver_mod_addr - адрес модуля приемника
receiver_chassis_addr - адрес шасси приемника
sender_mod_addr - адрес модуля отправителя
sender_chassis_addr - адрес шасси отправителя
cmd - код команды
data[] - массив байтов данных
data_size - размер в байтах
device - кто отправляет данные (ПМ - 0, УМ - 1)
*/
void MODBUS_send_data(uint8_t receiver_mod_addr, uint8_t receiver_chassis_addr, uint8_t sender_mod_addr, uint8_t sender_chassis_addr, uint8_t cmd, uint8_t data[], uint8_t data_size, uint8_t device)
{
	uint8_t DATA[200] = {0};//отправляемый массив байтов
	
	uint8_t header = 0x55; //заголовок
	uint8_t address_receiver; //адрес получателя
	uint8_t address_sender; //адрес отправителя
	uint16_t data_length; //длина пакета
	uint8_t service_byte; //сервисный байт
	uint32_t checksum; //контрольная сумма
	uint16_t end = 0xAAAA; //признак конца пакета

	DATA[0] = header;
	address_receiver = (receiver_chassis_addr << 4) | receiver_mod_addr;
	DATA[1] = address_receiver;
	address_sender = (sender_chassis_addr << 4) | sender_mod_addr;
	DATA[2] = address_sender;
	data_length = 10 + data_size;
	DATA[3] = data_length & 0xFF;
	DATA[4] = data_length>>8;
	if (device == 0)
	{
		service_byte = 0x1; //пока взял такой - "все исправно, управл по шине 1"
	}
	else if (device == 1)
	{
		service_byte = 0x80; //пока взял такой - "все исправно, готов к управлению"
	}
	DATA[5] = service_byte;
	DATA[6] = cmd;
	for (uint8_t len = 0;len < data_size; len++)
	{
		DATA[7+len] = data[len];
	}
	
	uint8_t buf[5 + data_size];//буффер для расчета контрольной суммы
	buf[0] = address_sender;
	buf[1] = data_length & 0xFF;
	buf[2] = data_length >> 8;
	buf[3] = service_byte;
	buf[4] = cmd;
	for (int k = 0; k < data_size; k++)
	{
		buf[k+5] = data[k];
	}
	checksum = Find_Crc32(buf, 5 + data_size);
	DATA[7+data_size] = checksum & 0xFF;
	DATA[7+data_size+1] = checksum>>8;
	DATA[7+data_size+2] = checksum>>16;
	DATA[7+data_size+3] = checksum>>24;
	
	DATA[7+data_size+4] = end & 0xFF;
	DATA[7+data_size+5] = end>>8;

	uart_write(DATA, 13+data_size);
}
/*
Ф-ция для распознавания запроса и отправки ответа
*/
uint8_t MODBUS_read_request_and_send_response()
{
	uint8_t data[2048];//телеграмма
	uint32_t data_size;//размер телеграммы
	uint8_t buffer[2048];//весь считанный буфер
	
	if (uart_read(2048, buffer) == 0)
	{
		return 0;
	}
	//ожидаем пока не придет весь пакет (по заголовку определить длину пакета, и считать столько то байт)
	
	//распознавание границ пакета
	uint32_t count = 0;
	if (buffer[0] != 0x55)
	{
		return 0;
	}
	while ((buffer[count] != 0xAA)&&(buffer[count-1] != 0xAA))
	{
		if(count == 2049)
		{
			return 0;
		}
		data[count] = buffer[count];
		count++;
	}
	data[count] = buffer[count];
	data[count+1] = buffer[count+1];
	data_size = count+2;
	uart_set_pos(data_size + uart_read_pos());
	uart_write(data, data_size);
	/*
	//заголовок
	uint8_t header;
	header = data[0];
	//адрес получателя
	uint8_t address_receiver; 
	address_receiver = data[1];	
	//адрес отправителя
	uint8_t address_sender;
	address_sender = data[2];
	//длина пакета
	uint16_t data_length = 0;
	for (int len = 1; len >= 0; len--)
	{
		data_length = data_length << 8;
		data_length |= data[3+len];
	}
	//сервисный байт
	uint8_t service_byte;
	service_byte = data[5];
	//код команды 
	uint8_t cmd;
	cmd = data[6];
	//поле данных
	uint8_t DATA_length = data_size-13; //кол-во байт данных
	uint8_t DATA[DATA_length]; // массив поля данных в байтах
	for (uint8_t len = 0; len < DATA_length; len++)
	{
		DATA[len] = data[7+len];
	}
	//контрольная сумма
	uint32_t checksum = 0;
	for (int len = 3; len >= 0; len--)
	{
		checksum = checksum << 8;
		checksum |= data[7+DATA_length+len];
	}
	//признак конца пакета
	uint16_t end = 0;
	for (int len = 1; len >= 0; len--)
	{
		end = end << 8;
		end |= data[11+DATA_length+len];
	}
	*/
	// вычисление контрольной суммы и сравнение ее с той, что в телеграмме
	/*
	для примера для строки данных "0x55 0x11 0x22 0x10 0x11 0x80 0x01 0x22 0x5E 0xAD 0xC9 0xC3 0xAA 0xAA " - CRC = 0x5E 0xAD 0xC9 0xC3
	*/
	/*
	uint32_t real_checksum;
	uint8_t buf[5 + DATA_length];//буффер для расчета контрольной суммы
	buf[0] = address_sender;
	buf[1] = data_length & 0xFF;
	buf[2] = data_length >> 8;
	buf[3] = service_byte;
	buf[4] = cmd;
	for (int k = 0; k < DATA_length; k++)
	{
		buf[k+5] = DATA[k];
	}
	real_checksum = Find_Crc32(buf, 5 + DATA_length);
	//если контрольная сумма не верна, то выход из функции
	if (real_checksum != checksum)
	{
		return 0;
	}
	//отправка ответа
	uint8_t SendBuf[16];
	switch (cmd)
	{
		case TYPE:
			//PLC_SoftVer
			for(int i = 0; i < 4; i++)
			{
				SendBuf[i] = read_register(0, 4, COMMON)[i];
			}
			//PLC_DeviceType
			for(int i = 0; i < 4; i++)
			{
				SendBuf[i+4] = read_register(1160, 4, COMMON)[i];
			}
			//PLC_Config
			for(int i = 0; i < 4; i++)
			{
				SendBuf[i+8] = read_register(4, 2, COMMON)[i];
			}
			//PLC_SerialNumber
			for(int i = 0; i < 4; i++)
			{
				SendBuf[i+10] = read_register(1164, 4, COMMON)[i];
			}
			MODBUS_send_data(address_sender&0xF , address_sender>>4, address_receiver&0xF , address_receiver>>4, TYPE, SendBuf, 14, 0);
			break;
		case INIT:
			for(int i = 0; i < 4; i++)
			{
				SendBuf[i] = read_register(1164, 4, COMMON)[i];
			}
			MODBUS_send_data(address_sender&0xF , address_sender>>4, address_receiver&0xF , address_receiver>>4, INIT, SendBuf, 4, 0);
			break;
		case READ:	
			//если адрес регистра равен 1662 
			if((DATA[0] == 0x7E)&&(DATA[1] == 0x06))
			{
				for(int i = 0; i < 16; i++)
				{
					SendBuf[i] = read_register(1662, 16, MPA)[i];
				}	
				MODBUS_send_data(address_sender&0xF , address_sender>>4, address_receiver&0xF , address_receiver>>4, READ, SendBuf, 16, 0);
			}
			else if ((DATA[0] == 0xAE)&&(DATA[1] == 0x06))
			{
				for(int i = 0; i < 16; i++)
				{
					SendBuf[i] = read_register(1710, 16, MPA)[i];
				}				
				MODBUS_send_data(address_sender&0xF , address_sender>>4, address_receiver&0xF , address_receiver>>4, READ, SendBuf, 16, 0);
			}
			break;
		case WRITE:
			break;
		case RESET:
			SendBuf[0] = 0x04;
			MODBUS_send_data(address_sender&0xF , address_sender>>4, address_receiver&0xF , address_receiver>>4, RESET, SendBuf, 1, 0);
			//сброс всех регистров
			//инициализация пространства регистров
			registers_ini();
			break;
		case CONFIG:
			break;
	}
	*/
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
	}

	crc = 0xFFFFFFFFUL;

	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

	return crc ^ 0xFFFFFFFFUL;
}