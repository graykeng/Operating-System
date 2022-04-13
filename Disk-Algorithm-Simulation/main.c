#include "diskSimulation.h"
#include "list.h"

int main(){
    puts("=====Disk Scheduling Simulation=====");
    init();

    // Make the original random list
    fillOriginalList(MAX_SEQUENCE_TRACK_NUMBER);

    // Print original random list
    printf("\n=====Printing original random list...=====\n");
    listPrinter(original);

    // Make the FCFS random list (simply copy original list to FCFS)
    makeFCFS(FCFS, original);

    // Print FCFS
    printf("\n=====Printing FCFS...=====\n");
    listPrinter(FCFS);
    traversedPrinterFCFS();

    // Make the SCAN algorithm
    makeSCAN();

    // Print SCAN
    printf("\n=====Printing SCAN...=====\n");
    listPrinter(SCAN);
    traversedPrinterSCAN();
    longestDelaySCAN();
    averageDelaySCAN();

    // Make the CSCAN algorithm
    makeCSCAN();

    // Print CSCAN
    printf("\n=====Printing CSCAN...=====\n");
    listPrinter(CSCAN);
    traversedPrinterCSCAN();
    longestDelayCSCAN();
    averageDelayCSCAN();
    return 0;
}