<!--- File name should be dd mm meeting --->
# Week 11 Lab minute

**Date and time:** 10/05/23 14:00

**Participants:** David Boian,  Jack Thompson, Jia Xu Chee, Jiapeng Liu     
**Absentees:** Imran Tauqeer (taking care of sick family member), Prithik Saini (injured)

## Agenda
Continue working on last week's assigned parts.

## Progress update
**Lidar sensing module:** able to receive data from LIDAR, adding pass condition

**LED and timer module:** trying to make sequence randomiser

**Serial module:** added serial interrupt, creating LED sequence stage framework

**PTU actuating module:** 
Absent. Worked out how to use serial to control PTU.

**PTU sensing module:**
Worked out how the PTU gyro and accelerometer sensor functions, trying to work out another serial output interrupt.

## Changes made to initial plan
* PTU actuating module may use serial as input instead of joystick
* If LED sequence stage fails, it starts over at the same stage instead of to the very beginning
