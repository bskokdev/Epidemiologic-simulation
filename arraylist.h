#pragma once // cycle prevention
#ifndef ARRAY_LIST
#define ARRAY_LIST

typedef struct {
  double healthyCount;
  double infectiousCount;
  double recoveredCount;
} Record;

typedef struct {
  Record *data;
  int maxCapacity;
  int currCapacity;
} ArrayList;

void newArrayList(ArrayList *self, int maxCapacity);
void add(ArrayList *self, Record item);
Record get(ArrayList *self, int index);
void freeArrayList(ArrayList *self);
void newCsvRecord(Record *record, double healthy, double infectious,
                  double recovered);
int getSize(ArrayList *self);

#endif
