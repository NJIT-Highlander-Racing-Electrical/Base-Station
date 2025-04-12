#include <stdint.h>
#include <stddef.h>

#ifndef BAJA_DATA_COMPRESSION_H
#define BAJA_DATA_COMPRESSION_H

// unused, this is used for pack/unpack_all_data
//#define DATA_SIZE_BITS 210

// bit field for sent data points
#define SENT_DATA_POINTS_BITS 9
typedef uint16_t sent_data_points_t;

#define CVT_DATA           0b1
#define WHEEL_SPEEDS       0b10
#define SUS_DISPLACEMENTS  0b100
#define PEDAL_DATA         0b1000
#define ACCEL_DATA         0b10000
#define GYRO_DATA          0b100000
#define GPS_LAT_LONG       0b1000000
#define GPS_LAT_LONG_CONST 0b10000000
#define GPS_OTHER_DATA     0b100000000

// units: rpm, degrees ?
#define CVT_DATA_TOTAL_BITS 26
#define CVT_RPM_BITS 9
#define CVT_TEMP_BITS 8
typedef struct {
	uint16_t primary : CVT_RPM_BITS;
	uint16_t secondary : CVT_RPM_BITS;
	uint8_t temperature;
} cvt_data_t;

// units: rpm
#define WHEEL_SPEEDS_TOTAL_BITS 40
#define WHEEL_SPEED_BITS 10
typedef struct {
	uint16_t fl : WHEEL_SPEED_BITS;
	uint16_t fr : WHEEL_SPEED_BITS;
	uint16_t rl : WHEEL_SPEED_BITS;
	uint16_t rr : WHEEL_SPEED_BITS;
} wheel_speeds_t;

// units: percent
#define SUS_DISPLACEMENTS_TOTAL_BITS 32
#define SUS_DISPLACEMENT_BITS 8
typedef struct {
	uint8_t fl : SUS_DISPLACEMENT_BITS;
	uint8_t fr : SUS_DISPLACEMENT_BITS;
	uint8_t rl : SUS_DISPLACEMENT_BITS;
	uint8_t rr : SUS_DISPLACEMENT_BITS;
} suspension_displacements_t;

// units: percent
#define PEDAL_DATA_TOTAL_BITS 38
#define PEDAL_BITS 7
#define PRESSURE_BITS 12
typedef struct {
	uint8_t gas : PEDAL_BITS;
	uint8_t brake : PEDAL_BITS;
	uint16_t frontPressure : PRESSURE_BITS;
	uint16_t rearPressure : PRESSURE_BITS;
} pedal_data_t;

#define ACCEL_DATA_TOTAL_BITS 27
#define ACCEL_BITS 9
typedef struct {
	int16_t x : ACCEL_BITS;
	int16_t y : ACCEL_BITS;
	int16_t z : ACCEL_BITS;
} accelerometer_data_t;

#define GYRO_DATA_TOTAL_BITS 42
#define GYRO_BITS 14
typedef struct {
	int16_t yaw : GYRO_BITS;
	int16_t pitch : GYRO_BITS;
	int16_t roll : GYRO_BITS;
} gyroscope_data_t;

#define GPS_POS_DIFF_TOTAL_BITS 28
#define GPS_POS_DIFF_BITS 14
typedef struct {
	int16_t latitude : GPS_POS_DIFF_BITS;
	int16_t longitude : GPS_POS_DIFF_BITS;
} gps_lat_long_diff_t;

#define GPS_POS_CONST_TOTAL_BITS 50
typedef struct {
	int32_t latitude : 25;
	int32_t longitude : 25;
} gps_lat_long_t;

typedef struct {
	cvt_data_t cvt_data;
	wheel_speeds_t wheel_speeds;
	suspension_displacements_t sus_displacements;
	pedal_data_t pedal_data;
	accelerometer_data_t accel_data;
	gyroscope_data_t gyro_data;
	gps_lat_long_diff_t gps_pos_diff;
	gps_lat_long_t gps_pos_const;
} baja_data_t;

size_t pack_data(char *buffer, const baja_data_t *baja_data, const sent_data_points_t sent_data_points);
sent_data_points_t unpack_data(const char *buffer, baja_data_t *baja_data);

// unused
/*void pack_all_data(char *buffer, 
		const cvt_data_t *cvt_data, 
		const wheel_speeds_t *wheel_speeds, 
		const suspension_displacements_t *suspension_displacements, 
		const pedal_data_t *pedal_data, 
		const accelerometer_data_t *accelerometer_data, 
		const gyroscope_data_t *gyroscope_data,
		const gps_lat_long_diff_t *gps_pos_diff);

void unpack_all_data(const char *buffer, 
		cvt_data_t *cvt_data, 
		wheel_speeds_t *wheel_speeds, 
		suspension_displacements_t *suspension_displacements, 
		pedal_data_t *pedal_data, 
		accelerometer_data_t *accelerometer_data, 
		gyroscope_data_t *gyroscope_data,
		gps_lat_long_diff_t *gps_pos_diff);
*/

#endif
