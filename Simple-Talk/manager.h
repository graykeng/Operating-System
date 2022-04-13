#pragma once
#include "list.h"
#include <pthread.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t mutexQuene = PTHREAD_MUTEX_INITIALIZER;
// static struct initReceiverReturn{
//     char* localPort;
//     List* list;
// };
// static struct initSenderReturn{
//     char* localName;
//     char* remotePort;
//     List* list;
// };

// Message quene
int messageCount(List* list);
int messageQuene(List* list, char* message);
char* messageDequene(List* list);
char* messageStore(int numChar, char* tempMessageStorage);

// Cancel
void cancel(pthread_t thread);


// Signaller
void signaller(pthread_mutex_t* mutex, pthread_cond_t* cond);
void locknWait(pthread_mutex_t* mutex, pthread_cond_t* cond);

void deallocateReceiver();
void deallocateSender();
void deallocateReader();
void deallocateWriter();

// Error Handler
void errorHandler(int listFull, char* listEmpty, int pthreadCreateFailed, int wrongAddress, int wrongSocket, int jjj, int recvfromWrong, int sendtoWrong, int bindWrong, int bindBro, int readingFailed, int i);