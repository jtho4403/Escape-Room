import serial
import threading
import subprocess
import os

# configure the serial port parameters
port = serial.Serial('/dev/cu.usbmodem113103', 115200)

# use relative file path to locate file
root_dir = os.path.realpath(os.path.join(os.path.dirname(__file__)))
game_dir = os.path.join(root_dir, 'Option 1', 'balloon_shooter.py')

# function to run the Python script
def run_python_script():
    # subprocess.run(['python', r'C:\Users\prith\Documents\Code Files\Python\balloon_shooter.py'])
    subprocess.run(['python', game_dir])

# thread for running the Python script
def script_thread():
    while True:
        # read data from the serial port
        data = port.readline().decode().strip()

        if data == 'RUN_PYTHON':
            # start a new thread to run the Python script
            threading.Thread(target=run_python_script).start()

# start the script thread
threading.Thread(target=script_thread).start()