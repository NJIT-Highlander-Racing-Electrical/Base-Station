#include "data_compression.h"
#define TWOS_COMP(x) ((~x) + 1)

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

void pack_data(char *buffer, 
		const cvt_data_t *cvt_data, 
		const wheel_speeds_t *wheel_speeds, 
		const suspension_displacements_t *suspension_displacements, 
		const pedal_data_t *pedal_data, 
		const accelerometer_data_t *accelerometer_data, 
		const gyroscope_data_t *gyroscope_data) {
	char bits[DATA_SIZE_BITS] = { 0 };
	int i = 0;

	load_uintx(bits, &i, CVT_RPM_BITS, cvt_data->primary);
	load_uintx(bits, &i, CVT_RPM_BITS, cvt_data->secondary);
	
	load_uintx(bits, &i, WHEEL_SPEED_BITS, wheel_speeds->fl);
	load_uintx(bits, &i, WHEEL_SPEED_BITS, wheel_speeds->fr);
	load_uintx(bits, &i, WHEEL_SPEED_BITS, wheel_speeds->rl);
	load_uintx(bits, &i, WHEEL_SPEED_BITS, wheel_speeds->rr);

	load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, suspension_displacements->fl);
	load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, suspension_displacements->fr);
	load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, suspension_displacements->rl);
	load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, suspension_displacements->rr);

	load_uintx(bits, &i, PEDAL_BITS, pedal_data->gas);
	load_uintx(bits, &i, PEDAL_BITS, pedal_data->brake);

	load_intx(bits, &i, ACCEL_BITS, accelerometer_data->x);
	load_intx(bits, &i, ACCEL_BITS, accelerometer_data->y);
	load_intx(bits, &i, ACCEL_BITS, accelerometer_data->z);

	load_intx(bits, &i, GYRO_BITS, gyroscope_data->yaw);
	load_intx(bits, &i, GYRO_BITS, gyroscope_data->pitch);
	load_intx(bits, &i, GYRO_BITS, gyroscope_data->roll);

	pack_bits(buffer, bits, DATA_SIZE_BITS);
}

void unpack_data(const char *buffer, 
		cvt_data_t *cvt_data, 
		wheel_speeds_t *wheel_speeds, 
		suspension_displacements_t *suspension_displacements, 
		pedal_data_t *pedal_data, 
		accelerometer_data_t *accelerometer_data, 
		gyroscope_data_t *gyroscope_data) {
	char bits[DATA_SIZE_BITS] = { 0 };
	int i = 0;
	
	unpack_bits(buffer, bits, DATA_SIZE_BITS);

	cvt_data->primary = read_uintx(bits, &i, CVT_RPM_BITS);
	cvt_data->secondary = read_uintx(bits, &i, CVT_RPM_BITS);

	wheel_speeds->fl = read_uintx(bits, &i, WHEEL_SPEED_BITS);
	wheel_speeds->fr = read_uintx(bits, &i, WHEEL_SPEED_BITS);
	wheel_speeds->rl = read_uintx(bits, &i, WHEEL_SPEED_BITS);
	wheel_speeds->rr = read_uintx(bits, &i, WHEEL_SPEED_BITS);

	suspension_displacements->fl = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
	suspension_displacements->fr = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
	suspension_displacements->rl = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
	suspension_displacements->rr = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);

	pedal_data->gas = read_uintx(bits, &i, PEDAL_BITS);
	pedal_data->brake = read_uintx(bits, &i, PEDAL_BITS);

	accelerometer_data->x = read_intx(bits, &i, ACCEL_BITS);
	accelerometer_data->y = read_intx(bits, &i, ACCEL_BITS);
	accelerometer_data->z = read_intx(bits, &i, ACCEL_BITS);

	gyroscope_data->yaw = read_intx(bits, &i, GYRO_BITS);
	gyroscope_data->pitch = read_intx(bits, &i, GYRO_BITS);
	gyroscope_data->roll = read_intx(bits, &i, GYRO_BITS);
}

