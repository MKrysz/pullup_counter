from pullupSerial import PullupSerial
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import pandas as pd

welcomeMsg = \
    "For connecting to the device write connect\n"\
    "dayDistribution plots day distribution\n"\
    "To exit write exit\n"

def ConnectAndUpdate():
    pullupCounter = PullupSerial(debug=False)
    pullupCounter.PortList()
    portNr = int(input("Choose port (just number):\n"))
    pullupCounter.PortConnect(portNr)
    pullupCounter.RTC_Update()
    pullupCounter.DataBackupUpdate()
    pullupCounter.PortDisconnect()
    return 0

def plotDayDistribution():
    data = pd.read_table('data.txt', delimiter = ' ')
    temp = data.groupby(["Hour"]).count()["Id"]
    N = len(data)

    plotData = []
    i = 0
    for hour, val in temp.iteritems():
        while(i < hour):
            plotData.append(0)
            i+=1
        plotData.append(val/N)
        i+=1

    plt.plot(plotData)
    plt.xlabel("Hours")
    plt.ylabel("Pullups")
    plt.title("Pullups distribution during a day")
    plt.grid('on')
    plt.xticks([i for i in range(24)])
    plt.gca().yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
    plt.show()

def main():
    while(1):
        cmd = input(welcomeMsg)
        if cmd == "exit":
            break
        elif(cmd == "connect"):
            ConnectAndUpdate()
        elif(cmd == "dayDistribution"):
            plotDayDistribution()
        
    return 0

if __name__ == '__main__':
    main()