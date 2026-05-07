#include <Arduino.h>

#include <heltec_unofficial.h>
#include <baja_data_compression.h>

#include <BajaCAN.h>

#define FREQUENCY 905.2
#define BANDWIDTH_FAST 500
#define SPREADING_FACTOR_FAST 7

#define BANDWIDTH_SLOW 125
#define SPREADING_FACTOR_SLOW 9

#define SPEED_SWITCH_PIN 39
#define TRANSMIT_POWER 22

//#define RADIO_TEST

bool speedSwitchState;

// Declare these here since BajaCAN.h no longer provides them
baja_data_t all_data;
sent_data_points_t current_recv_data_points = 0;

union crc_value
{
	uint16_t intVal;
	char strVal[2];
};

void setup()
{
	Serial.begin(115200);

	RadioLibCRCInstance.size = 16;
	RadioLibCRCInstance.poly = 0x755B;
	RadioLibCRCInstance.init = 0xFFFF;
	RadioLibCRCInstance.out = 0x0000;

	heltec_setup();
	heltec_ve(true);

	pinMode(SPEED_SWITCH_PIN, INPUT_PULLUP);
	speedSwitchState = digitalRead(SPEED_SWITCH_PIN);

	both.println("Radio init");
	radio.begin(FREQUENCY, speedSwitchState ? BANDWIDTH_SLOW : BANDWIDTH_FAST, speedSwitchState ? SPREADING_FACTOR_SLOW : SPREADING_FACTOR_FAST, 7, 0x13, 0, 8);
	both.println(speedSwitchState ? "SLOW" : "FAST");
	if (radio.setCRC(0) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION)
	{
		both.println("bad bad bad");
	}
	radio.setOutputPower(TRANSMIT_POWER);

	setupCAN(BASE_STATION);
}

int a = 0;

int16_t send_packet(sent_data_points_t sd, const baja_data_t* bt)
{
	char buf[100];
	memset(buf, 0, 100);

	size_t byteCount = pack_data(buf, bt, sd);

	union crc_value checksum;
	checksum.intVal = RadioLibCRCInstance.checksum((const uint8_t*)buf, byteCount);

	char* fullStr = (char*)malloc(byteCount + 2);
	memcpy((void*)fullStr, (const void*)buf, byteCount);
	fullStr[byteCount] = checksum.strVal[0];
	fullStr[byteCount + 1] = checksum.strVal[1];

	int16_t status = radio.transmit((const uint8_t*)fullStr, byteCount + 2, 0);

	free(fullStr);

	return status;
}

// Helper to populate all_data from the flat BajaCAN.h variables
void populate_all_data()
{
	memset(&all_data, 0, sizeof(all_data));

	all_data.cvt_data.primary = primaryRPM;
	all_data.cvt_data.secondary = secondaryRPM;
	all_data.cvt_data.temperature = primaryTemperature;

	all_data.wheel_speeds.fl = frontLeftWheelSpeed;
	all_data.wheel_speeds.fr = frontRightWheelSpeed;
	all_data.wheel_speeds.rl = rearLeftWheelSpeed;
	all_data.wheel_speeds.rr = rearRightWheelSpeed;

	all_data.pedal_data.gas = gasPedalPercentage;
	all_data.pedal_data.brake = brakePedalPercentage;
	all_data.pedal_data.frontPressure = frontBrakePressure;
	all_data.pedal_data.rearPressure = rearBrakePressure;

	all_data.accel_data.x = accelerationX;
	all_data.accel_data.y = accelerationY;
	all_data.accel_data.z = accelerationZ;

	all_data.gyro_data.roll = gyroscopeRoll;
	all_data.gyro_data.pitch = gyroscopePitch;
	all_data.gyro_data.yaw = gyroscopeYaw;

	all_data.gps_pos.latitude = gpsLatitude;
	all_data.gps_pos.longitude = gpsLongitude;
}

void loop()
{
	heltec_loop();

	bool newSwitchState;
	if ((newSwitchState = digitalRead(SPEED_SWITCH_PIN)) != speedSwitchState) {
		speedSwitchState = newSwitchState;
		if (!speedSwitchState) {
			radio.setBandwidth(BANDWIDTH_FAST);
			radio.setSpreadingFactor(SPREADING_FACTOR_FAST);
			both.println("-> FAST");
		}
		else {
			radio.setBandwidth(BANDWIDTH_SLOW);
			radio.setSpreadingFactor(SPREADING_FACTOR_SLOW);
			both.println("-> SLOW");
		}
	}

	#ifdef RADIO_TEST
	if (button.isSingleClick())
	{
		both.printf("Button %i\n", ++a);

		baja_data_t bt;
		sent_data_points_t sd = 0;
		sd |= CVT_DATA;
		sd |= PEDAL_DATA;
		sd |= GPS_LAT_LONG;

		for (int i = 0; i < 500; ++i)
		{
			bt.gps_pos.latitude = 16123456;
			bt.gps_pos.longitude = -16654321;
			bt.cvt_data.primary = 22;
			bt.cvt_data.secondary = i;
			bt.cvt_data.temperature = 37;
			bt.wheel_speeds.fr = i * 2;

			bt.pedal_data.gas = 1;
			bt.pedal_data.brake = 2;
			bt.pedal_data.frontPressure = 3;
			bt.pedal_data.rearPressure = 4;

			if (i % 10 == 0)
				sd |= WHEEL_SPEEDS;

			if (send_packet(sd, &bt) != RADIOLIB_ERR_NONE)
			{
				both.println("Problems!!!!");
			}

			if (sd & WHEEL_SPEEDS)
				sd ^= WHEEL_SPEEDS;
}
	}
	#else
	current_recv_data_points = 0;

	delay(100);

	Serial.println("Attempting to send packet");
	Serial.print("sd = ");
	Serial.println(current_recv_data_points);

	populate_all_data();

	if (send_packet(current_recv_data_points, &all_data) != RADIOLIB_ERR_NONE)
	{
		both.println("Problems!!!!");
	}
	#endif
}