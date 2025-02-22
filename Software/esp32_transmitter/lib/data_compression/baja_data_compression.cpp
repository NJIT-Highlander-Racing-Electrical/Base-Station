#include "baja_data_compression.h"
#include "data_compression.h"
#include <stdlib.h>

void pack_all_data(char *buffer, 
		const cvt_data_t *cvt_data, 
		const wheel_speeds_t *wheel_speeds, 
		const suspension_displacements_t *suspension_displacements, 
		const pedal_data_t *pedal_data, 
		const accelerometer_data_t *accelerometer_data, 
		const gyroscope_data_t *gyroscope_data,
		const gps_lat_long_diff_t *gps_pos_diff) {
	char bits[DATA_SIZE_BITS] = { 0 };
	int i = 0;

	// put the bitfield for which data points we're sending first
	load_uintx(bits, &i, SENT_DATA_POINTS_BITS, CVT_DATA | WHEEL_SPEEDS |
		SUS_DISPLACEMENTS | PEDAL_DATA | ACCEL_DATA | GYRO_DATA | GPS_LAT_LONG);

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

	load_intx(bits, &i, GPS_POS_DIFF_BITS, gps_pos_diff->latitude);
	load_intx(bits, &i, GPS_POS_DIFF_BITS, gps_pos_diff->longitude);

	pack_bits(buffer, bits, DATA_SIZE_BITS);
}

void unpack_all_data(const char *buffer, 
		cvt_data_t *cvt_data, 
		wheel_speeds_t *wheel_speeds, 
		suspension_displacements_t *suspension_displacements, 
		pedal_data_t *pedal_data, 
		accelerometer_data_t *accelerometer_data, 
		gyroscope_data_t *gyroscope_data,
		gps_lat_long_diff_t *gps_pos_diff) {
	char bits[DATA_SIZE_BITS] = { 0 };
	int i = 0;
	
	unpack_bits(buffer, bits, DATA_SIZE_BITS);

	sent_data_points_t recv_data = read_uintx(bits, &i, SENT_DATA_POINTS_BITS);
	if(recv_data & CVT_DATA
			&& recv_data & WHEEL_SPEEDS
			&& recv_data & SUS_DISPLACEMENTS
			&& recv_data & PEDAL_DATA
			&& recv_data & ACCEL_DATA
			&& recv_data & GYRO_DATA
			&& recv_data & GPS_LAT_LONG) {
		// throw error
		return;
	}

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

	gps_pos_diff->latitude = read_intx(bits, &i, GPS_POS_DIFF_BITS);
	gps_pos_diff->longitude = read_intx(bits, &i, GPS_POS_DIFF_BITS);

}

size_t pack_data(char *buffer, const baja_data_t *baja_data, const sent_data_points_t sent_data_points) {

	int data_size_bits = SENT_DATA_POINTS_BITS;
	if(sent_data_points & CVT_DATA) {
		data_size_bits += CVT_DATA_TOTAL_BITS;
	}

	if(sent_data_points & WHEEL_SPEEDS) {	
		data_size_bits += WHEEL_SPEEDS_TOTAL_BITS;
	}
	
	if(sent_data_points & SUS_DISPLACEMENTS) {
		data_size_bits += SUS_DISPLACEMENTS_TOTAL_BITS;
	}

	if(sent_data_points & PEDAL_DATA) {
		data_size_bits += PEDAL_DATA_TOTAL_BITS;
	}

	if(sent_data_points & ACCEL_DATA) {
		data_size_bits += ACCEL_DATA_TOTAL_BITS;
	}
	
	if(sent_data_points & GYRO_DATA) {
		data_size_bits += GYRO_DATA_TOTAL_BITS;
	}

	if(sent_data_points & GPS_LAT_LONG) {
		data_size_bits += GPS_POS_DIFF_TOTAL_BITS;
	}

	char *bits = (char*)malloc(data_size_bits);
	if(!bits) {
		// TODO: throw error: out of memory
		return 0;
	}
	int i = 0;

	// put the bitfield for which data points we're sending first
	load_uintx(bits, &i, SENT_DATA_POINTS_BITS, sent_data_points);

	if(sent_data_points & CVT_DATA) {
		load_uintx(bits, &i, CVT_RPM_BITS, baja_data->cvt_data.primary);
		load_uintx(bits, &i, CVT_RPM_BITS, baja_data->cvt_data.secondary);
		load_uintx(bits, &i, CVT_TEMP_BITS, baja_data->cvt_data.temperature);
	}

	if(sent_data_points & WHEEL_SPEEDS) {	
		load_uintx(bits, &i, WHEEL_SPEED_BITS, baja_data->wheel_speeds.fl);
		load_uintx(bits, &i, WHEEL_SPEED_BITS, baja_data->wheel_speeds.fr);
		load_uintx(bits, &i, WHEEL_SPEED_BITS, baja_data->wheel_speeds.rl);
		load_uintx(bits, &i, WHEEL_SPEED_BITS, baja_data->wheel_speeds.rr);
	}
	
	if(sent_data_points & SUS_DISPLACEMENTS) {
		load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, baja_data->sus_displacements.fl);
		load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, baja_data->sus_displacements.fr);
		load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, baja_data->sus_displacements.rl);
		load_uintx(bits, &i, SUS_DISPLACEMENT_BITS, baja_data->sus_displacements.rr);
	}

	if(sent_data_points & PEDAL_DATA) {
		load_uintx(bits, &i, PEDAL_BITS, baja_data->pedal_data.gas);
		load_uintx(bits, &i, PEDAL_BITS, baja_data->pedal_data.brake);
	}

	if(sent_data_points & ACCEL_DATA) {
		load_intx(bits, &i, ACCEL_BITS, baja_data->accel_data.x);
		load_intx(bits, &i, ACCEL_BITS, baja_data->accel_data.y);
		load_intx(bits, &i, ACCEL_BITS, baja_data->accel_data.z);
	}
	
	if(sent_data_points & GYRO_DATA) {
		load_intx(bits, &i, GYRO_BITS, baja_data->gyro_data.yaw);
		load_intx(bits, &i, GYRO_BITS, baja_data->gyro_data.pitch);
		load_intx(bits, &i, GYRO_BITS, baja_data->gyro_data.roll);
	}

	if(sent_data_points & GPS_LAT_LONG) {
		load_intx(bits, &i, GPS_POS_DIFF_BITS, baja_data->gps_pos_diff.latitude);
		load_intx(bits, &i, GPS_POS_DIFF_BITS, baja_data->gps_pos_diff.longitude);
	}

	pack_bits(buffer, bits, data_size_bits);
	free(bits);

	if(data_size_bits % 8 == 0) {
		return data_size_bits / 8;
	}
	return (data_size_bits / 8) + 1;
}

sent_data_points_t unpack_data(const char *buffer, baja_data_t *baja_data) {

	int i = 0;
	char bits_initial[SENT_DATA_POINTS_BITS] = { 0 };
	unpack_bits(buffer, bits_initial, SENT_DATA_POINTS_BITS);
	sent_data_points_t recv_data_points = read_uintx(bits_initial, &i, SENT_DATA_POINTS_BITS);

	int data_size_bits = SENT_DATA_POINTS_BITS;
	if(recv_data_points & CVT_DATA) {
		data_size_bits += CVT_DATA_TOTAL_BITS;
	}

	if(recv_data_points & WHEEL_SPEEDS) {	
		data_size_bits += WHEEL_SPEEDS_TOTAL_BITS;
	}
	
	if(recv_data_points & SUS_DISPLACEMENTS) {
		data_size_bits += SUS_DISPLACEMENTS_TOTAL_BITS;
	}

	if(recv_data_points & PEDAL_DATA) {
		data_size_bits += PEDAL_DATA_TOTAL_BITS;
	}

	if(recv_data_points & ACCEL_DATA) {
		data_size_bits += ACCEL_DATA_TOTAL_BITS;
	}
	
	if(recv_data_points & GYRO_DATA) {
		data_size_bits += GYRO_DATA_TOTAL_BITS;
	}

	if(recv_data_points & GPS_LAT_LONG) {
		data_size_bits += GPS_POS_DIFF_TOTAL_BITS;
	}

	char *bits = (char*)malloc(data_size_bits);
	if(!bits) {
		// TODO: throw error: out of memory
		return 0;
	}

	unpack_bits(buffer, bits, data_size_bits);

	// this shouldn't be necessary (along with the line below)
	i = 0;
	// read this again to make my programming life easier
	// probably should change with i starting from SENT_DATA_POINTS_BITS
	read_uintx(bits, &i, SENT_DATA_POINTS_BITS);
	

	if(recv_data_points & CVT_DATA) {
		baja_data->cvt_data.primary = read_uintx(bits, &i, CVT_RPM_BITS);
		baja_data->cvt_data.secondary = read_uintx(bits, &i, CVT_RPM_BITS);
		baja_data->cvt_data.temperature = read_uintx(bits, &i, CVT_TEMP_BITS);
	}

	if(recv_data_points & WHEEL_SPEEDS) {	
		baja_data->wheel_speeds.fl = read_uintx(bits, &i, WHEEL_SPEED_BITS);
		baja_data->wheel_speeds.fr = read_uintx(bits, &i, WHEEL_SPEED_BITS);
		baja_data->wheel_speeds.rl = read_uintx(bits, &i, WHEEL_SPEED_BITS);
		baja_data->wheel_speeds.rr = read_uintx(bits, &i, WHEEL_SPEED_BITS);
	}
	
	if(recv_data_points & SUS_DISPLACEMENTS) {
		baja_data->sus_displacements.fl = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
		baja_data->sus_displacements.fr = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
		baja_data->sus_displacements.rl = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
		baja_data->sus_displacements.rr = read_uintx(bits, &i, SUS_DISPLACEMENT_BITS);
	}

	if(recv_data_points & PEDAL_DATA) {
		baja_data->pedal_data.gas = read_uintx(bits, &i, PEDAL_BITS);
		baja_data->pedal_data.brake = read_uintx(bits, &i, PEDAL_BITS);
	}

	if(recv_data_points & ACCEL_DATA) {
		baja_data->accel_data.x = read_intx(bits, &i, ACCEL_BITS);
		baja_data->accel_data.y = read_intx(bits, &i, ACCEL_BITS);
		baja_data->accel_data.z = read_intx(bits, &i, ACCEL_BITS);
	}
	
	if(recv_data_points & GYRO_DATA) {
		baja_data->gyro_data.yaw = read_intx(bits, &i, GYRO_BITS);
		baja_data->gyro_data.pitch = read_intx(bits, &i, GYRO_BITS);
		baja_data->gyro_data.roll = read_intx(bits, &i, GYRO_BITS);
	}

	if(recv_data_points & GPS_LAT_LONG) {
		baja_data->gps_pos_diff.latitude = read_intx(bits, &i, GPS_POS_DIFF_BITS);
		baja_data->gps_pos_diff.longitude = read_intx(bits, &i, GPS_POS_DIFF_BITS);
	}
	
	free(bits);

	return recv_data_points;
}
