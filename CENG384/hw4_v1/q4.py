import numpy as np
import matplotlib.pyplot as plt
import scipy.io as sio


def fft(signal):
    N = len(signal)
    if N <= 1:
        return signal

    # dividing signal into two parts and fft'ing them
    evenPart = fft(signal[::2])
    oddPart = fft(signal[1::2])

    fourierTransform = np.zeros(N, dtype=np.complex128)
    for i in range(N // 2):
        """ We couldn't get why but fft only works if we multiply the odd part,
        our odd part interpretion might be wrong, we would be happy if you leave a comment."""
        oddPart_i = oddPart[i] * np.exp(-2j * np.pi * i / N)
        fourierTransform[i] = evenPart[i] + oddPart_i
        fourierTransform[i + N // 2] = evenPart[i] - oddPart_i
    return fourierTransform


def ifft(signal):
    csignal = np.conjugate(signal)
    return np.conjugate(fft(csignal))/len(signal)


def plotter(data, title, xlabel, ylabel):
    plt.figure()
    plt.plot(np.abs(data))
    plt.title(title)
    plt.ylabel(ylabel)
    plt.xlabel(xlabel)


def decode(rate, data):
    # plot original data in time domain
    plotter(data, "Encoded data in time domain", "n", "|x[n]|")

    # fourier transforming data
    fourierTransformed = fft(data)
    plotter(fourierTransformed,
            "Encoded data in frequency domain", "k", "|X[k]|")

    # dividing data to positive and negative part
    firstHalf = fourierTransformed[:len(data)//2]
    secondHalf = fourierTransformed[len(data)//2:]

    # concatanating them
    reversedAndConcatenated = np.concatenate(
        [firstHalf[::-1], secondHalf[::-1]])

    # inverse fft
    newData = ifft(reversedAndConcatenated)

    # normalizing
    decoded = np.real(newData)/np.max(np.abs(newData))
    plotter(decoded, "Decoded data in time domain", "n", "|x'[n]|")

    # normalizing and writing to file.
    sio.wavfile.write('decodedOutput.wav', rate, decoded)
    return data


def main():
    # reading rate and data
    rate, data = sio.wavfile.read("encoded.wav")
    decoded = decode(rate, data)
    fourierOfDecoded = fft(decoded)
    plotter(fourierOfDecoded, "Decoded data in frequency domain", "k", "|X'[k]|")
    plt.show()
    """Secret message is I have a Dream."""


if __name__ == "__main__":
    main()
