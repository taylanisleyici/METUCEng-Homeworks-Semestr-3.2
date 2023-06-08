import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

def ReadWav():
    sampleRate, data = wavfile.read('encoded.wav')
    return sampleRate, data

def FastFourierTransform(data):
    length = len(data)
    if length <= 1:
        return data
    else:
        evenData = data[::2]
        oddData = data[1::2]
        evenPart = FastFourierTransform(evenData)
        oddPart = FastFourierTransform(oddData)
        result = [0] * length
        for k in range(length // 2):
            complexPart = np.exp(-2j * np.pi * k / length)
            result[k] = evenPart[k] + complexPart * oddPart[k]
            result[k + length // 2] = evenPart[k] - complexPart * oddPart[k]
    return result

def SplitReverseConcat(fftData):
    length = len(fftData)
    positive = fftData[:length // 2]
    negative = fftData[length // 2:]
    positiveReversed = np.flip(positive)
    negativeReversed = np.flip(negative)
    result = np.concatenate([positiveReversed, negativeReversed])
    return result

def InverseFourierTransform(data):
    data = np.array(data)
    length = len(data)
    if length <= 1:
        return data
    else:
        conjugate = np.conj(data)
        conjugateFft = FastFourierTransform(conjugate)
        conjugateFft = np.array(conjugateFft)
        conjugateFft = conjugateFft / length
        return np.conj(conjugateFft)

def DecodeTheMessage():
    sampleRate, data = ReadWav()
    fftData = FastFourierTransform(data)
    fftData = SplitReverseConcat(fftData)
    ifftData = InverseFourierTransform(fftData)
    decodedMessage = ifftData.real.astype(np.int16)
    wavfile.write('decoded.wav', sampleRate, decodedMessage)
    Plot(sampleRate, data, fftData, decodedMessage)

def Plot(sampleRate, data, fftData, decodedMessage):
    #Plotting Encoded in Time Domain
    length = data.shape[0] / sampleRate
    time = np.linspace(0., length, data.shape[0])
    plt.plot(time, data)
    plt.title('Encoded in Time Domain')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.grid(True)
    plt.show()
    ################################################
    #Plotting Encoded in Frequency Domain
    plt.magnitude_spectrum(np.abs(fftData), Fs=sampleRate, scale='dB')
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Magnitude (dB)")
    plt.title("Encoded in Frequency Domain")
    plt.show()
    ################################################
    #Plotting Decoded in Time Domain
    plt.plot(time, decodedMessage)
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.title("Decoded in Time Domain")
    plt.show()
    ################################################
    #Plotting Decoded in Frequency Domain
    decodedFft = FastFourierTransform(decodedMessage)
    plt.magnitude_spectrum(np.abs(decodedFft), Fs=sampleRate, scale='dB')
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Magnitude (dB)")
    plt.title("Decoded in Frequency Domain")
    plt.show()
    ###############################################    

DecodeTheMessage()