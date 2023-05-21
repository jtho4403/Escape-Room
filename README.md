# MTRX2700 Major Project - Escape Room
## Group members, roles & responsibilities
David Boian - PTU sensing module    
Imran Tauqeer - PTU actuating module    
Jack Thompson - LED & timer module      
Jia Xu Chee - sequence module, minutes     
Jiapeng Liu - LIDAR module    
Prithik Saini - PTU actuating module   

## Overview
![image](https://github.com/jtho4403/Escape-Room/blob/main/image/flowchart.drawio.png)   
The code contains 3 self-containing stages that works separately. There's one stages each before and after the 3 stages that require coding.

## Stage 1 - LIDAR
Done by Jiapeng Liu.    
### High level explanation

### Testing


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
