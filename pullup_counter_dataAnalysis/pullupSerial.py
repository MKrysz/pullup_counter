import serial
import serial.tools.list_ports
import time
import os
from datetime import datetime

class PullupSerial():

    def __init__(self, debug = True) -> None:
        self.ser = serial.Serial()
        self.debug = debug

    def PortList(self):
        ports = serial.tools.list_ports.comports()

        for port, desc, hwid in sorted(ports):
                print("{}".format(port))


    def PortConnect(self, portNr):
        if self.ser.is_open:
            print("Already connected")
            return False
        self.ser.baudrate = 115200
        self.ser.port = "COM" + str(portNr)
        self.ser.open()
        print("Restart the device\n")
        # wait for device to reboot
        time.sleep(1)
        # read welcoming message
        for i in range(4):
            buffer = self.ser.readline()
            if self.debug:
                print(buffer)
        
        print("Device connected")
        return True
    
    def PortDisconnect(self):
        self.ser.close()

    def SendCmd(self, cmd):
        self.ser.write(bytes(cmd, 'utf-8'))
        if self.debug:
            print("CMD sended:\n", cmd)
        time.sleep(1)

    def checkOk(self):
        if self.ser.readline() != "OK":
            return True
        return False
    
    def DataBackupUpdate(self):
        with open("data.txt", 'r') as dataFile:
            lastLine = dataFile.readlines()[-1]
        with open('data.txt', 'a') as dataFile:
            lastEntry = PullupEntry(lastLine)
            self.SendCmd("flash readSince "+ str(lastEntry.id+1))
            # get format line out of the buffer
            self.ser.readline()
            entriesSaved = 0
            while(1):
                line = self.ser.readline()
                if line == b"OK\n":
                    break
                dataFile.write(str(line, 'utf-8'))
                entriesSaved += 1
            print("saved", entriesSaved, "entries")
    
    def DataBackupCreate(self):
        if os.path.exists('data.txt'):
            print("DataBackup already exists")
            return
        with open('data.txt', 'x') as dataFile:
            self.SendCmd("flash readAll")
            entriesSaved = -1
            while(1):
                line = self.ser.readline()
                if line == b"OK\n":
                    break
                dataFile.write(str(line, 'utf-8'))
                entriesSaved += 1
            print("DataBackup created with", entriesSaved, "entries")
    
    def readline(self):
        return str(self.ser.readline(), 'utf-8')
    
    def RTC_Read(self):
        self.SendCmd("rtc read")
        result = self.readline()
        result += self.readline()
        result += self.readline()
        self.checkOk()
        return result
    
    def RTC_Update(self):
        print("RTC time before update = \n", self.RTC_Read())
        now = datetime.now()
        timeStr = now.strftime("%H %M")
        self.SendCmd("rtc time " + timeStr)
        self.checkOk()
        dateStr = now.strftime("%d %m")
        self.SendCmd("rtc date " + dateStr)
        self.checkOk()
        yearStr = now.strftime("%y")
        self.SendCmd("rtc year " + yearStr)
        self.checkOk()
        weekdayNr = datetime.today().weekday() + 1
        self.SendCmd("rtc weekday " + str(weekdayNr))
        self.checkOk()
        print("RTC time after update = \n", self.RTC_Read())
        


class PullupEntry():

    def __init__(self, str):
        strArgs = str.split(' ')
        args = []
        for strArg in strArgs:
            args.append(int(strArg))
        
        self.id = args[0]
        self.hour = args[1]
        self.min = args[2]
        self.month = args[3]
        self.date = args[4]
        self.weekday = args[5]
        self.year = args[6]

    def toRaw(self):
        result = ''
        result += str(self.id)
        result += ' '
        result += str(self.hour)
        result += ' '
        result += str(self.min)
        result += ' '
        result += str(self.month)
        result += ' '
        result += str(self.date)
        result += ' '
        result += str(self.weekday)
        result += ' '
        result += str(self.year)
        return result

# for testing the module
def main():
    pullupCounter = PullupSerial()
    # pullupCounter.PortList()
    # portNr = int(input("Choose port:\n"))
    portNr = 6
    pullupCounter.PortConnect(portNr)
    pullupCounter.RTC_Update()
    # pullupCounter.DataBackupCreate()

    pullupCounter.PortDisconnect()
    return

if __name__ == "__main__":
    main()

    
