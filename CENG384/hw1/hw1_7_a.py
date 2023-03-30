import matplotlib.pyplot as plt

def main():
    filePath = input()
    fileCSV = open(filePath)
    csvList = [i for i in fileCSV.read().split(",")]
    startingIndex = int(csvList[0])
    csvList = [float(i) for i in csvList[1:]]
    even_results = dict()
    odd_results = dict()
    
    for iindex, i in enumerate(csvList):
        if((iindex + startingIndex) not in even_results.keys()):
            even_results[iindex + startingIndex] = i
        else:
            even_results[iindex + startingIndex] += i
        
        if(-(iindex + startingIndex) not in even_results.keys()):
            even_results[-(iindex + startingIndex)] = i
        else:
            even_results[-(iindex + startingIndex)] += i
        
        if((iindex + startingIndex) not in odd_results.keys()):
            odd_results[iindex + startingIndex] = i
        else:
            odd_results[iindex + startingIndex] += i
        
        if(-(iindex + startingIndex) not in odd_results.keys()):
            odd_results[-(iindex + startingIndex)] = -i
        else:
            odd_results[-(iindex + startingIndex)] -= i
        
    xVals_even = []
    yVals_even = []
    xVals_odd = []
    yVals_odd = []

    for i in even_results.items():
        xVals_even.append(i[0])
        yVals_even.append(i[1])
    
    for i in odd_results.items():
        xVals_odd.append(i[0])
        yVals_odd.append(i[1])
    

    plt.stem(xVals_even, yVals_even)
    plt.show()

    plt.stem(xVals_odd, yVals_odd)
    plt.show()

if __name__ == "__main__":
    main()