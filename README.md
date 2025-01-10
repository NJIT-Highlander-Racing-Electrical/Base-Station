# Base Station

A repository dedicated to the implementation of a PC Base Station that receives vehicle data over LoRa and displays it on [Serial Studio](https://serial-studio.github.io/)
Essentially, a wireless version of the dashboard on a PC. This will also require a dedicated transmission subsystem on the vehicle.


## Features/Goals

* PC Gauges showing engine RPM and CVT secondary RPM
* Digital readouts of RPM, CVT Temp, vehicle speed, 2WD/4WD status, etc
* Live view of vehicle on map via GPS (may require offline map downloading/overlaying on image)
* 3D view of car orientation based on accelerometer

## Physical Implementation

 * This is its own independent subsystem on the car, receiving data over CAN and then transmitting it out to the PC over LoRa

 * At the PC side, we will have an enclosure at the top of the flag pole on the trailer, which contains the antenna and ESP32

 * There are a few options to power this:
    * Battery, but then we have to replace it every so often
    * 5V over USB
        * This would be ideal because we could power it with a laptop or battery bank too
        * At ~50 ft in length, the resistance may be too high to supply close enough to a full 5V, especially with a higher current draw from WiFi
    * <b><i>12V to eliminate any voltage drop over the long distance, but then we need a dedicated 12V source and step down regulator</i></b>
     * An aviation plug connector can be used at the antenna side to make the connection rigid while still being able to be disconnected.
     * We can power it with a 12VDC wall adapter

     
 * There are a few options to get data:
     * Long USB cable, but this was tested and did not work
     * Active USB cable, but we would need to find a way to enclose that hardware at the top, which can get bulky and heavy
     * RS485, but this requires translating the signal at the top and bottom
     * <b><i>Bluetooth LE. This may be less reliable than a wired serial connection, but it is easier and cheaper to set up. It also allows the laptop receiving the data to be more portable. Last, Serial Studio natively supports Bluetooth LE as a data source</i></b>

* It would be nice to still be able to switch between a wired Serial connection and BLE on the base station side, as we can hookup directly to the device when it is not on the flagpole

## Hardware

* LoRa ESP32 from Heltec
* LoRa Antennas
* Mode switch (to switch between low/high datarate, etc)
 
 ## References
 * Take some inspiration from Cornell Zigbee Data Acquisition Module
   * https://people.ece.cornell.edu/land/courses/eceprojectsland/STUDENTPROJ/2008to2009/joeneiss/MEng_Joe%20Neiss_v3.pdf
 * Can also take inspiration from this video at 0:09 of their XBee dashboard setup
    * https://www.blaisesbaker.com/projects/baja-sae/data-acquisition#h.3fc4t45tiraj
 * 3D view of car orientation could be done in Unity; take inspiration from Blaise Baker DAQ setup
     * https://www.blaisesbaker.com/projects/baja-sae/data-acquisition#h.7vc1ioheo1co
