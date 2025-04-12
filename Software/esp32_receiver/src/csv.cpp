#include <stdio.h>
#include <stddef.h>
#include "util.h"
#include "baja_data_compression.h"
//#include "gps.h"

int make_csv(char* buf, size_t bufLen, const rxdata_t rxdata) {
	baja_data_t sortedData;

	// this contains which data points we received on this packet
	sent_data_points_t newDataPoints = unpack_data(rxdata.data, &sortedData);

	// even worse than the other stuff

	// there has to be a neater way to do this...
	
	// only print the stuff we just received, otherwise empty values for the csv
	

	/*
	 *	buf += written;
	 *	bufLen -= written;
	 *
	 *  the first line moves our pointer so that the next time we write to the buffer
	 *  it is appended after what we just wrote
	 *  
	 *  the second line makes sure we don't write past the buffer
	 */

	size_t written = snprintf(buf, bufLen, "BEGIN");
	buf += written;
	bufLen -= written;

	if(newDataPoints & CVT_DATA) {
		cvt_data_t cvt = sortedData.cvt_data;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,", cvt.primary, cvt.secondary, cvt.temperature);
		buf += written;
		bufLen -= written;	
	} else {
		size_t written = snprintf(buf, bufLen, ",,,");
		buf += written;
		bufLen -= written;
	}

	if(newDataPoints & WHEEL_SPEEDS) {
		wheel_speeds_t wheel = sortedData.wheel_speeds;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,%i,", wheel.fl, wheel.fr, wheel.rl, wheel.rr);
		buf += written;
		bufLen -= written;
	} else {
		size_t written = snprintf(buf, bufLen, ",,,,");
		buf += written;
		bufLen -= written;
	}

	if(newDataPoints & SUS_DISPLACEMENTS) {
		suspension_displacements_t sus = sortedData.sus_displacements;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,%i,", sus.fl, sus.fr, sus.rl, sus.rr);
		buf += written;
		bufLen -= written;
	} else {
		size_t written = snprintf(buf, bufLen, ",,,,");
		buf += written;
		bufLen -= written;
	}

	if(newDataPoints & PEDAL_DATA) {
		pedal_data_t pedal = sortedData.pedal_data;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,%i,", pedal.gas, pedal.brake, pedal.frontPressure, pedal.rearPressure);
		buf += written;
		bufLen -= written;
	} else {
		size_t written = snprintf(buf, bufLen, ",,,,");
		buf += written;
		bufLen -= written;
	}

	if(newDataPoints & ACCEL_DATA) {
		accelerometer_data_t accel = sortedData.accel_data;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,", accel.x, accel.y, accel.z);
		buf += written;
		bufLen -= written;
	} else {
		size_t written = snprintf(buf, bufLen, ",,,");
		buf += written;
		bufLen -= written;
	}
	
	if(newDataPoints & GYRO_DATA) {
		gyroscope_data_t gyro = sortedData.gyro_data;
		size_t written = snprintf(buf, bufLen, "%i,%i,%i,", gyro.yaw, gyro.pitch, gyro.roll);
		buf += written;
		bufLen -= written;	
	} else {
		size_t written = snprintf(buf, bufLen, ",,,");
		buf += written;
		bufLen -= written;
	}

	/*if(newDataPoints & GPS_LAT_LONG) {
		
	}

	if(newDataPoints & GPS_LAT_LONG_CONST) {

	}*/

	if(newDataPoints & GPS_LAT_LONG) {
		gps_lat_long_t gps = sortedData.gps_pos;
		size_t written = snprintf(buf, bufLen, "%.6f,%.6f,", gps.latitude / 100000.f, gps.longitude / 100000.f);
		buf += written;
		bufLen -= written;
	} else {
		size_t written = snprintf(buf, bufLen, ",,");
		buf += written;
		bufLen -= written;
	}

	if(newDataPoints & GPS_OTHER_DATA) {

	}

	written = snprintf(buf, bufLen, "END");
	buf += written;
	bufLen -= written;

	// I forget why this is here, shouldn't it screw up the first char of the buf?
	buf[0] = '\0';

	return 1;
}
