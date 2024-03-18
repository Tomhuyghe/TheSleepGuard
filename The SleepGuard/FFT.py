import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import firwin, freqz

# Charger les données à partir du fichier CSV
file_path = 'chemin/vers/votre/fichier.csv'  # Remplacez par le chemin réel de votre fichier
data = pd.read_csv(file_path, header=None)

# Extraire la première colonne du DataFrame comme signal avgBPM
avgBPM_signal = data.iloc[:, 0].values

# Calculer la transformée de Fourier du signal avgBPM
fft_result = np.fft.fft(avgBPM_signal)
fft_magnitude = np.abs(fft_result)
fft_frequency = np.fft.fftfreq(len(avgBPM_signal))

# Tracer la transformée de Fourier
plt.figure()
plt.plot(fft_frequency, fft_magnitude)
plt.title('Transformée de Fourier du Signal avgBPM')
plt.xlabel('Fréquence (Hz)')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()

# Choisissez une fréquence de coupure à partir de l'analyse de la transformée de Fourier
cutoff_frequency = 0.1  # Remplacez par la valeur appropriée en Hz

# Choisissez un ordre initial du filtre
filter_order = 30  # Remplacez par la valeur appropriée

# Conception du filtre passe-bas avec firwin
fir_coefficients = firwin(filter_order, cutoff=cutoff_frequency, fs=2*cutoff_frequency, pass_zero=True)

# Réponse en fréquence du filtre
frequency_response = freqz(fir_coefficients, 1, worN=8000, fs=2*cutoff_frequency)

# Tracé de la réponse en fréquence
plt.figure()
plt.plot(0.5 * 2 * cutoff_frequency * frequency_response[0] / np.pi, 20 * np.log10(np.abs(frequency_response[1])), 'b-')
plt.title('Réponse en Fréquence du Filtre Passe-Bas')
plt.xlabel('Fréquence (Hz)')
plt.ylabel('Atténuation (dB)')
plt.grid(True)
plt.show()
