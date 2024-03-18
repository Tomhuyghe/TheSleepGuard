#include "MesureSleep.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
float beatAvg;

const int IR_THRESHOLD = 50000;
const int BPM_MIN = 40;
const int BPM_MAX = 150;

File dataFile;  // Variable pour stocker le fichier sur la carte SD
unsigned long startTime;

void setupMeasure()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }

  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  // Initialise la carte SD
  if (!SD.begin(10))  // Assurez-vous de connecter correctement la broche CS de la carte SD à la broche 10 de l'Arduino
  {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  // Enregistre le temps de début
  startTime = millis();
}

void loopMeasure()
{
  long irValue = particleSensor.getIR();

  if (irValue < IR_THRESHOLD)
  {
    Serial.println("No finger detected. Place your finger on the sensor with steady pressure.");
    delay(1000);
    stopLoop = true;
    return;
  }

  if (checkForBeat(irValue))
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute >= BPM_MIN && beatsPerMinute <= BPM_MAX)
    {
      rates[rateSpot++] = static_cast<byte>(beatsPerMinute);
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;

      Serial.print("Avg BPM=");
      Serial.println(beatAvg);

      // Enregistre la valeur dans le fichier CSV
      dataFile = SD.open("heart_rate_data.csv", FILE_WRITE);
      if (dataFile)
      {
        // Calcul du temps écoulé depuis le début de l'analyse
        unsigned long elapsedTime = millis() - startTime;
        int hours = elapsedTime / 3600000;
        int minutes = (elapsedTime % 3600000) / 60000;
        int seconds = (elapsedTime % 60000) / 1000;

        // Enregistre le temps au format HH:MM:SS et beatAvg dans le fichier CSV
        dataFile.print(hours);
        dataFile.print(":");
        dataFile.print(minutes);
        dataFile.print(":");
        dataFile.print(seconds);
        dataFile.print(",");
        dataFile.println(beatAvg);

        dataFile.close();
      }
      else
      {
        Serial.println("Error opening file for writing");
      }
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  Serial.println();
}
