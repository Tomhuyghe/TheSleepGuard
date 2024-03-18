
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz

# Coefficients du filtre
fir_coefficients = [0.1, 0.2, 0.3, 0.4]

# Réponse impulsionnelle
n = np.arange(len(fir_coefficients))
impulse_response = fir_coefficients

# Réponse en fréquence
w, h = freqz(fir_coefficients, worN=8000)

# Tracé de la réponse impulsionnelle
plt.figure()
plt.stem(n, impulse_response, basefmt='b', use_line_collection=True)
plt.title('Réponse Impulsionnelle du Filtre FIR Passe-Bas (Ordre 4)')
plt.xlabel('n (Échantillons)')
plt.ylabel('Amplitude')
plt.grid(True)

# Tracé de la réponse en fréquence
plt.figure()
plt.plot(0.5 * w / np.pi, np.abs(h), 'b-')
plt.title('Réponse en Fréquence du Filtre FIR Passe-Bas (Ordre 4)')
plt.xlabel('Fréquence Normalisée')
plt.ylabel('Magnitude')
plt.grid(True)
plt.show()
