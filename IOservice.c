#include "IOservice.h"
#include "arraylist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Checks if the file is valid // not null
 * Exits if invalid
 * @param file
 */
void checkForNullFile(FILE* file) {
  if(file == NULL) {
    printf("Cannot create or read file");
    exit(1);
  }
}

/**
 * Gets the index of argument flag ("--") in arguments array provided by user
 * @param argTarget - the target flag
 * @param argv - array of arguments provided by user
 * @param argc - count of arguments
 * @return index of the flag
 */
int getIndexOfArgument(const char *argTarget, char **argv, int argc) {
    for(int i = 0; i < argc; ++i) {
      if(strstr(argTarget, argv[i])) {
          return i;
      }
    }
    return -1;
}

/**
 * Updates command line arguments at the given memory
 * @param argv - array of arguments provided by user
 * @param argc - count of arguments
 * @param input - input file path
 * @param output - output file path
 * @param infectionsPerDay
 * @param infDays
 */
void readArguments(char **argv, int argc, char *input, char *output, double *infectionsPerDay, int *infDays) {
  int idxInput = getIndexOfArgument("--real-data", argv, argc);
  int idxOutput = getIndexOfArgument("--output", argv, argc);
  int idxInfPerDay = getIndexOfArgument("--infections_per_day", argv, argc);
  int idxInfDays = getIndexOfArgument("--infectious_days", argv, argc);

  strcpy(input, argv[idxInput+1]);
  strcpy(output, argv[idxOutput+1]);
  *infectionsPerDay = atof(argv[idxInfPerDay+1]);
  *infDays = atoi(argv[idxInfDays+1]);
}


/**
 * Fills argsCount array which keeps track of used parameters 
 * @param argsCount - keeps track of argument occurances
 * @param argv - arguments provided by user
 * @param argc - number of arguments
 */
void populateArgsCountArray(int *argsCount, char *argv[], int argc) {
  for(int i = 0; i < argc; ++i) {
    char *curArg = argv[i];
    if(strstr("--real-data", curArg) != NULL) {
      argsCount[0]++;
    } else if(strstr("--infectious_days", curArg) != NULL) {
      argsCount[1]++; 
    } else if(strstr("--infections_per_day", curArg) != NULL) {
      argsCount[2]++;
    } else if(strstr("--output", curArg) != NULL) {
      argsCount[3]++;
    } else if(strstr("--", curArg) != NULL){
      // if any other -- argument is passed -> return error
      printf("Invalid argument");
      exit(1); 
    }
  }
}

/**
 * Checks if the expected number of parameters matches the actual number
 * @param actual number of parameters
 * @param expected number of parameters
 */
void checkParamCount(int argc, int expected) {
  if(argc > expected) {
    printf("too many parameters");
    exit(1);
  } else if(argc < expected) {
    printf("too few parameters");
    exit(1);
  }
}

/**
 * Valides command line parameters provided by user according to the specs 
 * @param argsCount - keeps track of argument occurances
 * @param argv - arguments provided by user
 * @param argc - number of arguments
 */
void validateCmdParams(char **args, int argc, int expectedParamCount) {
  checkParamCount(argc, expectedParamCount);
  int argsCount[5] = {0};
  populateArgsCountArray(argsCount, args, argc);
  for(int i = 0; i < (expectedParamCount-1)/2; ++i) {
    if(argsCount[i] > 1) {
      printf("Duplicate parameters");
      return;
    }
  } 
}

/**
 * Fills record struct parameters based on the index
 * @param record
 * @param fieldIndex
 * @param value
 */
void fillCsvRecord(Record *record, int fieldIndex, double value) {
  switch (fieldIndex) {
    case 0:
      record->healthyCount = value;
      break;
    case 1:
      record->infectiousCount = value;
      break;
    case 2:
      record->recoveredCount = value;
      break;
    default:
      printf("invalid data provided");
      exit(0);
  }
}

/***
 * Fills list of records with data from csv file
 * @param list - data gonna be stored here
 * @param csvPath - path to the csv data file
 */
void loadCsvIntoArrayList(ArrayList *list, const char *csvPath) {
  FILE *csvDataFile = fopen(csvPath, "r");
  checkForNullFile(csvDataFile);
  char inputLine[256];
  while (fgets(inputLine, 256, csvDataFile)) {
    inputLine[strcspn(inputLine, "\n")] = 0;
    char *token = strtok(inputLine, ",");
    int idx = 0;
    Record record;
    while (token != NULL) {
      // field separated by ','
      double value = atof(token);
      fillCsvRecord(&record, idx, value);
      token = strtok(NULL, ",");
      idx++;
    }
    add(list, record);
  }
  fclose(csvDataFile);
}

/**
 * Prints the chart line based on type
 * @param chart - the output file
 * @param data - data from which line is created
 * @param days - number of records
 * @param type - type of the line; 1 = healthy, 2 = infectious, 3 = recovered
 * @param hexColor - color of the line
 * @param maxHeight - height of the chart (not svg)
 * @param xOffset - offset on the x axis from the edge (0)
 */
void printChartLine(FILE *chart, ArrayList *data, int days, int type, const char *hexColor, int maxHeight, int xOffset) {
  switch (type) {
  // healthy
  case 1:
    fprintf(chart,
            "\t<polyline\n\tfill='none'\n\tstroke='%s'\n\tstroke-width='2'\n\tpoints='\n", hexColor);
    for (int i = 0; i < days; ++i) {
      fprintf(chart, "\t\t%d, %f\n", xOffset + i, maxHeight - (get(data, i).healthyCount * 100));
    }
    fprintf(chart, "\t'/>\n");
    break;
  // infectious
  case 2:
    fprintf(chart,
            "\t<polyline\n\tfill='none'\n\tstroke='%s'\n\tstroke-width='2'\n\tpoints='\n", hexColor);
    for (int i = 0; i < days; ++i) {
      fprintf(chart, "\t\t%d, %f\n", xOffset + i, maxHeight - (get(data, i).infectiousCount * 100));
    }
    fprintf(chart, "\t'/>\n");
    break;
  // recovered
  case 3:
    fprintf(chart,
            "\t<polyline\n\tfill='none'\n\tstroke='%s'\n\tstroke-width='2'\n\tpoints='\n", hexColor);
    for (int i = 0; i < days; ++i) {
      fprintf(chart, "\t\t%d, %f\n", xOffset + i, maxHeight - (get(data, i).recoveredCount * 100));
    }
    fprintf(chart, "\t'/>\n");
    break;
  default:
    printf("invalid line type!\n");
    return;
  }
}

/**
 * Prints x axis to the chart file
 * @param chartFile - the output file
 * @param days - count of records
 * @param maxHeight - height of the graph (not svg)
 */
void printXAxis(FILE *chartFile, int xOffset, int days, int maxHeight) {
  fprintf(chartFile, "<g style='stroke: #ccc; stroke-width: 1'>\n");
  fprintf(chartFile, 
      "<line x1='%d' x2='%d' y1='%d' y2='%d'></line>\n", xOffset, days + 150, maxHeight, maxHeight);
  fprintf(chartFile, "</g>\n");
  fprintf(chartFile, "<g style='font-size: 13px;'>\n");
  for (int i = 0; i < days; i += 25) {
    fprintf(chartFile, "<text x='%d' y='130'>%d</text>\n", xOffset + i, i);
  }
  fprintf(chartFile, "<text x='%d' y='165'>Days</text>n",
          (2 * xOffset + days) / 2);
  fprintf(chartFile, "</g>\n");
}

/**
 * Prints y axis to the chart file
 * @param chartFile - the output file
 * @param xOffset - offset from the edge on the x axis
 * @param maxHeight - height of the graph (not svg)
 */
void printYAxis(FILE *chartFile, int xOffset, int maxHeight) {
  fprintf(chartFile, "<g style='stroke: #ccc; stroke-width: 1'>\n");
  fprintf(chartFile, "<line x1='%d' x2='%d' y1='0' y2='%d'></line>\n", xOffset, xOffset, maxHeight);
  fprintf(chartFile, "</g>\n");

  fprintf(chartFile, "<g style='font-size: 15px;'>\n");
  for (int i = maxHeight; i > 0; i -= 25) {
    fprintf(chartFile, "<text x='%d' y='%d'>%d</text>\n", xOffset - 30, i, maxHeight - i);
  }
  fprintf(chartFile, "<text x='%d' y='%d'>People[%%]</text>\n", 10, maxHeight / 2);
  fprintf(chartFile, "</g>\n");
}

/**
 * Prints SIR lines to the chart based on the data set
 * @param chart - output file
 * @param data - data set (real or predicted)
 * @param days - number of records
 * @param maxHeight - height of chart (not svg)
 * @param colors - array of 3 colors - for each SIR line one
 * @param xOffset - offset from the x axis edge (0)
 */
void printChartLines(FILE *chart, ArrayList *data, int days, int maxHeight, char *colors[3], int xOffset) {
  // healthy line
  printChartLine(chart, data, days, 1, colors[0], maxHeight, xOffset);
  // infectious line
  printChartLine(chart, data, days, 2, colors[1], maxHeight, xOffset);
  // recovered line
  printChartLine(chart, data, days, 3, colors[2], maxHeight, xOffset);
}

/**
 * Gets legend name based on the index
 * @param index
 * @return legend name
 */
char *getLegendName(int index) {
  if (index == 0)
    return "Susceptible";
  if (index == 1)
    return "Infectious";
  if (index == 2)
    return "Recovered";
  return "Invalid index";
}

/**
 * Prints legend color and name
 * @param chartFile - output file
 * @param realDataColors - colors for real data set's lines
 * @param predictionDataColors - colors for prediction data set's lines
 * @param n - size of color arrays - defaults to 3
 * @param xOffset - offset on the x axis from the edge (0)
 */
void printLegend(FILE *chartFile, char **realDataColors, char **predictionDataColors, int n) {
  fprintf(chartFile, "<g style='stroke-width: 1; font-size: 14px;'>\n");
  // real data colors
  int yCor = 0;
  for (int i = 0; i < n; ++i) {
    int cy = 200 + yCor;
    fprintf(chartFile, 
        "<circle style='fill: %s'cx='%d'cy='%d' r='8'></circle>", realDataColors[i], 10, cy);
    fprintf(chartFile, "<text x='%d' y='%d'>%s</text>\n", 30, cy + 5, getLegendName(i));
    yCor += 20;
  }
  // predicted data colors
  yCor = 0;
  for (int i = 0; i < n; ++i) {
    int cy = 200 + yCor;
    fprintf(chartFile, 
        "<circle style='fill: %s'cx='%d'cy='%d' r='8'></circle>", predictionDataColors[i], 150, cy);
    fprintf(chartFile, "<text x='%d' y='%d'>%s</text>\n", 180, cy + 5, getLegendName(i));
    yCor += 20;
  }
  fprintf(chartFile, "</g>\n");
}

/**
 * Generates svg chart with lines for real and predicted data sets
 * @param chartFile - output file
 * @param dataStore - real data set
 * @param predictionStore - predicted data set
 */
void makeSvgChart(FILE *chartFile, ArrayList *dataStore, ArrayList *predictionStore) {
  int days = getSize(dataStore);
  char *realDataColors[3] = {"#B27AB3", "#4252A2", "#33BFEB"};
  char *predictionDataColors[3] = {"#F4AB1A", "#E70012", "#96C625"};
  int maxChartHeight = 100;
  int xOffset = 120;
  fprintf(chartFile, "<svg style='padding: 30px;height: 500px; width: 800px; font-family:'Arial'>\n");
  // axes
  printYAxis(chartFile, xOffset, maxChartHeight);
  printXAxis(chartFile, xOffset, days, maxChartHeight);
  // lines
  printChartLines(chartFile, dataStore, days, maxChartHeight, realDataColors, xOffset);
  printChartLines(chartFile, predictionStore, days, maxChartHeight,predictionDataColors, xOffset);
  // legend
  printLegend(chartFile, realDataColors, predictionDataColors, 3);
  fprintf(chartFile, "</svg>");
}
