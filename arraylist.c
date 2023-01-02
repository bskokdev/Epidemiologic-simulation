#include "arraylist.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Fills record struct with all parameters at once
 * @param record
 * @param healthy
 * @param infectious
 * @param recovered
 */
void newCsvRecord(Record *record, double healthy, double infectious, double recovered) {
  record->healthyCount = healthy;
  record->infectiousCount = infectious;
  record->recoveredCount = recovered;
}

void newArrayList(ArrayList *self, int maxCapacity) {
  self->data = (Record *)malloc(maxCapacity * sizeof(Record));
  self->maxCapacity = maxCapacity;
  self->currCapacity = 0;
}

void add(ArrayList *self, Record item) {
  if (self->maxCapacity <= self->currCapacity) {
    self->data =
        (Record *)realloc(self->data, self->maxCapacity * 2 * sizeof(Record));
    self->maxCapacity = self->maxCapacity * 2;
  }
  self->data[self->currCapacity++] = item;
}

Record get(ArrayList *self, int index) {
  if (index > self->currCapacity) {
    printf("index out of bounds!");
    exit(1);
  }
  return self->data[index];
}

int getSize(ArrayList *self) { return self->currCapacity; }

void freeArrayList(ArrayList *self) { free(self->data); }
