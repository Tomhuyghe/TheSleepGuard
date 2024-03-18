#include <Wire.h>
#include <U8g2lib.h>
#include "MesureSleep.h"
#include "Analysing.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
bool buttonState = LOW;
bool lastButtonState = LOW;
bool analysisInProgress = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long longPressDuration = 1000; // Durée d'appui long en millisecondes
unsigned long pressStartTime = 0;
bool buttonPressed = false; // Variable pour suivre l'état de l'appui long
int menuOption = 0; // Variable pour suivre l'option actuelle du menu
bool stopLoop = false;

void setup() {
  
  Serial.begin(115200);
  Serial.println("Initializing...");

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.clearBuffer();
  u8g2.drawStr(1, 12, "The SleepGuard");

  u8g2.setFont(u8g2_font_courB08_tr);
  u8g2.drawStr(30, 30, "Smart Sleep ");

  u8g2.setFont(u8g2_font_courB08_tr);
  u8g2.drawStr(30, 40, "Optimization");

  u8g2.setFont(u8g2_font_courB08_tr);
  u8g2.drawStr(30, 50, " Wristband");

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(25, 63, " Press Button");
  
  u8g2.sendBuffer();
  setupMeasure();
  setupAnalysis();  // Ajoute cette ligne pour initialiser l'analyse
}

void loop() {
  int reading = digitalRead(0); // Lecture de l'état du bouton sur la broche 0

  // Détecter un changement d'état du bouton
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Mettre à jour le temps de rebondissement
  }

  // Vérifier si le bouton est stable
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si l'état du bouton a changé, et il est maintenant HIGH, alors le bouton a été appuyé
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        // Incrémenter l'option du menu lors d'un appui court
        int previousOption = menuOption;
        menuOption = (menuOption + 1) % 2; // Deux options dans le menu

        // Afficher une nouvelle page sur l'écran OLED en fonction de l'option du menu
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.clearBuffer();
        u8g2.drawStr(30, 15, "MENU");

        u8g2.setFont(u8g2_font_ncenB08_tr);

        if (menuOption == 0) {
          u8g2.drawStr(10, 35, "> Starting Sleep ");
          u8g2.drawStr(10, 55, "  Starting Analysis");
        } else {
          u8g2.drawStr(10, 35, "  Starting Sleep ");
          u8g2.drawStr(10, 55, "> Starting Analysis");
        }

        u8g2.sendBuffer();
        
        // Enregistrer le temps d'appui pour la gestion de l'appui long
        pressStartTime = millis();

        // Vérifier si l'option a changé
        if (previousOption != menuOption) {
          // Si l'option a changé, réinitialiser l'état de l'appui long
          buttonPressed = false;
        }
      }
    }
  }

  // Gérer l'appui long sur les options du menu
  if (buttonState == LOW && !buttonPressed) {
    if ((millis() - pressStartTime) > longPressDuration) {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.clearBuffer();

      if (menuOption == 1) {
        SD.remove("heart_rate_data.csv");
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.clearBuffer();
        u8g2.drawStr(1, 45, "RECORDING... ");
        u8g2.sendBuffer();
        delay(1000);
        while (stopLoop == false) {
        loopMeasure();
        }
      } else {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.clearBuffer();
        analyzeSleep();
        u8g2.setCursor(0, 15);
        u8g2.print("WAKE: ");
        u8g2.print(formatTime(calculateTotalTime(wakeTimes)));

        u8g2.setCursor(0, 30);
        u8g2.print("REM: ");
        u8g2.print(formatTime(calculateTotalTime(remSleepTimes)));

        u8g2.setCursor(0, 45);
        u8g2.print("LIGHT: ");
        u8g2.print(formatTime(calculateTotalTime(lightSleepTimes)));

        u8g2.setCursor(0, 60);
        u8g2.print("DEEP: ");
        u8g2.print(formatTime(calculateTotalTime(deepSleepTimes)));
      }

      u8g2.sendBuffer();

      // Marquer le bouton comme appuyé longtemps pour éviter les répétitions
      buttonPressed = true;
    }
  }

  // Mettre à jour l'état du bouton précédent
  lastButtonState = reading;
}