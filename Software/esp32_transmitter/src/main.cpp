#include <heltec_unofficial.h>
#include <baja_data_compression.h>

#include <BajaCAN_forLORA.h>

#define FREQUENCY 905.2
// #define BANDWIDTH 125
// #define SPREADING_FACTOR 9

#define BANDWIDTH 500
#define SPREADING_FACTOR 7

// max 22
#define TRANSMIT_POWER 22

//#define RADIO_TEST

union crc_value
{
	uint16_t intVal;
	char strVal[2];
};

void setup()
{

	Serial.begin(115200);
	setupCAN();

	RadioLibCRCInstance.size = 16;
	RadioLibCRCInstance.poly = 0x755B;
	RadioLibCRCInstance.init = 0xFFFF;
	RadioLibCRCInstance.out = 0x0000;

	heltec_setup();
	heltec_ve(true);

	both.println("Radio init");
	// radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR);
	radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, 7, 0x13, 0, 8);
	if (radio.setCRC(0) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION)
	{
		both.println("bad bad bad");
	}
	radio.setOutputPower(TRANSMIT_POWER);

#ifndef RADIO_TEST
	setupCAN();
#endif
}

int a = 0;

int16_t send_packet(sent_data_points_t sd, const baja_data_t *bt)
{
	char buf[100];
	memset(buf, 0, 100);

	// take the new data points and their values and pack them into the byte array we will transmit
	size_t byteCount = pack_data(buf, bt, sd);

	// compute a checksum and put it in our packet because radiolib can't seem to
	// put it in the packet on its own
	union crc_value checksum;
	checksum.intVal = RadioLibCRCInstance.checksum((const uint8_t *)buf, byteCount);

	char *fullStr = (char *)malloc(byteCount + 2); // 2 extra bytes for checksum
	memcpy((void *)fullStr, (const void *)buf, byteCount);
	fullStr[byteCount] = checksum.strVal[0];
	fullStr[byteCount + 1] = checksum.strVal[1];

	int16_t status = radio.transmit((const uint8_t *)fullStr, byteCount + 2, 0);

	free(fullStr);

	return status;
}

void loop()
{
	heltec_loop();

#ifdef RADIO_TEST
	if (button.isSingleClick())
	{
		both.printf("Button %i\n", ++a);	

		// send some fake data a couple hundred times

		baja_data_t bt;
		sent_data_points_t sd = 0;
		sd |= CVT_DATA;

		char buf[100];
		for (int i = 0; i < 500; ++i)
		{
			bt.cvt_data.primary = 22;
			bt.cvt_data.secondary = i;
			bt.cvt_data.temperature = 37;
			bt.wheel_speeds.fr = i * 2;
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

	// can task will populate this as we receive data points
	current_recv_data_points = 0;

	char buf[100];

	Serial.println("Attempting to send packet");

	// all_data is in BajaCAN_forLORA.h
	if (send_packet(current_recv_data_points, &all_data) != RADIOLIB_ERR_NONE)
	{
		both.println("Problems!!!!");
	}

#endif
}
