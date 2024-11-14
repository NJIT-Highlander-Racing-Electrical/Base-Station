#define DATA_SIZE_BITS 173
#define CVT_RPM_BITS 9
#define WHEEL_SPEED_BITS 10
#define SUS_DISPLACEMENT_BITS 8
#define PEDAL_BITS 7
#define ACCEL_BITS 9
#define GYRO_BITS 14

// units: rpm, degrees ?
typedef struct {
	uint16_t primary;
	uint16_t secondary;
	uint8_t temperature;
} cvt_data_t;

// units: rpm
typedef struct {
	uint16_t fl;
	uint16_t fr;
	uint16_t rl;
	uint16_t rr;
} wheel_speeds_t;

// units: percent
typedef struct {
	uint8_t fl;
	uint8_t fr;
	uint8_t rl;
	uint8_t rr;
} suspension_displacements_t;

// units: percent
typedef struct {
	uint8_t gas;
	uint8_t brake;
} pedal_data_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} accelerometer_data_t;

typedef struct {
	int16_t yaw;
	int16_t pitch;
	int16_t roll;
} gyroscope_data_t;

void pack_data(char *buffer, 
		const cvt_data_t *cvt_data, 
		const wheel_speeds_t *wheel_speeds, 
		const suspension_displacements_t *suspension_displacements, 
		const pedal_data_t *pedal_data, 
		const accelerometer_data_t *accelerometer_data, 
		const gyroscope_data_t *gyroscope_data);

void unpack_data(const char *buffer, 
		cvt_data_t *cvt_data, 
		wheel_speeds_t *wheel_speeds, 
		suspension_displacements_t *suspension_displacements, 
		pedal_data_t *pedal_data, 
		accelerometer_data_t *accelerometer_data, 
		gyroscope_data_t *gyroscope_data);

