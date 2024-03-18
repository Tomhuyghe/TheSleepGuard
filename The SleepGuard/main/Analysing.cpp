#include "Analysing.h"

std::list<unsigned long> wakeTimes;
std::list<unsigned long> deepSleepTimes;
std::list<unsigned long> lightSleepTimes;
std::list<unsigned long> remSleepTimes;

std::list<int> allBPMs;  // Liste pour stocker tous les BPM
std::list<unsigned long> allTimes;  // Liste pour stocker tous les temps écoulés
void setupAnalysis() {
    // Initialisation des composants si nécessaire
}
void analyzeBPM(int bpm, unsigned long elapsedTime) {
    if (bpm > 73) {
        wakeTimes.push_back(elapsedTime);
    } else if (bpm <45) {
        deepSleepTimes.push_back(elapsedTime);
    } else if (bpm > 45 && bpm < 58) {
        lightSleepTimes.push_back(elapsedTime);
    } else if (bpm >58 && bpm < 73) {
        remSleepTimes.push_back(elapsedTime);
    }
}
void analyzeSleep() {
    File dataFile = SD.open("heart_rate_data.csv");
    if (!dataFile) {
        Serial.println("Erreur lors de l'ouverture du fichier");
        return;
    }

    int highestBPM = 0;
    int lowestBPM = 255;

    unsigned long lastTime = 0;
    unsigned long elapsedTime;
    
    while (dataFile.available()) {
        String line = dataFile.readStringUntil('\n');
        line.trim();
        
        int commaIndex = line.indexOf(',');
        if (commaIndex != -1) {
            String bpmString = line.substring(commaIndex + 1);
            int bpm = bpmString.toInt();

            if (bpm > highestBPM) {
                highestBPM = bpm;
            }

            if (bpm < lowestBPM) {
                lowestBPM = bpm;
            }

            unsigned long currentTime = calculateCurrentTime(line);

            if (lastTime != 0) {
                elapsedTime = currentTime - lastTime;

                // Stocker le BPM et le temps écoulé
                allBPMs.push_back(bpm);
                allTimes.push_back(elapsedTime);

                // Filtrer les phases selon les seuils BPM
                analyzeBPM(bpm, elapsedTime);
            }

            lastTime = currentTime;

        }
    }

    dataFile.close();

    Serial.println("Point le plus haut : " + String(highestBPM));
    Serial.println("Point le plus bas : " + String(lowestBPM));

    // Calculer et afficher la moyenne des BPM
    float averageBPM = calculateAverageBPM(allBPMs);
    Serial.println("Moyenne des BPM : " + String(averageBPM));

    // Calculer et afficher le temps total passé hors de la phase WAKE
    unsigned long totalTimeNotInWake = calculateTotalTimeNotInWake(allTimes, wakeTimes);
    Serial.println("Temps total passé hors de la phase WAKE : " + formatTime(totalTimeNotInWake));

    Serial.println("Temps total passé en WAKE (BPM > 73): " + formatTime(calculateTotalTime(wakeTimes)));
    Serial.println("Temps total passé en REM_SLEEP (58 < BPM < 73): " + formatTime(calculateTotalTime(remSleepTimes)));
    Serial.println("Temps total passé en LIGHT_SLEEP (45 < BPM < 58): " + formatTime(calculateTotalTime(lightSleepTimes)));
    Serial.println("Temps total passé en DEEP_SLEEP (BPM < 45): " + formatTime(calculateTotalTime(deepSleepTimes)));
}

unsigned long calculateTotalTime(const std::list<unsigned long>& times) {
    // Implementation de la fonction pour calculer le temps total dans une liste
    unsigned long totalTime = 0;
    for (const auto& time : times) {
        totalTime += time;
    }
    return totalTime;
}

unsigned long calculateCurrentTime(const String& line) {
    int commaIndex = line.indexOf(',');
    String timeString = line.substring(0, commaIndex);
    int hours = timeString.substring(0, timeString.indexOf(':')).toInt();
    int minutes = timeString.substring(timeString.indexOf(':') + 1, timeString.lastIndexOf(':')).toInt();
    int seconds = timeString.substring(timeString.lastIndexOf(':') + 1).toInt();
    return (hours * 3600 + minutes * 60 + seconds) * 1000;
}

float calculateAverageBPM(const std::list<int>& bpms) {
    // Implementation de la fonction pour calculer la moyenne des BPM dans une liste
    if (bpms.empty()) {
        return 0.0;
    }

    float totalBPM = 0.0;
    for (const auto& bpm : bpms) {
        totalBPM += static_cast<float>(bpm);
    }

    return totalBPM / bpms.size();
}

unsigned long calculateTotalTimeNotInWake(const std::list<unsigned long>& allTimes, const std::list<unsigned long>& wakeTimes) {
    // Implementation de la fonction pour calculer le temps total passé hors de la phase WAKE
    unsigned long totalTimeNotInWake = calculateTotalTime(allTimes) - calculateTotalTime(wakeTimes);
    return totalTimeNotInWake;
}


String formatTime(unsigned long totalTime) {
    int seconds = totalTime / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;
    return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}
