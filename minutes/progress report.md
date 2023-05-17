# Week 12 Progress Report
*see 06 Apr minutes for flowchart*

## LIDAR sensing module
completed functions: Visualization of LiDAR detection distance.

in progress functions: Optimization algorithm for detection distance

## LED and timer module
completed functions: LED sequence display, timer module partially complete

in progress functions: timer module, modularisation of both functions

## serial module
completed functions: serial output, receiving sequence with interrupt, checking sequence, stage 2 framework 

in progress functions: modularising serial input

## PTU actuating module
completed functions: arduino joystick module

in progress functions: taking joystick from arduino into stm32. using joystick input to control the PTU

## PTU sensing module
- got transmission via interrupts to work to get data out
- got magnotemter reading via I2C line 2
- got accleromemeter to work via I2C

completed functions: PTU raw data readings

in progress functions: calibration and covnersion to x y coordinates using data
