#include "utils.h"

void write_str(char* buff, const char* data, size_t data_len)
{
	memcpy(buff, data, data_len);
}

void write_int16(char*buff, int16_t data)
{
	memcpy(buff, &data, 2);
}

void write_int32(char*buff, int32_t data)
{
	memcpy(buff, &data, 4);
}

void write_int64(char*buff, int64_t data)
{
	memcpy(buff, &data, 8);

}

int16_t read_int16(const char* buff)
{
	int16_t num;
	memcpy(&num, buff, 2);

	return num;
}

int32_t read_int32(const char* buff)
{
	int32_t num;
	memcpy(&num, buff, 4);

	return num;
}

int64_t read_int64(const char* buff)
{
	int64_t num;
	memcpy(&num, buff, 8);

	return num;
}
