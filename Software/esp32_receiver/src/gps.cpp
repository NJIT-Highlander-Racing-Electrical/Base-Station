#include "gps.h"

gps_lat_long_t process_gps_data(const baja_data_t *baja_data, const sent_data_points_t sent_data_points, gps_lat_long_t *gps_ref) {
	// update offset values if we received them
	if(sent_data_points & GPS_LAT_LONG_CONST) {
		gps_lat_long_t new_ref = baja_data->gps_pos_const;
		gps_ref->latitude = new_ref.latitude;
		gps_ref->longitude = new_ref.longitude;
	}

	if(sent_data_points & GPS_LAT_LONG) {
		gps_lat_long_t corrected;
		gps_lat_long_diff_t difference = baja_data->gps_pos_diff;

		corrected.latitude = gps_ref->latitude + difference.latitude;
		corrected.longitude = gps_ref->longitude + difference.longitude;
		
		return corrected;
	}
	static gps_lat_long_t empty;
	empty.latitude = 0;
	empty.longitude = 0;
	return empty;
}
