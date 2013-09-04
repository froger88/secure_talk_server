/* 
 * File:   utils.h
 * Author: mazabinski
 *
 * Created on 4 wrzesień 2013, 02:17
 */

#ifndef UTILS_H
#define	UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void write_int16(char* buff, int16_t data);
void write_int32(char* buff, int32_t data);
void write_int64(char* buff, int64_t data);
void write_str(char* buff, const char* data, size_t data_len);

int16_t read_int16(const char* buff);
int32_t read_int32(const char* buff);
int64_t read_int64(const char* buff);

#endif	/* UTILS_H */

