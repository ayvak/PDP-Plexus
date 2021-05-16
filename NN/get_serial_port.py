import serial
import time
import subprocess

def get_port():
    dev = subprocess.run(["ls", "/dev/"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode().split()
    sys_info = subprocess.run(["uname", "-a"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode().lower()

    # Determining system
    if 'linux' in sys_info:
        # check for ttyACM* in devices
        check = 'ACM'
    elif 'darwin' in sys_info:
        # check for cu.usbmodem* in devices
        check = 'cu.usbmodem'
    else:
        print("Is this windows o.O ? We do not support it.")
        exit()

    ports = [item for item in dev if check in item]
    if len(ports) > 1:
        print("too many ports, assign port manually, exiting.")
        exit()
    elif len(ports) == 1:
        port = '/dev/' + ports[0]
        print("Teensy connected to {}".format(port))
    else:
        print("No Teensy found. Exiting.")
        exit()

    return port


if __name__ == "__main__":
    port = get_port()
    print("Testing port...")
    arduino = serial.Serial(port=port, baudrate=115200, timeout=.1)
    print("Connected to port {}".format(port))
