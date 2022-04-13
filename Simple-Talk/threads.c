#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>


// #include "list.c"
#include "list.h"
#include "manager.h"
#include "threads.h"

// Limit of message length for IPv4 protocol
#define MAXMESSAGELENGTH 65507

// List
void func (void* pItem){
    pItem = NULL;
}

bool exiting = false;

// Threads
static pthread_t threadReceiver;
static pthread_t threadSender;
static pthread_t threadWriter;
static pthread_t threadReader;

// Mutex & Conds
static pthread_mutex_t mutexWriter = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexSender = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condWriter = PTHREAD_COND_INITIALIZER;
static pthread_cond_t condSender = PTHREAD_COND_INITIALIZER;

// Variables
static char* localPort;
static char* remotePort;
static char* remoteHostname;
static char* messageSender;
static char* messageWriter;
static char* messageReader;
static struct addrinfo *server;
static struct addrinfo *server2;

static void* threadLoopReceiver(void* arg){
    struct addrinfo info, *server, *pth;
    int errorCheckerGAI;
    int errorCheckerBind;
    int errorCheckerQuene;
    char tempMessageStorage[MAXMESSAGELENGTH];
    int numChar;
    socklen_t addr_len;
    struct sockaddr_in rPort;
	int socketInt;

    memset(&info,0,sizeof(info));

	List* list = (List*)arg;
	
    // Setting up the addrinfo
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_DGRAM;
    info.ai_flags = AI_PASSIVE;

    errorCheckerGAI = getaddrinfo(NULL, localPort, &info, &server);
    errorHandler(0, 0, 0, errorCheckerGAI, 0, 0, 0, 0, 0, 0, 0, 4);
    
    // Setting up a socket
    pth = server;
    while(pth != NULL){
        socketInt = socket(pth->ai_family, pth->ai_socktype, pth->ai_protocol);
        errorHandler(0, 0, 0, 0, socketInt, 0, 0, 0, 0, 0, 0, 5);
        if(socketInt == -1){
            continue;
        }
        errorCheckerBind = bind(socketInt, pth->ai_addr, pth->ai_addrlen);
        errorHandler(0, 0, 0, 0, 0, 0, 0, 0, errorCheckerBind, socketInt, 0, 9);
        if(errorCheckerBind == -1){
            continue;
        }
        break;
    }
    if (pth == NULL){
        printf("Error: bind socket failed.");
        exit(-1);
    }
    freeaddrinfo(server);

    while(1){
		//printf("[RECEIVER]start loop\n");
        int i = 0;
        {
            i++;
            memset(tempMessageStorage, 0, MAXMESSAGELENGTH);

            addr_len = sizeof(rPort);
            numChar = recvfrom(socketInt, tempMessageStorage, MAXMESSAGELENGTH, 0, (struct sockaddr* ) NULL, NULL);
			if(exiting) break;
            errorHandler(0, 0, 0, 0, 0, 0, numChar, 0, 0, 0, 0, 7);
			
			//printf("[RECEIVER]receive %d bytes: %s\n", numChar, tempMessageStorage);
			
			tempMessageStorage[numChar] = 0;
			if(!strcmp(tempMessageStorage, "!\n")) {
				exiting = true;
				signaller(&mutexWriter, &condWriter);
				break;
			}

            char* messageReceiver = messageStore(numChar, tempMessageStorage);

            errorCheckerQuene = messageQuene(list, messageReceiver);
            errorHandler(errorCheckerQuene, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
          
        }
        signaller(&mutexWriter, &condWriter);
		//printf("[RECEIVER]end loop\n");
    }
	//printf("[RECEIVER]exit\n");
	
	cancel(threadSender);
    cancel(threadReader);
}

static void* threadLoopSender(void* arg){
    struct addrinfo info, *pth;
    int errorCheckerGAI;
    int numChar;
	int socketInt;
	
	List* list = (List*)arg;

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_DGRAM;

    errorCheckerGAI = getaddrinfo(remoteHostname, remotePort, &info, &server2);
    errorHandler(0, 0, 0, errorCheckerGAI, 0, 0, 0, 0, 0, 0, 0, 4);

	if(server2->ai_family != AF_INET) {
		printf("ipv4 %d\n", server2->ai_family);
	}
    pth = server2;
    while(pth != NULL){
        socketInt = socket(pth->ai_family, pth->ai_socktype, pth->ai_protocol);
        errorHandler(0, 0, 0, 0, socketInt, 0, 0, 0, 0, 0, 0, 5);
        if(socketInt == -1){
            continue;
        }
        break;
    }
    // errorHandler(0, 0, 0, 0, 0, pth, 0, 0, 0, 0, 0, 6);
    if (pth == NULL){
        printf("Error: bind socket failed.");
        exit(-1);
    }

    while(1){
		//printf("[SENDER]start loop\n");
        locknWait(&mutexSender, &condSender);
        int i = 0;
        do{
            i++;
            messageSender = messageDequene(list);
            errorHandler(0, messageSender, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2);
            if (messageSender == NULL){
                break;
            }

            numChar = sendto(socketInt, messageSender, strlen(messageSender), 0, pth->ai_addr, pth->ai_addrlen);
			//printf("[SENDER]send %d %s\n", numChar, messageSender);
			
            free(messageSender);
            messageSender = NULL;
        }while(messageCount(list)!= 0);
		
		if(exiting) break;
		//printf("[SENDER]end loop\n");
    }
	//printf("[SENDER]exit\n");
    return NULL;
}

static void* threadLoopWriter(void* arg){
    List* list = (List*)arg;
    while(1){
		//printf("[WRITER]start loop\n");
        locknWait(&mutexWriter, &condWriter);
		if(exiting) break;
        int i = 0;
        while(messageCount(list) != 0){
            i++;
            
            messageWriter = messageDequene(list);
            if(messageWriter == NULL){
                errorHandler(0, messageWriter, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2);
                break;
            }

            int printer = printf("%s", messageWriter);
            errorHandler(0, 0, 0, 0, 0, 0, printer, 0, 0, 0, 0, 7);

			//printf("[WRITER] write %d %s\n", printer, messageWriter);
            // Detect "!"
            if(!strcmp(messageWriter, "!\n")){
                free(messageWriter);
                messageWriter = NULL;
                return NULL;
            }

            free(messageWriter);
            messageWriter = NULL;
        }
		//printf("[WRITER]end loop\n");
    }
	//printf("[WRITER]exit\n");
    return NULL;
}

static void* threadLoopReader(void* arg){
	List* list = (List*)arg;
	
    while(1){
		//printf("[READER]start loop\n");
        char tempMessageStorage[MAXMESSAGELENGTH];
        int i = 0;
        int numChar;
        
		if(!fgets(tempMessageStorage, sizeof(tempMessageStorage), stdin)) break;
		
		
		numChar = strlen(tempMessageStorage);
		
		//printf("[READER] read %d %s\n", numChar, tempMessageStorage);
		
		errorHandler(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, numChar, 10);
		char* messageReader = messageStore(numChar, tempMessageStorage);
		
		// Add the message to the list
        int queneErrorChecker = messageQuene(list, messageReader);
        errorHandler(queneErrorChecker, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
			
       
        signaller(&mutexSender, &condSender);
		
		if(!strcmp(tempMessageStorage, "!\n")) break;
		//printf("[READER]end loop\n");
    }
	
	//printf("[READER]exit\n");
	exiting = true;
	signaller(&mutexSender, &condSender);
    cancel(threadReceiver);
    cancel(threadWriter);
    return NULL;
}


// Initiate four threads
// Init RECEIVER and return the local port and list
void initReceiver(char* LP, List* l){
    localPort = LP;
    
    int receiverPthread = pthread_create(&threadReceiver, NULL, threadLoopReceiver, l);
    errorHandler(0, 0, receiverPthread, 0, 0, 0, 0, 0, 0, 0, 0, 3);
}

// Init SENDER and return the local name, remote port and list
void initSender(char* RN, char* RP, List* l){
    remoteHostname = RN;
    remotePort = RP;

    int senderPthread = pthread_create(&threadSender, NULL, threadLoopSender, l);
    errorHandler(0, 0, senderPthread, 0, 0, 0, 0, 0, 0, 0, 0, 3);
}

// Init READER
void initReader(List* l){

    int readerPthread = pthread_create(&threadReader, NULL, threadLoopReader, l);
    errorHandler(0, 0, readerPthread, 0, 0, 0, 0, 0, 0, 0, 0, 3);
}

// Init WRITER
void initWriter(List* l){

    int writerPthread = pthread_create(&threadWriter, NULL, threadLoopWriter, l);
    errorHandler(0, 0, writerPthread, 0, 0, 0, 0, 0, 0, 0, 0, 3);
}

// Deallocate Everything
// Deallocate RECEIVER
void deallocateReceiver(){
    freeaddrinfo(server);
    //close(socketInt);
    pthread_join(threadReceiver, NULL);
}

// Deallocate SENDER
void deallocateSender(){
    free(messageSender);
    messageSender = NULL;
    freeaddrinfo(server);
    //close(socketInt);
    pthread_join(threadSender, NULL);
}

// Deallocate READER
void deallocateReader(){
    pthread_join(threadReader, NULL);
}

// Deallocate WRITER
void deallocateWriter(){
    free(messageWriter);
    messageWriter = NULL;
    pthread_join(threadWriter, NULL);
}


int main (int argc, char * argv[]){
    if (argc != 4){
        printf("Enter ./s-talk [local port number] [remote hostname] [remote port number]");
        return -1;
    }
    char* localPort = argv[1];
    char* remoteHostname = argv[2];
    char* remotePort = argv[3];

    List* list1 = List_create();
	List* list2 = List_create();

    initReader(list1);
    initSender(remoteHostname, remotePort, list1);
    initReceiver(localPort, list2);
    initWriter(list2);

    deallocateReader();
    deallocateSender();
    deallocateReceiver();
    deallocateWriter();
    FREE_FN ff = func;
    List_free(list1, ff);
	List_free(list2, ff);
    return 0;
}
