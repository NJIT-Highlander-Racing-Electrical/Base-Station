#include "data_compression.h"

void load_uintx(char *bits, int *index, const int x, const uint16_t data) {
	for(int i = 0; i < x; ++i) {
		bits[(*index)++] = (data & (1 << i)) >> i;
	}
}

// signed int stored with signed magnitude
void load_intx(char *bits, int *index, const int x, const int16_t data) {
	uint16_t unsigned_data = (uint16_t)data;
	char sign = unsigned_data >> 15;
	if(sign) unsigned_data = TWOS_COMP(unsigned_data); // get magnitude
	load_uintx(bits, index, x-1, unsigned_data);
	bits[(*index)++] = sign;
}

uint16_t read_uintx(const char *bits, int *index, const int x) {
	uint16_t val = 0;
	for(int i = 0; i < x; ++i) {
		val |= (bits[(*index)++] & 1) << i;
	}
	return val;
}

int16_t read_intx(const char *bits, int *index, const int x) {
	int16_t val = 0;
	uint16_t val_magnitude = read_uintx(bits, index, x-1);
	char sign = bits[(*index)++] & 1;
	val = sign ? -val_magnitude : val_magnitude;
	return val;
	
}

void pack_bits(char *buffer, const char *bits, const int numBits) {
	for(int i = 0; i < numBits; ++i) {
		int currentByte = i / 8;
		buffer[currentByte] |= (bits[i] & 1) << (7 - (i % 8));
	}
}

void unpack_bits(const char *buffer, char *bits, const int numBits) {
	for(int i = 0; i < numBits; ++i) {
		int currentByte = i / 8;
		bits[i] = (buffer[currentByte] >> (7 - (i % 8))) & 1;
	}
}

// UNTESTED WITH INT32, CHANGE BACK IF HAVING PROBLEMS!!!!!!!

/*void load_uintx(char *bits, int *index, const int x, const uint32_t data) {
	for(int i = 0; i < x; ++i) {
		bits[(*index)++] = (data & (1 << i)) >> i;
	}
}

// signed int stored with signed magnitude
void load_intx(char *bits, int *index, const int x, const int32_t data) {
	uint32_t unsigned_data = (uint32_t)data;
	char sign = unsigned_data >> 31;
	if(sign) unsigned_data = TWOS_COMP(unsigned_data); // get magnitude
	load_uintx(bits, index, x-1, unsigned_data);
	bits[(*index)++] = sign;
}

uint32_t read_uintx(const char *bits, int *index, const int x) {
	uint32_t val = 0;
	for(int i = 0; i < x; ++i) {
		val |= (bits[(*index)++] & 1) << i;
	}
	return val;
}

int32_t read_intx(const char *bits, int *index, const int x) {
	int32_t val = 0;
	uint32_t val_magnitude = read_uintx(bits, index, x-1);
	char sign = bits[(*index)++] & 1;
	val = sign ? -val_magnitude : val_magnitude;
	return val;
	
}

void pack_bits(char *buffer, const char *bits, const int numBits) {
	for(int i = 0; i < numBits; ++i) {
		int currentByte = i / 8;
		buffer[currentByte] |= (bits[i] & 1) << (7 - (i % 8));
	}
}

void unpack_bits(const char *buffer, char *bits, const int numBits) {
	for(int i = 0; i < numBits; ++i) {
		int currentByte = i / 8;
		bits[i] = (buffer[currentByte] >> (7 - (i % 8))) & 1;
	}
}*/
