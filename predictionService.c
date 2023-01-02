#include "arraylist.h"
#include <stdio.h>

double predictSusceptible(double prevDayHealthy, double prevDayInfectious,
                          double infectionsPerDay) {
  return (prevDayHealthy - infectionsPerDay * prevDayHealthy * prevDayInfectious);
}

double predictInfected(double prevDayHealthy, double prevDayInfectious, double infectionsPerDay, int infectiousDays) {
  return (prevDayInfectious + infectionsPerDay * prevDayHealthy * prevDayInfectious - (1.0 / infectiousDays) * prevDayInfectious);
}

double predictRecovered(double prevDayRecovered, double prevDayInfectious, int infectiousDays) {
  return prevDayRecovered + (1.0 / infectiousDays) * prevDayInfectious;
}

/**
 * Converts record's values to fractions (healthyCount = 1000/1250)
 * @param record
 * @param daySumCount - all record values summed up
 */
void convertDataRecordToFraction(Record *record, double daySumCount) {
  record->healthyCount = record->healthyCount / daySumCount;
  record->infectiousCount = record->infectiousCount / daySumCount;
  record->recoveredCount = record->recoveredCount / daySumCount;
}

double getRecordTotalCount(Record *record) {
  return record->healthyCount + record->infectiousCount + record->recoveredCount;
}

/**
 * This functions predicts the next day based on provided parameters
 * @param prevDay - Record with data from previous day
 * @param infectionsPerDay
 * @param infectiousDays
 * @return Record struct with predicted data
 */
void predictNextDay(Record prevDay, Record *predictedDay, double infectionsPerDay, int infectiousDays, double totalCount) {
  convertDataRecordToFraction(&prevDay, totalCount);
  double predictedHealthy = predictSusceptible(prevDay.healthyCount, prevDay.infectiousCount, infectionsPerDay);
  double predictedInfectious =
      predictInfected(prevDay.healthyCount, prevDay.infectiousCount,
                      infectionsPerDay, infectiousDays);
  double predictedRecovered = predictRecovered(
      prevDay.recoveredCount, prevDay.infectiousCount, infectiousDays);

  newCsvRecord(predictedDay, predictedHealthy, predictedInfectious,predictedRecovered);
}

void convertOriginalDataToFractions(ArrayList *dataStore) {
  int size = getSize(dataStore);
  for (int i = 0; i < size; ++i) {
    Record rec = get(dataStore, i);
    double totalCount = getRecordTotalCount(&rec);
    convertDataRecordToFraction(&dataStore->data[i], totalCount);
  }
}

/**
 * This functions predicts n days and saves them to an arraylist
 * @param predictions - list of Records filled with predicted data
 * @param firstDay - the 1st day from which predictions are made
 * @param n - number of predicted days
 * @param infectionsPerDay
 * @param infectiousDays
 */
void predictDaysAndSaveToList(ArrayList *predictions, Record firstDay, int n, double infectionsPerDay, int infectiousDays) {
  Record prevDay = firstDay;
  double firstDayTotalCount = getRecordTotalCount(&firstDay);
  convertDataRecordToFraction(&firstDay, firstDayTotalCount);
  add(predictions, firstDay);
  for (int i = 0; i < n; ++i) {
    double totalCount = getRecordTotalCount(&prevDay);
    Record predictedDay;
    predictNextDay(prevDay, &predictedDay, infectionsPerDay, infectiousDays,
                   totalCount);

    add(predictions, predictedDay);
    prevDay = predictedDay;
  }
}
