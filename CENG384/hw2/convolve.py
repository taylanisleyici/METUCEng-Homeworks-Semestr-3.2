import matplotlib.pyplot as plt

def conv(s1, si1, s2, si2):
    plotx = []
    ploty = []

    for i in range(len(s1) + len(s2) - 1):
        plotx.append(si1 + si2 + i)
        count = 0
        for j in range(len(s1)):
            if(plotx[-1] - (si1 + j) < si2):
                continue
            elif(plotx[-1] - (si1 + j) > len(s2) + si2 - 1):
                continue
            else:
                count += float(s1[j]) * float(s2[plotx[-1] - (si1 + si2 + j)])
        ploty.append(count)
    plt.stem(plotx, ploty)
    plt.show()

def nPoint(n):
    if n <= 0:
        return [0]
    res = []
    for i in range(n):
        res.append(1 / float(n))
    return res

def main():
    filePath = input()
    fileCSV = open(filePath)
    csvList = [i for i in fileCSV.read().split(",")]
    startingIndex = int(csvList[0])
    conv([1],5,csvList[1:], int(csvList[0]))
    conv(nPoint(3), 0, csvList[1:], int(csvList[0]))
    conv(nPoint(5), 0, csvList[1:], int(csvList[0]))
    conv(nPoint(10), 0, csvList[1:], int(csvList[0]))
    conv(nPoint(20), 0, csvList[1:], int(csvList[0]))
    



if __name__ == "__main__":
    main()