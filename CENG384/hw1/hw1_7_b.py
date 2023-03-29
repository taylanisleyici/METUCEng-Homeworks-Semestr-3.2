import matplotlib.pyplot as plt


def main():
    filePath = input()
    fileCSV = open(filePath)
    csvList = [i for i in fileCSV.read().split(",")]
    startingIndex = int(csvList[0])
    a = int(csvList[1])
    b = int(csvList[2])
    csvList = [float(i) for i in csvList[3:]]
    xVals = []
    yVals = []
    """The following three lines are left for debugging purposes."""
    # for iindex, i in enumerate(csvList):
    #     xVals.append(iindex + startingIndex)
    #     yVals.append(i)
    for iindex, i in enumerate(csvList):
        if (iindex + startingIndex - b)%a == 0:
            xVals.append((iindex + startingIndex - b)//a)
            yVals.append(i)

    plt.stem(xVals, yVals)
    plt.show()


if __name__ == "__main__":
    main()
