#pragma once
#include "arraylist.h"
#include <stdio.h>
#ifndef IO_SERVICE
#define IO_SERVICE

void checkForNullFile(FILE* file);
void validateCmdParams(char **argv, int argc, int expectedParamsCount);
void loadCsvIntoArrayList(ArrayList *list, const char *csvPath);
void makeSvgChart(FILE *chartFile, ArrayList *dataStore, ArrayList *predictionStore);
void readArguments(char **argv, int argc, char *input, char *output, double *infPerDay, int *infDays);
#endif // !IO_SERVICE
