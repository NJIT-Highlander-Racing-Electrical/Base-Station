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
  Serial.println(statusCVT_SecondaryTemp);
  

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
