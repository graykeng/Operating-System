#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define MAX_TRACK 200
#define MAX_SEQUENCE_TRACK_NUMBER 66

List* twoHundred;
List* original;
List* originalBackup;
List* FCFS;
List* SCAN;
List* CSCAN;
time_t t;

typedef struct TRACK{
    int value;
    int orderAfter;
    int orderBefore;
}track;

// Malloc for track
track* Track_create();

// Initiate everything
void init();

// Return 50 random in a list (0 - MAX)
void fillOriginalList(int MAX);

// Print the whole list
void listPrinter(List* list);

// Print the traversed number of FCFS algorithm
void traversedPrinterFCFS();

// Print the traversed number of SCAN algorithm
void traversedPrinterSCAN();

// Print the longest Delay track
void longestDelaySCAN();

void averageDelaySCAN();

// Print the traversed number of CSCAN algorithm
void traversedPrinterCSCAN();

void longestDelayCSCAN();

void averageDelayCSCAN();

// Copy list.
void makeFCFS(List* copyTo, List* toBeCopy);

// Make SCAN algorithm
void makeSCAN();

// Make CSCAN algorithm
void makeCSCAN();