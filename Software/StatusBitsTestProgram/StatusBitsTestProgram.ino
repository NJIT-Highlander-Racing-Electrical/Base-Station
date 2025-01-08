/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

/* 

Serial Studio Connection Steps for BLE:

- Open Serial Studio
- Choose Bluetooth LE in Device Setup
- Choose "Quick Plot (Comma Separated Values" for Frame Parsing
- In the "Device" tab, select device "NJIT Base Station"
- Select Service that matches SERVICE_UUID in this program
- Select Characteristic that matches CHARACTERISTIC_UUID in this program

*/

#include "src/libraries/BajaCAN.h"  // https://arduino.github.io/arduino-cli/0.35/sketch-specification/#src-subfolder

String data;  // data string as CSV

/*
  Status flag bits for CVT

  Bit 0 (LSB): Primary IR
  Bit 1: Primary Temp
  Bit 2: Secondary IR
  Bit 3: Secondary Temp
  Bit 4: Unused
  Bit 5: Unused
  Bit 6: Unused
  Bit 7 (MSB) Unused
*/

bool statusCVT_PrimaryIR = 1;
bool statusCVT_PrimaryTemp = 1;
bool statusCVT_SecondaryIR = 1;
bool statusCVT_SecondaryTemp = 1;

/*
  Status flag bits for WheelSpeedSensors

  Bit 0 (LSB): Front Left Wheel
  Bit 1: Front Left Travel Sensor
  Bit 2: Front Right Wheel
  Bit 3: Front Right Travel Sensor
  Bit 4: Rear Left Wheel
  Bit 5: Rear Left Travel Sensor
  Bit 6: Rear Right Wheel
  Bit 7 (MSB): Rear Right Travel Sensor
*/

bool statusWheels_FrontLeftWheel = 1;
bool statusWheels_FrontLeftShock = 1;
bool statusWheels_FrontRightWheel = 1;
bool statusWheels_FrontRightShock = 1;
bool statusWheels_RearLeftWheel = 1;
bool statusWheels_RearLeftShock = 1;
bool statusWheels_RearRightWheel = 1;
bool statusWheels_RearRightShock = 1;

void setup() {
  Serial.begin(460800);


  setupCAN(BASE_STATION);
}

void loop() {


  parseStatuses();

  Serial.print(statusCVT_PrimaryIR);
  Serial.print(",");
  Serial.print(statusCVT_PrimaryTemp);
  Serial.print(",");
  Serial.print(statusCVT_SecondaryIR);
  Serial.print(",");
  Serial.print(statusCVT_SecondaryTemp);
  Serial.print(",");

  Serial.print(statusWheels_FrontLeftWheel);
  Serial.print(",");
  Serial.print(statusWheels_FrontLeftShock);
  Serial.print(",");
  Serial.print(statusWheels_FrontRightWheel);
  Serial.print(",");
  Serial.print(statusWheels_FrontRightShock);
  Serial.print(",");
  Serial.print(statusWheels_RearLeftWheel);
  Serial.print(",");
  Serial.print(statusWheels_RearLeftShock);
  Serial.print(",");
  Serial.print(statusWheels_RearRightWheel);
  Serial.print(",");
  Serial.println(statusWheels_RearRightShock);
}



void parseStatuses() {



  // CVT Status Bits

  statusCVT_PrimaryIR = bitRead(statusCVT, 0);
  statusCVT_PrimaryTemp = bitRead(statusCVT, 1);
  statusCVT_SecondaryIR = bitRead(statusCVT, 2);
  statusCVT_SecondaryTemp = bitRead(statusCVT, 3);

  // DAS Status Bits

  // Wheel Speed Status Bits

  statusWheels_FrontLeftWheel = bitRead(statusWheels, 0);
  statusWheels_FrontLeftShock = bitRead(statusWheels, 1);
  statusWheels_FrontRightWheel = bitRead(statusWheels, 2);
  statusWheels_FrontRightShock = bitRead(statusWheels, 3);
  statusWheels_RearLeftWheel = bitRead(statusWheels, 4);
  statusWheels_RearLeftShock = bitRead(statusWheels, 5);
  statusWheels_RearRightWheel = bitRead(statusWheels, 6);
  statusWheels_RearRightShock = bitRead(statusWheels, 7);


  // Pedals Status Bits
}
