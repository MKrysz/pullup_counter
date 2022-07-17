import serial 
import time

with open('bckp.txt', 'r') as bckp:
    db = bckp.readlines()
    for i, line in enumerate(db):
        db[i] = (line.replace('\t', ' '))[:-1] + " 22\n"
    db = db[1:]
    # for line in db:
    #     print(line)

with serial.Serial('COM3', 115200) as ser:
    input("Connect/reconnect the device\n")
    time.sleep(1)
    print(ser.read_all())
    ser.write(b"flash clear")
    time.sleep(1)
    print(ser.read_all())
    ser.write(b"flash raw")
    time.sleep(1)
    before = ser.read_all()
    ser.write(b"flash write")
    time.sleep(1)
    ser.readline()
    for line in db:
        ser.write(bytes(line, 'utf-8'))
        # time.sleep(1)
        print("writing: ", line)
        print(ser.readline())
