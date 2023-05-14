import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as sp

def fourierCoefficients(signal, period, count):
    coefficients = []
    coefficients.append(np.mean(signal))

    for k in range(1, count + 1):
        value = 0
        for n in range(len(signal)):
            value += (signal[n] * np.cos(2 * np.pi * k * (n * period / len(signal)) / period))
        coefficients.append(2 / len(signal) * value)

    for k in range(1, count + 1):
        value = 0
        for n in range(len(signal)):
            value += (signal[n] * np.sin(2 * np.pi * k * (n * period / len(signal)) / period))
        coefficients.append(2 / len(signal) * value)
    return coefficients

def approximate(coefficients, period, count):
    result = np.empty(count)
    result.fill(coefficients[0])


    coef_num = (len(coefficients) - 1) // 2
    for k in range(1, coef_num + 1):
        for n in range(count):
            result[n] += coefficients[k] * np.cos(2 * np.pi * k * (n * period / count) / period)
            result[n] += coefficients[k + coef_num] * np.sin(2 * np.pi * k * (n * period / count) / period)
    
    return result

def sawtooth():
    result = np.empty(1000)
    t = np.linspace(-0.5, 0.5, 1000)
    for i in range(len(t)):
        if(t[i] < 0):
            result[i] = 1 + 2 * t[i]
        else:
            result[i] = -1 - 2 * t[i]
    return result

def main():
    signal = []
    for i in range(500):
        signal.append(-1)
    for i in range(500):
        signal.append(1)

    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.show()

    coef = fourierCoefficients(signal, 1, 1)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signal, 1, 5)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signal, 1, 10)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()


    coef = fourierCoefficients(signal, 1, 50)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signal, 1, 100)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signal, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    signalSaw = sawtooth()

    coef = fourierCoefficients(signalSaw, 1, 1)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signalSaw, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signalSaw, 1, 5)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signalSaw, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signalSaw, 1, 10)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signalSaw, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()


    coef = fourierCoefficients(signalSaw, 1, 50)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signalSaw, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

    coef = fourierCoefficients(signalSaw, 1, 100)
    approximation = approximate(coef, 1, 1000)
    plt.plot(np.linspace(-0.5, 0.5, 1000), signalSaw, label='Original')
    plt.plot(np.linspace(-0.5, 0.5, 1000), approximation, label='Approximation')
    plt.show()

if __name__ == "__main__":
    main()
