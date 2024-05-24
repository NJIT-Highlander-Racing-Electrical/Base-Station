# Base Station

A repository dedicated to the implementation of a PC Base Station that receives vehicle data over RF and displays it on a PC Program
Essentially, a wireless version of the dashboard on a PC

## Features/Goals

* PC Gauges showing engine RPM and CVT secondary RPM
* Digital readouts of RPM, CVT Temp, vehicle speed, 2WD/4WD status, etc
* Live view of vehicle on map via GPS (may require offline map downloading/overlaying on image)
* 3D view of car orientation based on accelerometer

## Hardware

* XBee RR Pro modules to transmit data between car and PC
  * RR Pro's advertise a maximum range of 2 miles outdoors line-of-sight
* RP-SMA Antenna with mounting bracket to attach antenna to rear of firewall
* RP-SMA Antenna with magnetic base to attach to trailer, pole, etc on PC base station side
* XBee Explorer USB Module to transfer data from RR Pro to PC over Serial

## Software

* May be able to use Serial Studio for displaying serial data as GUI 
  * https://serial-studio.github.io/
 
 ## References
 * Take some inspiration from Cornell Zigbee Data Acquisition Module
   * https://people.ece.cornell.edu/land/courses/eceprojectsland/STUDENTPROJ/2008to2009/joeneiss/MEng_Joe%20Neiss_v3.pdf  
