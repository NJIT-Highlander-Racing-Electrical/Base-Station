#include <heltec_unofficial.h>

#include <stddef.h>
#include <stdint.h>
#include "util.h"
#include "csv.h"

#define FREQUENCY 905.2
//#define BANDWIDTH 125
//#define SPREADING_FACTOR 9

#define BANDWIDTH 500
#define SPREADING_FACTOR 7

// max 22
#define TRANSMIT_POWER 0

#include <WiFi.h>

bool rxFlag = false;
bool overflow = false;
void rx_callback() {
	if(rxFlag) overflow = true;
	rxFlag = true;
}

bool usingWifi = false;

WiFiServer server(23); // serial studio defaults to port 23 (telnet port) 

QueueHandle_t queue_test;

void send_stuff(WiFiClient *client) {
	const char *thing = "HTTP/1.1 200 OK\nContent-type: text/html\nConnection: close\n\nI am some very useful data\n";
	client->println(thing);

}

#define MAX_CLIENTS 8
void loop_wifi(void*) {
	WiFiClient *clients[MAX_CLIENTS] = { NULL };
	bool newData = false;
	rxdata_t currentData;
	for(;;) {

		WiFiClient newClient = server.available();
		if(newClient) {
			bool foundSpot = false;
			for(int i = 0; i < MAX_CLIENTS; ++i) {
				if(clients[i] == NULL) { // found an empty spot
					clients[i] = new WiFiClient(newClient); // copy the new client into the array
					foundSpot = true;
					break;
				}
			}
			if(!foundSpot) {
				// TODO: too many clients error
			}
		}

		// free old data before receiving new
		if(newData && currentData.data) free(currentData.data);
		newData = false;
		if(xQueueReceive(queue_test, &currentData, 0) == pdPASS) {
			newData = true;
		} else {
			// TODO: rxdata was null?
		}

		if(uxQueueMessagesWaiting(queue_test) > 2) {
			both.println("Falling behind, message skipping");
			xQueueReset(queue_test);
		}

		if(newData) {

			char buf[200];
			make_csv(buf, 200, currentData);

			for(int i = 0; i < MAX_CLIENTS; ++i) {
				WiFiClient *client = clients[i];
				if(client) {
					if(!client->connected()) {
						delete clients[i]; // we made a copy before, delete it
						clients[i] = NULL; // remove disconnected client from array
						continue;
					}
					//client->println(currentData.data);
					client->println(buf);
					//send_stuff(client);
				}
			}
		}

	}
}

void setup() {

	RadioLibCRCInstance.size = 16;
	RadioLibCRCInstance.poly = 0x755B;
	RadioLibCRCInstance.init = 0xFFFF;
	RadioLibCRCInstance.out = 0x0000;

	heltec_setup();
	heltec_ve(true);

	both.print("Radio init... ");
	radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, 7, 0x13, 0, 8);
	if(radio.setCRC(0) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
		both.println("bad bad bad");
	}
	//radio.beginFSK(434.0, 2.4, 4.5, 156.2, 0, 512);
	radio.setOutputPower(TRANSMIT_POWER);

	radio.setDio1Action(rx_callback);
	radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF);
	both.println("success.");
	both.print("Wifi init... ");

	TaskHandle_t wifiHandle = NULL;
	if(WiFi.softAP("NJIT Highlander Racing", "password123")) {
		both.println("success.");
		IPAddress ip = WiFi.softAPIP();
		both.print("IP: ");
		both.println(ip);
		server.begin();
		usingWifi = true;
		xTaskCreate(
				loop_wifi,
				"wifi loop",
				10000,
				NULL,
				0,

				&wifiHandle
			   );
	} else {
		both.println("failure.");
	}



	queue_test = xQueueCreate(16, sizeof(rxdata_t));
	if(!queue_test) { both.println("QUEUE ERROR"); return; }

}

void loop() {
	heltec_loop();

	if(overflow) {
		both.println("o");
	}

	if(rxFlag) {
		size_t packetLen = radio.getPacketLength();

		// this will contain the useful data and the crc
		char *allrxdata = (char*)malloc(packetLen);
		if(!allrxdata) {// TODO: malloc failed	
		}

		radio.readData((uint8_t*)allrxdata, packetLen);

		// load the received checksum into our union to check it
		union crc_value checksum;
		checksum.strVal[0] = allrxdata[packetLen - 2];
		checksum.strVal[1] = allrxdata[packetLen - 1];

		bool passed = checksum.intVal == (uint16_t)RadioLibCRCInstance.checksum((const uint8_t*)allrxdata, packetLen - 2);

		if(!passed) {
			/*rxdata_t rxdata_struct;
			const char *errmsg = "  Packet corrupted";
			char *errmsg_copy = (char*)malloc(19);
			if(!errmsg_copy) return;
			strncpy(errmsg_copy, errmsg, 18);
			errmsg_copy[18] = 0;
			errmsg_copy += 2; // very hacky to get it working
			rxdata_struct.length = 17;
			rxdata_struct.data = errmsg_copy;
			xQueueSendToBack(queue_test, &rxdata_struct, 0);*/
		} else if(_radiolib_status == RADIOLIB_ERR_NONE) {
			//both.printf("SNR: %.2f\n", radio.getSNR());
			//both.printf("%s\n", rxdata);

			size_t dataLen = packetLen - 2;

			rxdata_t rxdata_struct;
			rxdata_struct.length = dataLen;

			// this contains just the useful data (the crc at the end is gone)
			char *rxdata = (char*)realloc((void*)allrxdata, dataLen);
			if(rxdata == NULL) { // if this happens we're all screwed
				both.println("bad bad bad");
			}

			rxdata_struct.data = rxdata; // the queue receiver is responsible for freeing the rxdata_struct.data
						// if the queue overflows then oops memory leak TODO fix!!!!!
			xQueueSendToBack(queue_test, &rxdata_struct, 0);
		}
		radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF);
		rxFlag = false;
	}

}


