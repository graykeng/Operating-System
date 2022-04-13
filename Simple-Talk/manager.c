#include "manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Message quene

// Return length of list
int messageCount(List* list){
    int count;

    pthread_mutex_lock(&mutexQuene);{
        count = List_count(list);
    }pthread_mutex_unlock(&mutexQuene);

    return count;
}

// Add new message to the tail of the list and return -1 when failed, return 0 when success
int messageQuene(List* list, char* message){
    int zeroOrOne;
    
    pthread_mutex_lock(&mutexQuene);{
        zeroOrOne = List_append(list, message);
    }pthread_mutex_unlock(&mutexQuene);

    return zeroOrOne;
}

// Return and remove the data from the head of the list.
char* messageDequene(List* list){
    char* message;

    pthread_mutex_lock(&mutexQuene);{
        message = List_first(list);
        List_remove(list);
    }pthread_mutex_unlock(&mutexQuene);

    return message;
}

// Store message from buffer to message
char* messageStore(int numChar, char* tempMessageStorage){
    char* message = (char*)malloc(sizeof(char)*(numChar+1));
    strncpy(message, tempMessageStorage, numChar);
    message[numChar] = '\0';
	return message;
}

// Cancel
void cancel(pthread_t thread){
    pthread_cancel(thread);
}


// Signaller for WRITER and SENDER. RECEIVER will call this
void signaller(pthread_mutex_t* mutex, pthread_cond_t* cond){
    pthread_mutex_lock(mutex);{
        pthread_cond_signal(cond);
    }pthread_mutex_unlock(mutex);
}

// Lock & Wait
void locknWait(pthread_mutex_t* mutex, pthread_cond_t* cond){
    pthread_mutex_lock(mutex);{
        pthread_cond_wait(cond, mutex);
    }pthread_mutex_unlock(mutex);
}



// Error handler
void errorHandler(int listFull, char* listEmpty, int pthreadCreateFailed, int wrongAddress, int wrongSocket, int jjj, int recvfromWrong, int sendtoWrong, int bindWrong, int bindBro, int readingFailed, int i){
    // 1. == -1
    if (i == 1){
        if (listFull == -1){
            printf("Error: Enquene failed. The list is full.");
        }
    }
    // 2. == NULL
    if (i == 2){
        if (listEmpty == NULL){
            printf("Error: Dequene failed. The list is empty.");
        }
    }
    // 3. ptherad_create != 0
    if (i == 3){
        if (pthreadCreateFailed != 0){
            printf("Error: pthead create failed.");
            exit(-1);
        }
    }
    // 4. != 0
    if (i == 4){
        if (wrongAddress != 0){
            printf("Error: get address information failed.");
            exit(-1);
        }
    }
    // 5. == -1
    if (i == 5){
        if (wrongSocket == -1){
            printf("Error: socket wrong");
        }
    }
    if (i == 7){
        if (recvfromWrong == -1){
            printf("Error: recvfrom failed.");
            exit(-1);
        }
    }
    if (i == 8){
        if (sendtoWrong == -1){
            printf("Error: sendto failed.");
            exit(-1);
        }
    }
    if (i == 9){
        if (bindWrong == -1){
            close(bindBro);
            printf("Error: bind failed.");
        }
    }
    if (i == 10){
        if (readingFailed == -1){
            printf("Error: reading failed.");
            exit(-1);
        }
    }
}