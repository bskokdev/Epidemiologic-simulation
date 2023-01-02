#pragma once
#include "arraylist.h"

#ifndef PREDICTION_SERVICE
#define PREDICTION_SERVICE
int predictSusceptible(int prevDayHealthy, int prevDayInfectious,
                       double infectionsPerDay);

int predictInfected(int prevDayHealthy, int prevDayInfectious,
                    double infectionsPerDay, int infectiousDays);

int predictRecovered(int prevDayRecovered, int prevDayInfectious,
                     int infectiousDays);

void predictNextDay(Record prevDay, Record *predictedDay,
                    double infectionsPerDay, int infectiousDays);

void predictDaysAndSaveToList(ArrayList *predictions, Record firstDay, int n,
                              double infectionsPerDay, int infectiousDays);

void convertDataRecordToFraction(Record *record, int daySumCount);

void convertOriginalDataToFractions(ArrayList *dataStore);
#endif // !PREDICTION_SERVICE
