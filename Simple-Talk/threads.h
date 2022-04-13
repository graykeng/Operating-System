#pragma once
#include "list.h"
#include "manager.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

static void* threadLoopReceiver();
static void* threadLoopSender();
static void* threadLoopWriter();
static void* threadLoopReader();

// Initiate
void initReceiver(char* localPort, List* list);

void initSender(char* localName, char* remotePort, List* list);

void initReader(List* list);

void initWriter(List* list);

