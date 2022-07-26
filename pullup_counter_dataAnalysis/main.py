import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

data = pd.read_table('data.txt', delimiter = '\t')

def plotDayDistribution(data):
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

def plotCalendar(data):
    temp = data.groupby(["Month", "Date"]).count()["Id"]
    print(temp)


plotDayDistribution(data)
plotCalendar(data)

