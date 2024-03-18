// MesureSleep.h
#ifndef MESURE_SLEEP_H
#define MESURE_SLEEP_H

#include <Wire.h>
#include <SD.h>
#include "MAX30105.h"
#include "heartRate.h"
extern bool buttonPressed;
extern bool stopLoop;
extern bool buttonState;

void setupMeasure();
void loopMeasure();

#endif
