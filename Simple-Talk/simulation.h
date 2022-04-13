#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MESSAGE_LENGTH 40
#define MAX_LIST_HEAD 10

enum processState{
    READY,
    RUNNING,
    BLOCKED
};

typedef struct MSG_STRUCT{
    char msg[MAX_MESSAGE_LENGTH];
    int PIDToBeSent;
}MSG;

typedef struct PCB_STRUCT{
    int PID;
    int priority;
    enum processState state;
    MSG msgUnderPCB;
}PCB;

typedef struct SEMAPHORE_STRUCT{
    int valueSemaphore;
    List* waitingProcess;
}SEMAPHORE;

List* queueHighPriority;
List* queueMidPriority;
List* queueLowPriority;
List* waitingReceiver;
List* waitingSender;

SEMAPHORE* sem[5];

void init();
void msgToPCBsNode(PCB* pcb, int pidToBeSent, char* msg);
void replace_Newline(char* str);
void printQueue(List* List_t);
void printDetailedQueue(List* List_t);
void printRunningProcessInfo();
void printProcessInfo(PCB* process);
bool isNumber(char* str);
int countTotalProcess();
int countQueueProcess();
int PIDcounter();
void queueWithPriority(int pri, void* process);
void sendTo(char* msg, PCB* sendToPCB);



// Command 'C'
// Create a new process.
int createPCB(int pri, PCB* processNew);

// Command 'F'
// Fork current process with same message, priority.
int fork();

// Command 'K'
// Kill a process with the killingPID.
int kill(int killingPID);

// Command 'E'
// Kill current process
int exitA();

// Command 'Q'
// Singalling the current process is expired.
// Append the current process to the List.
int quantum();

// Command 'S'
int send(int sendToPID, char* msg);

// Command 'R'
int receive();

// Command 'Y'
int reply(int replyToPID, char* msg);

// Command 'N'
int semaphoreNew(int SID, int value);

// Command 'P'
int semaphoreP(int SID);

// Command 'V'
int semaphoreV(int SID);

// Command 'I'
int procInfo(int pid);

// Command 'T'
void totalInfo();