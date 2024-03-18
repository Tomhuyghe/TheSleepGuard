import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# FIR filter coefficients
firCoefficients = [0.1, 0.2, 0.3, 0.4]

# Frequency response
frequency_response = np.fft.fft(firCoefficients, 1024)
frequency_response = np.fft.fftshift(frequency_response)
frequency_response_magnitude = np.abs(frequency_response)

# Frequency axis
sampling_rate = 4000  # Adjust this based on your actual sampling rate
frequency_axis = np.fft.fftshift(np.fft.fftfreq(1024, d=1/sampling_rate))

# Magnitude de la réponse en fréquence
magnitude_response = np.abs(frequency_response)

# Phase de la réponse en fréquence
phase_response = np.angle(frequency_response)

# Affichage de la magnitude
plt.figure()
plt.plot(frequency_axis, magnitude_response)
plt.title('Magnitude de la Réponse en Fréquence du Filtre FIR')
plt.xlabel('Fréquence (Hz)')
plt.ylabel('Magnitude')
plt.grid(True)
plt.show()

# Affichage de la phase
plt.figure()
plt.plot(frequency_axis, phase_response)
plt.title('Phase de la Réponse en Fréquence du Filtre FIR')
plt.xlabel('Fréquence (Hz)')
plt.ylabel('Phase (radians)')
plt.grid(True)

plt.show()

# Plot the frequency response
plt.plot(frequency_axis, 20 * np.log10(frequency_response_magnitude))
plt.title('FIR Filter Frequency Response')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')
plt.grid(True)
plt.show()

# Charger les données à partir du fichier CSV (assurez-vous que le fichier CSV est dans le même répertoire que votre script)
file_path = '/Users/tom/Downloads/heart_rate_data1.csv'  # Remplacez par le chemin réel de votre fichier
data = pd.read_csv(file_path, header=None)

# Extraction de la première colonne du DataFrame comme signal d'entrée
input_signal = data.iloc[:, 0].values

# Convolution avec les coefficients du filtre FIR
output_signal = np.convolve(input_signal, firCoefficients, mode='full')

# Tracer le signal d'entrée et le signal de sortie
plt.figure()
plt.plot(input_signal, label='Signal d\'entrée')
plt.plot(output_signal[:len(input_signal)], label='Signal de sortie (convolution)')
plt.title('Convolution avec le Filtre FIR')
plt.xlabel('Échantillons')
plt.ylabel('Amplitude')
plt.legend()
plt.grid(True)
plt.show()
