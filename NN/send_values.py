import time
from pySerialTransfer import pySerialTransfer as txfer
from get_serial_port import get_port

BATCH_SIZE = 28
DEBUG = False

link = None

# def initialize_connection():

# get serial port
port = get_port()

try:
    link = txfer.SerialTransfer(port)
    
    link.open()
    time.sleep(2) # allow some time for the Arduino to completely reset
    
except KeyboardInterrupt:
    try:
        link.close()
    except:
        pass

except:
    import traceback
    traceback.print_exc()
    
    try:
        link.close()
    except:
        pass


def send_values(_input_layer, _hidden_layer, _output_layer):
    if DEBUG:
        print("input {}".format((_input_layer)))
        print("hidden {}".format((_hidden_layer)))
        print("output {}".format((_output_layer)))
    # while True:
    # send_size = 0

    # send input indicator
    # send_size = 0
    str_size = link.tx_obj("ii")
    link.send(str_size)
    time.sleep(0.001)

    # send input layer, batch wise
    # count = 0
    # l = [i for i in range(784)]
    l = _input_layer
    print("sending input layer ===========================")
    while len(l) > 0:
        send_size = 0
        # if DEBUG: print("sending batch {}".format(count+1))
        str_ = '-'.join([str(i) for i in l[:BATCH_SIZE]]) + '-'
        if DEBUG: print(str_)
        str_size = link.tx_obj(str_)#, send_size) - send_size
        send_size += str_size
        link.send(str_size)
        l = l[BATCH_SIZE:]
        # if DEBUG: print("remaining bytes {}".format(len(l)))
        # count += 1
        # time.sleep(0.0001)

    # send hidden indicator
    # send_size = 0
    str_size = link.tx_obj("hh")
    link.send(str_size)
    time.sleep(0.001)

    # send hidden layer, batch wise
    # count = 0
    # l = [i for i in range(1000,1400)]
    l = _hidden_layer
    print("sending hidden layer ===========================")
    while len(l) > 0:
        # send_size = 0
        # if DEBUG: print("sending batch {}".format(count+1))
        str_ = '-'.join([str(i) for i in l[:BATCH_SIZE]]) + '-'
        if DEBUG: print(str_)
        str_size = link.tx_obj(str_)#, send_size) - send_size
        # send_size += str_size
        link.send(str_size)
        l = l[BATCH_SIZE:]
        # if DEBUG: print("remaining bytes {}".format(len(l)))
        # count += 1
        # time.sleep(0.0001)

    # send output indicator
    # send_size = 0
    str_size = link.tx_obj("oo")
    link.send(str_size)
    time.sleep(0.001)

    # send output layer, batch wise
    # count = 0
    # l = [i for i in range(2000,2784)]
    l = _output_layer
    print("sending output layer ===========================")
    while len(l) > 0:
        # send_size = 0
        # if DEBUG: print("sending batch {}".format(count+1))
        str_ = '-'.join([str(i) for i in l[:BATCH_SIZE]]) + '-'
        if DEBUG: print(str_)
        str_size = link.tx_obj(str_)#, send_size) - send_size
        # send_size += str_size
        link.send(str_size)
        l = l[BATCH_SIZE:]
        # if DEBUG: print("remaining bytes {}".format(len(l)))
        # count += 1
        # time.sleep(0.0001)