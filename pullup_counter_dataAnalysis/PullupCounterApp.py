from pullupSerial import PullupSerial
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import pandas as pd
import numpy as np

welcomeMsg = \
    "For connecting to the device write connect\n"\
    "dayDistribution plots day distribution\n"\
    "weekDistribution plots week distribution\n"\
    "calendar for callendar heatmap\n"\
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
    hourCounter = 0
    for hour, val in temp.iteritems():
        while(hourCounter < hour):
            plotData.append(0)
            hourCounter+=1
        plotData.append(val/N)
        hourCounter+=1

    plt.plot([i+0.5 for i in range(24)], plotData)
    plt.xlabel("Hours")
    plt.ylabel("Pullups")
    plt.title("Pullups distribution during a day")
    plt.grid('on')
    plt.xticks([i for i in range(24)])
    plt.gca().yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
    plt.show()


def plotCalendarHeatmap():
    data = pd.read_table('data.txt', delimiter = ' ')
    temp = data.groupby(["Month", "Date"]).count()["Id"]
    print(temp)

def plotWeekDistribution():
    data = pd.read_table('data.txt', delimiter = ' ')
    temp = data.groupby(["Weekday"]).count()["Id"]
    N = len(data)

    plotData = []
    weekdayCounter = 0
    for weekday, val in temp.iteritems():
        weekday -= 1
        while(weekdayCounter < weekday):
            plotData.append(0)
            weekdayCounter+=1
        plotData.append(val/N)
        weekdayCounter+=1

    text_values = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
    plt.bar([i for i in range(7)], plotData, align='center')
    plt.xlabel("Weekday")
    plt.ylabel("Pullups")
    plt.xticks([i for i in range(7)], text_values)
    plt.gca().yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1.0))
    plt.title("Pullups distribution during a week")
    plt.grid('on')

    plt.show()

def interface():
    while(1):
        cmd = input(welcomeMsg)
        if cmd == "exit":
            break
        elif(cmd == "connect"):
            ConnectAndUpdate()
        elif(cmd == "dayDistribution"):
            plotDayDistribution()
        elif(cmd == "weekDistribution"):
            plotWeekDistribution()
        elif(cmd == "calendar"):
            plotCalendarHeatmap()
        else:
            print("Unknown command")
    return 0

def test():
    # plotDayDistribution()
    plotWeekDistribution()

def main():
    # interface()
    test()

if __name__ == '__main__':
    main()