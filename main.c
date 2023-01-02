#include "IOservice.h"
#include "arraylist.h"
#include "predictionService.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // validation of args
  // +1 is for the ./program arg itself; *2 is for string + value arguments
  int expectedNumberOfArgs = 4;
  validateCmdParams(argv, argc, (expectedNumberOfArgs*2)+1);

  // allocate memory for arguments
  char *input = (char *) malloc(256 * sizeof(char));
  char *output = (char *) malloc(256 * sizeof(char));
  double *infectionsPerDay = (double *) malloc(sizeof(double));
  int *infectiousDays = (int *) malloc(sizeof(int));

  // read arguments
  readArguments(argv, argc, input, output, infectionsPerDay, infectiousDays);

  // initialize & populate data structures
  ArrayList dataStore;
  ArrayList predictionsStore;

  newArrayList(&dataStore, 10);
  newArrayList(&predictionsStore, 10);

  loadCsvIntoArrayList(&dataStore, input);
  convertOriginalDataToFractions(&dataStore);

  // calculate predictions
  Record firstDay = get(&dataStore, 0);
  predictDaysAndSaveToList(
      &predictionsStore, firstDay, getSize(&dataStore), *infectionsPerDay, *infectiousDays);

  // make chart
  FILE *svgChart = fopen(output, "w");
  checkForNullFile(svgChart);
  makeSvgChart(svgChart, &dataStore, &predictionsStore);

  // free the memory of data sets
  freeArrayList(&dataStore);
  freeArrayList(&predictionsStore);

  // free memory of arguments
  free(input);
  free(output);
  free(infectionsPerDay);
  free(infectiousDays);

  // close created chart file
  fclose(svgChart);
  return 0;
}
