from pullupSerial import PullupSerial
from datetime import datetime, timedelta
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import pandas as pd
import numpy as np
from copy import deepcopy
import calplot
import calmap

welcomeMsg = \
    "For connecting to the device write connect\n"\
    "dayDistribution plots day distribution\n"\
    "weekDistribution plots week distribution\n"\
    "calendar for callendar heatmap\n"\
    "To exit write exit\n"

def ConnectAndUpdate():
    pullupCounter = PullupSerial(debug=True)
    pullupCounter.PortList()
    portNr = int(input("Choose port (just number):\n"))
    pullupCounter.PortConnect(portNr)
    pullupCounter.RTC_Update()
    pullupCounter.DataBackupUpdate()
    pullupCounter.PortDisconnect()
    return 0

def plotDayDistribution(data):
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


def plotCalendarHeatmap(data):
    data = deepcopy(data)
    data = pd.Series(data.Count.values, data['datetime'])
    fig, ax = calmap.calendarplot(data)                                                                                                                                   
    fig.colorbar(ax[0].get_children()[1], ax=ax.ravel().tolist())  
    fig.suptitle('Number of pull-ups per day heatmap', fontsize=16)
    plt.show()

def plotWeekDistribution(data):
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

def readData():
    data = pd.read_table('data.txt', delimiter = ' ')
    data.set_index('Id', inplace = True)
    data['Year'] = data['Year'] + 2000
    data['datetime'] = pd.to_datetime(data[["Year", "Month", "Day", "Hour", "Minute"]])
    data = data.drop(['Year', 'Month', 'Day', 'Hour', 'Minute'], axis=1)
    return data


def generalDataAnalysis(data, nrOfDays):
    startDate = datetime.now() - timedelta(days=nrOfDays)
    data = data.drop(data[(data.datetime < startDate)].index)
    SumOfPullups = data.Count.sum()
    print('Data from the last', nrOfDays, 'days')
    print('Pull-ups done :', SumOfPullups)
    print('Average per day : {:.2f}'.format(SumOfPullups/nrOfDays))
    print()

def analyze():
    data = readData()
    generalDataAnalysis(data, 7)
    generalDataAnalysis(data, 30)
    generalDataAnalysis(data, 365)
    plotDayDistribution(data)
    plotCalendarHeatmap(data)

def interface():
    while(1):
        cmd = input(welcomeMsg)
        if cmd == "exit":
            break
        elif(cmd in ("connect", 'con')):
            ConnectAndUpdate()
        elif(cmd in ("dayDistribution", 'day')):
            plotDayDistribution(0)
        elif(cmd in ("weekDistribution", 'week')):
            plotWeekDistribution(0)
        elif(cmd in ("calendar", 'cal')):
            plotCalendarHeatmap(0)
        elif(cmd in ('analyze', 'a')):
            analyze()
        else:
            print("Unknown command")
    return 0

def test():
    return 0

def main():
    interface()
    # test()

if __name__ == '__main__':
    main()