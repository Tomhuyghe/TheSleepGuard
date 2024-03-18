// Analysing.h
#ifndef ANALYSING_H
#define ANALYSING_H

#include "Arduino.h"
#include <list>
#include <SD.h>

// Déclaration des fonctions
void setupAnalysis();
void analyzeSleep();
unsigned long calculateTotalTime(const std::list<unsigned long>& times);
unsigned long calculateCurrentTime(const String& line);
float calculateAverageBPM(const std::list<int>& bpms);
unsigned long calculateTotalTimeNotInWake(const std::list<unsigned long>& allTimes, const std::list<unsigned long>& wakeTimes);
String formatTime(unsigned long totalTime);

extern std::list<unsigned long> wakeTimes;
extern std::list<unsigned long> remSleepTimes;
extern std::list<unsigned long> lightSleepTimes;
extern std::list<unsigned long> deepSleepTimes;

#endif

