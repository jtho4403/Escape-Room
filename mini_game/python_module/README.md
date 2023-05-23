## IMPORTANT INFORMATION

Python libraries are essential for the working of this module. Install the essential libraries by running 'pip install pyserial' and 'pip install pygame' via terminal. 
Make sure to replace 'COM6' in the Python script (line 7 in script.py):
```c
# configure the serial port parameters
port = serial.Serial('COM6', 115200)
```
with the appropriate port name that corresponds to the serial connection between the microcontroller and your computer (can be viwed in Device Manager).
The python script uses relative paths to run the game, therefore the location doesn't matter as long as current relativity is maintained.
