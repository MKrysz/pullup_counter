from pullupSerial import PullupSerial

def ConnectAndUpdate():
    pullupCounter = PullupSerial(debug=False)
    pullupCounter.PortList()
    portNr = int(input("Choose port (just number):\n"))
    pullupCounter.PortConnect(portNr)
    pullupCounter.RTC_Update()
    pullupCounter.DataBackupUpdate()
    pullupCounter.PortDisconnect()
    return 0

def main():
    return 0

if __name__ == '__main__':
    main()