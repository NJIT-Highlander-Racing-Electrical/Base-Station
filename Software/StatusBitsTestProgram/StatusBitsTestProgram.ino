/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

/* 

Serial Studio Connection Steps:

- Open Serial Studio
- Choose Bluetooth LE in Device Setup
- Choose "Quick Plot (Comma Separated Values" for Frame Parsing
- In the "Device" tab, select device "NJIT Base Station"
- Select Service that matches SERVICE_UUID in this program
- Select Characteristic that matches CHARACTERISTIC_UUID in this program

*/

#include "src/libraries/BajaCAN.h"  // https://arduino.github.io/arduino-cli/0.35/sketch-specification/#src-subfolder

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

String data;  // data string as CSV

// CVT Status Bits

/*
    Status flag bits for CVT

    Bit 7 (MSB): Unused
    Bit 6: Unused
    Bit 5: Unused
    Bit 4: Unused
    Bit 3: Secondary Temp
    Bit 2: Secondary IR
    Bit 1: Primary Temp
    Bit 0 (LSB): Primary IR
  */
bool statusCVT_PrimaryIR = 1;
bool statusCVT_PrimaryTemp = 1;
bool statusCVT_SecondaryIR = 1;
bool statusCVT_SecondaryTemp = 1;


BLECharacteristic *pCharacteristic;

void setup() {
  Serial.begin(460800);
  Serial.println("Starting BLE work!");

  setupCAN(BASE_STATION);

  // Initializes BLE device
  BLEDevice::init("NJIT Base Station ");  // Creates a BLE device with name passed in
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Creates a characteristic. A characteristic is a part of a service
  // that represents a piece of information/data that a server wants to
  // expxose to a client. For example, a battery level characteristic
  // would represent the remaining level of a battery in a BLE device
  // that can be read by a client. In our case, this client is
  // Serial Studio
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  // Set initial value
  pCharacteristic->setValue("This is the initial value of our characteristic");

  // Start the service. A service encapsulates zero or more
  // functionally-related characteristics.
  pService->start();

  // Set advertising parameters. Advertising is where a BLE device sends out packets
  // of data for others to receive and process.
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);  // A scan response is the packet sent by the advertising device (our ESP32) upon reception of scanning reqquests
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();  // Begin advertising
}

void loop() {

  parseStatuses();

  Serial.print("StatusCVT (main): ");
  Serial.println(statusCVT);


  formatCharacteristic();

  //pCharacteristic->setValue(data);
  pCharacteristic->setValue(data.c_str());


  delay(2000);
}



void parseStatuses() {



  // CVT Status Bits

  statusCVT_PrimaryIR = bitRead(statusCVT, 0);
  statusCVT_PrimaryTemp = bitRead(statusCVT, 1);
  statusCVT_SecondaryIR = bitRead(statusCVT, 2);
  statusCVT_SecondaryTemp = bitRead(statusCVT, 3);

  // DAS Status Bits

  // Wheel Speed Status Bits

  // Pedals Status Bits
}


void formatCharacteristic() {

  data = "";  // clear the string

  data += (statusCVT_PrimaryIR ? 1 : 0) + ",";
  data += (statusCVT_PrimaryTemp ? 1 : 0) + ",";
  data += (statusCVT_SecondaryIR ? 1 : 0) + ",";
  data += (statusCVT_SecondaryTemp ? 1 : 0);
}