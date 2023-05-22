# MTRX2700 Major Project - Escape Room
## Group members, roles & responsibilities
David Boian - PTU sensing module    
Imran Tauqeer - PTU actuating module    
Jack Thompson - LED & timer module      
Jia Xu Chee - sequence module, minutes     
Jiapeng Liu - LIDAR module    
Prithik Saini - PTU actuating module, python game module   

## Overview
![image](https://github.com/jtho4403/Escape-Room/blob/main/image/flowchart.drawio.png)   
The code contains 3 self-containing stages that works separately. There's one stages each before and after the 3 stages that require coding.

## Stage 1 - LIDAR
Done by Jiapeng Liu. 

The work of detecting distance with LIDAR mainly focuses on two aspects:
- Optimization of detection distance algorithm
- Visualization of detection distance

1. Optimization of detection distance algorithm
LiDAR may make errors when detecting distance due to external environmental interference or target angle interference. 1. Data processing and filtering: Consider using multiple continuous measurement data for judgment and decision-making, rather than just a single data point. By statistically averaging multiple data, the accuracy of distance measurement can be improved. 2. Threshold setting: Adjust the threshold according to the detection distance requirements. Due to the excessive sensitivity of the LiDAR detector, the distance data will bounce back and forth in two areas, such as the too-far area and the detected target area. By adding a buffer space to solve the problem, leave around 5 buffer spaces at the threshold setting for each region. For example, the threshold for the target area is distance < 50, and the threshold for areas too far away is set to distance > 55.

2. Visualization of detection distance
Arduino connects a 1.3-inch OLED screen to display the detection distance type (ok, too far, too close). The communication between STM32F3 Discover Board and Arduino uses a logic level detection method.

![image](https://github.com/jtho4403/Escape-Room/blob/main/image/LIDAR%20Circuit%20connection%20diagram.png) 
### High level explanation
![image](https://github.com/jtho4403/Escape-Room/blob/main/image/Logic%20diagram%20of%20laser%20detection%20LIDAR.png)  
### Testing
Connect all necessary equipment according to requirements and continuously change the distance between obstacles and LiDAR equipment for testing. When the distance is greater than 55, the OLED display screen displays "Too far". When the distance is less than 25, the OLED display screen displays "Too far". When the distance is within the range of 30 and 50, the OLED display screen displays "Ok"


## Stage 2 - Repeat after me
Done by Jia Xu Chee and Jack Thompson.    
### High level explanation
![image](https://github.com/jtho4403/Escape-Room/blob/main/image/Stage2.drawio.png)   
### Testing
Modular testing can be done in serial_module folder. Since this stage can be done solely in STM, the important parts of the code are in serial.c and sequence.c. To test the module, call the function Stage2() in main.c. The expected output of the stage is as the flowchart shown above, where the terminal will output "pass" when the player inputs correct sequence; "fail" for incorrect input; "Restarting stage 2" when restarting the stage; and "You've passed Stage 2" when the plater inputs 4 consecutive correct sequence.

## Stage 3 - Find the code
Done by David Boian, Imran Tauqeer and Prithik Saini.    
### High level explanation

### Testing

## Stage 4 - Pop the ballon
Done by Prithik Saini.    
### High level explanation

### Testing
