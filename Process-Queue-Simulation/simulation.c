#include "simulation.h"

List* queueHighPriority;
List* queueMidPriority;
List* queueLowPriority;
List* waitingReceiver;
List* waitingSender;
List* tempList;
PCB* processInit;
PCB* processRun;
SEMAPHORE* sem[5];
int tempPID = 0;

void init(){
    // Initialize the Lists
    queueHighPriority = List_create();
    queueMidPriority = List_create();
    queueLowPriority = List_create();
    waitingReceiver = List_create();
    waitingSender = List_create();
    
    // Initialize the semaphores
    for (int i = 0; i < 5; i++){
        sem[i] = NULL;
    }

    // Initialize the initial process
    processInit = (PCB*)malloc(sizeof(PCB));
    processInit->PID = PIDcounter();
    processInit->priority = 0;
    processInit->state = RUNNING;
    msgToPCBsNode(processInit, -1, NULL);
    processRun = processInit;
    printf("processInit is created. processRun is running. Pid: %d. Priority: %d\n", processInit->PID, processInit->priority);

}

// Store message to the PCB's message node.
void msgToPCBsNode(PCB* pcb, int pidToBeSent, char* msg){
    MSG* msgNode = &pcb->msgUnderPCB;
    msgNode->PIDToBeSent = pidToBeSent;
    memset(&(msgNode->msg), 0, MAX_MESSAGE_LENGTH * sizeof(char));
    if (msg != NULL){
        strcpy(msgNode->msg, msg);
    }
}

// Replace '\n' by '\0'
void replace_Newline(char* str){
    char* t;
    t = strchr(str, '\n');
    if (t != NULL){
        *t = '\0';
    }
}

// Print all PID, priority, message in queue
void printDetailedQueue(List* List_t){
    int length = List_count(List_t);
    PCB* ptr = (PCB*)List_first(List_t);
    for (int i = 0; i < length; i++){
        printProcessInfo(ptr);
        ptr = (PCB*)List_next(List_t);
    }
}

// Print a brief processRun info
void printRunningProcessInfo(){
    printf("\nThe Running Process now is PID %d, priority %d, with message \"%s\".\n", processRun->PID, processRun->priority, processRun->msgUnderPCB.msg);
}

// Print detailed process info
void printProcessInfo(PCB* process){
    printf("*****Printing Process PID %d's Information*****\n", process->PID);
    printf("Process PID: %d\n", process->PID);
    printf("Process Priority: %d\n", process->priority);
    printf("Process State: ");
    if (process->state == READY){
        printf("READY\n");
    }
    else if (process->state == RUNNING){
        printf("RUNNING\n");
    }
    else if (process->state == BLOCKED){
        printf("BLOCKED\n");
    }
    printf("Process Message: \"%s\"\n", process->msgUnderPCB.msg);
}

// Check if the char* is containing number only
bool isNumber(char* str){
    int length, i;
    length = strlen(str);
    for (i = 0; i < length; i++){
        if (!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

// Count total processes
int countTotalProcess(){
    int totalProcess = 0;
    totalProcess += List_count(queueHighPriority);
    totalProcess += List_count(queueMidPriority);
    totalProcess += List_count(queueLowPriority);
    totalProcess += List_count(waitingSender);
    totalProcess += List_count(waitingReceiver);
    
    for (int i = 0; i < 5; i++){
        if (sem[i] != NULL){
            totalProcess += List_count(sem[i]->waitingProcess);
        }
    }
    return totalProcess;
}

// Count queue processes
int countQueueProcess(){
    int queueProcess = 0;
    queueProcess += List_count(queueHighPriority);
    queueProcess += List_count(queueMidPriority);
    queueProcess += List_count(queueLowPriority);
    return queueProcess;
}

// PID++
int PIDcounter(){
    tempPID = tempPID + 1;
    return tempPID - 1;
}

// Append the process to the priority it contains
void queueWithPriority(int pri, void* process){
    switch (pri)
        {
        case 0:
            List_append(queueHighPriority, (void*)process);
            break;
        case 1:
            List_append(queueMidPriority, (void*)process);
            break;
        case 2:
            List_append(queueLowPriority, (void*)process);
            break;
        }
}

// Set processRun to be block and make a new processRun from queue
void sendTo(char* msg, PCB* sendToPCB){
    printf("The running process is PID %d, priority %d.\n", processRun->PID, processRun->priority);
    puts("It's been blocked now.");
    printf("Message \"%s\" is sent to process PID %d now.\n", msg, sendToPCB->PID);
    processRun->state = BLOCKED;
    List_append(waitingSender, (void*)processRun);
    strcpy(processRun->msgUnderPCB.msg, msg);
    if (List_count(waitingReceiver) != 0){
        PCB* tempPtr1 = (PCB*)List_first(waitingReceiver);
        while(tempPtr1 != NULL && tempPtr1->PID != sendToPCB->msgUnderPCB.PIDToBeSent){
            if (List_next(waitingReceiver) != NULL){
                List_prev(waitingReceiver);
                tempPtr1 = (PCB*)List_next(waitingReceiver);
            }
            else {
                break;
            }
        }
        if (tempPtr1->PID == sendToPCB->PID){
            tempPtr1->state = READY;
            queueWithPriority(tempPtr1->priority, tempPtr1);
        }
    }

    if (countQueueProcess == 0){
        processInit->state = RUNNING;
        processRun = processInit;
    }
    else{
        PCB* newRunningProcess;
        if (List_count(queueHighPriority) != 0){
            List_first(queueHighPriority);
            newRunningProcess = (PCB*)List_remove(queueHighPriority);
        }
        else if (List_count(queueMidPriority) != 0){
            List_first(queueMidPriority);
            newRunningProcess = (PCB*)List_remove(queueMidPriority);
        }
        else if (List_count(queueLowPriority) != 0){
            List_first(queueLowPriority);
            newRunningProcess = (PCB*)List_remove(queueLowPriority);
        }
        newRunningProcess->state = RUNNING;
        processRun = newRunningProcess;
        printf("The next running process is running now PID %d, priority %d.\n", processRun->PID, processRun->priority);
    }
}

// Command 'C'
int createPCB(int pri, PCB* processNew){

    // Initialize processNew
    processNew = (PCB*)malloc(sizeof(PCB));
    processNew->PID = PIDcounter();
    processNew->priority = pri;
    processNew->state = READY;
    msgToPCBsNode(processNew, -1, NULL);
    
    // If the processRun is the initial process
    if (processRun->PID == 0){
        processRun->state = READY; // was RUNNING
        processNew->state = RUNNING; // was READY
        processRun = processNew;
        
    }
    else{
        queueWithPriority(pri, (void*)processNew);
    }
    return processNew->PID;
}

// Command 'F'
int fork(){
    // Check if the fork process is initial process
    if (processRun == processInit){
        return -1;
    }
    PCB* processNew = (PCB*)malloc(sizeof(PCB));
    int newPid = createPCB(processRun->priority, processNew);
    msgToPCBsNode(processNew, processRun->msgUnderPCB.PIDToBeSent, processRun->msgUnderPCB.msg);
    return newPid;
}

// Command 'K'
int kill(int killingPID){

    PCB* tempPtr1;
    PCB* tempPtr2;
    PCB* tempPtr3;
    PCB* tempPtr4;
    PCB* tempPtr5;

    if (killingPID == 0){
        if (countTotalProcess() == 0){
            free(processRun);
            return -1;
        }
        else{
            return 0;
        }
    }
    // Check if the killingPID is the running PID
    else if (processRun->PID == killingPID){
        // Need to schedule in order
        if (List_count(queueHighPriority) != 0 ){
            tempPtr1 = (PCB*)List_first(queueHighPriority);
            List_remove(queueHighPriority);
            tempPtr1->state = RUNNING;
            free(processRun);
            processRun = tempPtr1;
            //printf("new running process is %d", processRun->PID);
        }
        else if (List_count(queueMidPriority) != 0){
            tempPtr1 = (PCB*)List_first(queueMidPriority);
            List_remove(queueMidPriority);
            tempPtr1->state = RUNNING;
            free(processRun);
            processRun = tempPtr1;
        }
        else if (List_count(queueLowPriority) != 0){
            tempPtr1 = (PCB*)List_first(queueLowPriority);
            List_remove(queueLowPriority);
            tempPtr1->state = RUNNING;
            free(processRun);
            processRun = tempPtr1;
        }
        // No ready process in queue
        else{
            processInit->state = RUNNING;
            free(processRun);
            processRun = processInit;
        }
        return killingPID;
    }
    // Check if the killingPID is the initial process
    else{
        // The killingPID is not running and not initial.
        // Check where is the killingPID
        // Kill it
        tempPtr1 = (PCB*)List_first(queueHighPriority);
        tempPtr2 = (PCB*)List_first(queueMidPriority);
        tempPtr3 = (PCB*)List_first(queueLowPriority);
        tempPtr4 = (PCB*)List_first(waitingSender);
        tempPtr5 = (PCB*)List_first(waitingReceiver);
        while(true){
            if (List_count(queueHighPriority) != 0){
                if (tempPtr1 != NULL){
                    if (tempPtr1->PID == killingPID){
                        printf("Kill the process PID %d, priority %d.\n", tempPtr1->PID, tempPtr1->priority);
                        PCB* pcbRemove = (PCB*)List_remove(queueHighPriority);
                        free(pcbRemove);
                        pcbRemove = NULL;
                        return killingPID;
                    }
                    else if (tempPtr1 != NULL){
                        tempPtr1 = (PCB*)List_next(queueHighPriority);
                    }
                }
            }
            
            if (List_count(queueMidPriority) != 0){
                if (tempPtr2 != NULL){
                    if (tempPtr2->PID == killingPID){
                        printf("Kill the process PID %d, priority %d.\n", tempPtr2->PID, tempPtr2->priority);
                        PCB* pcbRemove = (PCB*)List_remove(queueMidPriority);
                        printf("PID: %d, Proirity %d\n", pcbRemove->PID, pcbRemove->priority);
                        free(pcbRemove);
                        pcbRemove = NULL;
                        return killingPID;
                    }
                    else if (tempPtr2 != NULL){
                        tempPtr2 = (PCB*)List_next(queueMidPriority);
                    }
                }
            }

            if (List_count(queueLowPriority)!= 0){
                if (tempPtr3 != NULL){
                    if (tempPtr3->PID == killingPID){
                        printf("Kill the process PID %d, priority %d.\n", tempPtr3->PID, tempPtr3->priority);
                        PCB* pcbRemove = (PCB*)List_remove(queueLowPriority);
                        free(pcbRemove);
                        pcbRemove = NULL;
                        return killingPID;
                    }
                    else if (tempPtr3 != NULL){
                        tempPtr3 = (PCB*)List_next(queueLowPriority);
                    }
                }
            }

            if (List_count(waitingSender) != 0){
                if (tempPtr4 != NULL){
                    if (tempPtr4->PID == killingPID){
                        printf("Kill the process PID %d, priority %d.\n", tempPtr4->PID, tempPtr4->priority);
                        PCB* pcbRemove = (PCB*)List_remove(waitingSender);
                        free(pcbRemove);
                        pcbRemove = NULL;
                        return killingPID;
                    }
                    else if (tempPtr4 != NULL){
                        tempPtr4 = (PCB*)List_next(waitingSender);
                    }
                }
            }

            if (List_count(waitingReceiver) != 0){
                if (tempPtr5 != NULL){
                    if (tempPtr5->PID == killingPID){
                        printf("Kill the process PID %d, priority %d.\n", tempPtr5->PID, tempPtr5->priority);
                        PCB* pcbRemove = (PCB*)List_remove(waitingReceiver);
                        free(pcbRemove);
                        pcbRemove = NULL;
                        return killingPID;
                    }
                    else if (tempPtr5 != NULL){
                        tempPtr5 = (PCB*)List_next(waitingReceiver);
                    }
                }
            }

            // semaphore's waiting queueu

            if (tempPtr1 == NULL || List_count(queueHighPriority) == 0){
                if (tempPtr2 == NULL || List_count(queueMidPriority) == 0){
                    if (tempPtr3 == NULL || List_count(queueLowPriority) == 0){
                        if (tempPtr4 == NULL || List_count(waitingSender) == 0){
                            if (tempPtr5 == NULL || List_count(waitingReceiver) == 0){
                                break;
                            }
                        }
                    }
                }
            }
        }
        return -2;
    }
}

// Command 'E'
int exitA(){
    int runningPID = processRun->PID;
    if (runningPID == 0){
        if (countTotalProcess() == 0){
            free(processRun);
            return -1;
        }
    }
    else{
        kill(runningPID);
        // nextRunningProcess();
        return runningPID;
    }
    return 0;
}

// Command 'Q'
int quantum(){
    printRunningProcessInfo();
    if (countQueueProcess() != 0){
        processRun->state = READY;
        PCB* newRunningProcess;
        PCB* tempPtr1;
        if (List_count(queueHighPriority) != 0){
            List_first(queueHighPriority);
            newRunningProcess = (PCB*)List_remove(queueHighPriority);
        }
        else if (List_count(queueMidPriority) != 0){
            List_first(queueMidPriority);
            newRunningProcess = (PCB*)List_remove(queueMidPriority);
        }
        else if (List_count(queueLowPriority) != 0){
            List_first(queueLowPriority);
            newRunningProcess = (PCB*)List_remove(queueLowPriority);
        }
        newRunningProcess->state = RUNNING;
        tempPtr1 = processRun;
        processRun = newRunningProcess;
        queueWithPriority(tempPtr1->priority, tempPtr1);
        puts("Current process goes queue. New process start running.");
        printRunningProcessInfo();
    }
    else{
        puts("There is no READY process in queue.");
        puts("Therefore, the current running process is still running.");
        printRunningProcessInfo();
        return processRun->PID;
    }
}

// Command 'S'
int send(int sendToPID, char* msg){
    PCB* tempPtr1 = (PCB*)List_first(queueHighPriority);
    PCB* tempPtr2 = (PCB*)List_first(queueMidPriority);
    PCB* tempPtr3 = (PCB*)List_first(queueLowPriority);
    PCB* tempPtr4 = (PCB*)List_first(waitingSender);
    PCB* tempPtr5 = (PCB*)List_first(waitingReceiver);
    PCB* tempPtr6;
    int i = 0;

    while(true){
        if (processRun->PID == sendToPID){
            printf("The PID you searched is the running process.\n");
            processRun->msgUnderPCB.PIDToBeSent = sendToPID;
            sendTo(msg, processRun);
            return processRun->PID;
        }
        if (List_count(queueHighPriority) != 0){
            if (tempPtr1 != NULL){
                if (tempPtr1->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr1);
                    return sendToPID;
                }
                else if (tempPtr1 != NULL){
                    tempPtr1 = (PCB*)List_next(queueHighPriority);
                }
            }
        }
        
        if (List_count(queueMidPriority) != 0){
            if (tempPtr2 != NULL){
                if (tempPtr2->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr2);
                    return sendToPID;
                }
                else if (tempPtr2 != NULL){
                    tempPtr2 = (PCB*)List_next(queueMidPriority);
                }
            }
        }

        if (List_count(queueLowPriority)!= 0){
            if (tempPtr3 != NULL){
                if (tempPtr3->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr3);
                    return sendToPID;
                }
                else if (tempPtr3 != NULL){
                    tempPtr3 = (PCB*)List_next(queueLowPriority);
                }
            }
        }

        if (List_count(waitingSender) != 0){
            if (tempPtr4 != NULL){
                if (tempPtr4->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr4);
                    return sendToPID;
                }
                else if (tempPtr4 != NULL){
                    tempPtr4 = (PCB*)List_next(waitingSender);
                }
            }
        }

        if (List_count(waitingReceiver) != 0){
            if (tempPtr5 != NULL){
                if (tempPtr5->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr5);
                    return sendToPID;
                }
                else if (tempPtr5 != NULL){
                    tempPtr5 = (PCB*)List_next(waitingReceiver);
                }
            }
        }

        if (i < 5 && sem[i] != NULL && List_count(sem[i]->waitingProcess) != 0){
            tempPtr6 = (PCB*)List_first(sem[i]->waitingProcess);
            while (tempPtr6->PID != sendToPID && tempPtr6 != NULL){
                tempPtr6 = (PCB*)List_next(sem[i]->waitingProcess);
                if (tempPtr6->PID == sendToPID){
                    processRun->msgUnderPCB.PIDToBeSent = sendToPID;
                    sendTo(msg, tempPtr6);
                    return sendToPID;
                }
            }
            i++;
        }
        if (tempPtr1 == NULL || List_count(queueHighPriority) == 0){
            if (tempPtr2 == NULL || List_count(queueMidPriority) == 0){
                if (tempPtr3 == NULL || List_count(queueLowPriority) == 0){
                    if (tempPtr4 == NULL || List_count(waitingSender) == 0){
                        if (tempPtr5 == NULL || List_count(waitingReceiver) == 0){
                            if (i = 5){
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

// Command 'R'
int receive(){
    PCB* tempPtr1 = (PCB*)List_first(waitingSender);
    while (true){
        if (List_count(waitingSender) != 0){
            if (tempPtr1 != NULL){
                if (processRun->PID == tempPtr1->msgUnderPCB.PIDToBeSent){
                    tempPtr1->msgUnderPCB.PIDToBeSent = -1;
                    printf("The message from PID %d, priority %d is \"%s\".\n", tempPtr1->PID, tempPtr1->priority, tempPtr1->msgUnderPCB.msg);
                    memset(tempPtr1->msgUnderPCB.msg, 0, sizeof(tempPtr1->msgUnderPCB.msg));
                    return tempPtr1->PID;
                }
                else if (tempPtr1 != NULL){
                    tempPtr1 = (PCB*)List_next(waitingSender);
                }
            }
        }
        if (tempPtr1 == NULL){
            break;
        }
    }
    puts("Your mailbox is empty.\n");
    printf("The running process is PID %d, priority %d.\n", processRun->PID, processRun->priority);
    puts("It's been blocked now.\n");
    processRun->state = BLOCKED;
    List_append(waitingReceiver, processRun);
    if (countQueueProcess == 0){
        processInit->state = RUNNING;
        processRun = processInit;
    }
    else{
        PCB* newRunningProcess;
        if (List_count(queueHighPriority) != 0){
            List_first(queueHighPriority);
            newRunningProcess = (PCB*)List_remove(queueHighPriority);
        }
        else if (List_count(queueMidPriority) != 0){
            List_first(queueMidPriority);
            newRunningProcess = (PCB*)List_remove(queueMidPriority);
        }
        else if (List_count(queueLowPriority) != 0){
            List_first(queueLowPriority);
            newRunningProcess = (PCB*)List_remove(queueLowPriority);
        }
        newRunningProcess->state = RUNNING;
        processRun = newRunningProcess;
        printf("The next running process is running now PID %d, priority %d.\n", processRun->PID, processRun->priority);
    }
    return 0;
}

// Command 'Y'
int reply(int replyToPID, char* msg){
    PCB* tempPtr1 = (PCB*)List_first(waitingSender);

    while (tempPtr1->PID != replyToPID && tempPtr1 != NULL){
        tempPtr1 = (PCB*)List_next(waitingSender);
    }

    if (tempPtr1 == NULL){
        return -1;
    }

    // Replying initial process
    if (replyToPID == 0){
        tempPtr1 = processInit;
        strcpy(tempPtr1->msgUnderPCB.msg, msg);
        return replyToPID;
    }
    else{
        strcpy(tempPtr1->msgUnderPCB.msg, msg);
        List_remove(waitingSender);
        tempPtr1->state = READY;
        queueWithPriority(tempPtr1->priority, tempPtr1);
        return replyToPID;
    }
}

// Command 'N'
int semaphoreNew(int SID, int value){
    // Have not yet been initialized
    if (sem[SID] == NULL){
        sem[SID] = (SEMAPHORE*)malloc(sizeof(SEMAPHORE));
        sem[SID]->valueSemaphore = value;
        sem[SID]->waitingProcess = List_create();
        return SID;
    }
    return -1;
}

// Command 'P'
int semaphoreP(int SID){
    if (sem[SID] != NULL){
        sem[SID]->valueSemaphore = sem[SID]->valueSemaphore - 1;
        if (sem[SID]->valueSemaphore >= 0){
            printf("The running process PID %d, priority %d is not blocked!\n", processRun->PID, processRun->priority);
        }
        else{
            if (processRun->PID == 0){
                return -1;
            }
            else{
                printf("The runnning process PID %d, priority %d is blocked!\n", processRun->PID, processRun->priority);
                processRun->state = BLOCKED;
                List_append(sem[SID]->waitingProcess, (void*)processRun);
                PCB* newRunningProcess;
                if (List_count(queueHighPriority) != 0){
                    List_first(queueHighPriority);
                    newRunningProcess = (PCB*)List_remove(queueHighPriority);
                }
                else if (List_count(queueMidPriority) != 0){
                    List_first(queueMidPriority);
                    newRunningProcess = (PCB*)List_remove(queueMidPriority);
                }
                else if (List_count(queueLowPriority) != 0){
                    List_first(queueLowPriority);
                    newRunningProcess = (PCB*)List_remove(queueLowPriority);
                }
                else{
                    processInit->state = RUNNING;
                    processRun = processInit;
                    return SID;
                }
                newRunningProcess->state = RUNNING;
                processRun = newRunningProcess;
            }
        }
        return SID;
    }
    return -1;
}

// Command 'V'
int semaphoreV(int SID){
    PCB* tempPtr1;
    if (sem[SID] != NULL){
        sem[SID]->valueSemaphore = sem[SID]->valueSemaphore + 1;
        if (sem[SID]->valueSemaphore >= 0){
            if (List_count(sem[SID]->waitingProcess) != 0){
                List_first(sem[SID]->waitingProcess);
                tempPtr1 = (PCB*)List_remove(sem[SID]->waitingProcess);
                printf("The process PID %d, priority %d is unblocked!\n", tempPtr1->PID, tempPtr1->priority);
                queueWithPriority(tempPtr1->priority, tempPtr1);
            }
        }
        else{
            printf("The SID's value is %d, which is < 0.\n", sem[SID]->valueSemaphore);
            printf("Therefore, there's no blocked process can use semaphoreV.\n");
        }
        return SID;
    }
    return -1;
}

// Command 'I'
int procInfo(int pid){

    PCB* tempPtr1 = (PCB*)List_first(queueHighPriority);
    PCB* tempPtr2 = (PCB*)List_first(queueMidPriority);
    PCB* tempPtr3 = (PCB*)List_first(queueLowPriority);
    PCB* tempPtr4 = (PCB*)List_first(waitingSender);
    PCB* tempPtr5 = (PCB*)List_first(waitingReceiver);
    PCB* tempPtr6;
    int i = 0;

    while(true){
        if (processRun->PID == pid){
            printf("The PID you searched is the running process.\n");
            printProcessInfo(processRun);
            return processRun->PID;
        }
        if (List_count(queueHighPriority) != 0){
            if (tempPtr1 != NULL){
                if (tempPtr1->PID == pid){
                    printProcessInfo(tempPtr1);
                    return pid;
                }
                else if (tempPtr1 != NULL){
                    tempPtr1 = (PCB*)List_next(queueHighPriority);
                }
            }
        }
        
        if (List_count(queueMidPriority) != 0){
            if (tempPtr2 != NULL){
                if (tempPtr2->PID == pid){
                    printProcessInfo(tempPtr2);
                    return pid;
                }
                else if (tempPtr2 != NULL){
                    tempPtr2 = (PCB*)List_next(queueMidPriority);
                }
            }
        }

        if (List_count(queueLowPriority)!= 0){
            if (tempPtr3 != NULL){
                if (tempPtr3->PID == pid){
                    printProcessInfo(tempPtr3);
                    return pid;
                }
                else if (tempPtr3 != NULL){
                    tempPtr3 = (PCB*)List_next(queueLowPriority);
                }
            }
        }

        if (List_count(waitingSender) != 0){
            if (tempPtr4 != NULL){
                if (tempPtr4->PID == pid){
                    printProcessInfo(tempPtr4);
                    return pid;
                }
                else if (tempPtr4 != NULL){
                    tempPtr4 = (PCB*)List_next(waitingSender);
                }
            }
        }

        if (List_count(waitingReceiver) != 0){
            if (tempPtr5 != NULL){
                if (tempPtr5->PID == pid){
                    printProcessInfo(tempPtr5);
                    return pid;
                }
                else if (tempPtr5 != NULL){
                    tempPtr5 = (PCB*)List_next(waitingReceiver);
                }
            }
        }

        if (i < 5 && sem[i] != NULL && List_count(sem[i]->waitingProcess) != 0){
            tempPtr6 = (PCB*)List_first(sem[i]->waitingProcess);
            while (tempPtr6->PID != pid && tempPtr6 != NULL){
                tempPtr6 = (PCB*)List_next(sem[i]->waitingProcess);
                if (tempPtr6->PID == pid){
                    printProcessInfo(tempPtr6);
                    return pid;
                }
            }
            i++;
        }
        if (tempPtr1 == NULL || List_count(queueHighPriority) == 0){
            if (tempPtr2 == NULL || List_count(queueMidPriority) == 0){
                if (tempPtr3 == NULL || List_count(queueLowPriority) == 0){
                    if (tempPtr4 == NULL || List_count(waitingSender) == 0){
                        if (tempPtr5 == NULL || List_count(waitingReceiver) == 0){
                            if (i = 5){
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

// Command 'T'
void totalInfo(){
    printRunningProcessInfo();
    printf("\nThere are %d processes in queue of the HIGH priority(0): \n", List_count(queueHighPriority));
    printDetailedQueue(queueHighPriority);
    printf("\nThere are %d processes in queue of the MIDDLE priority(1): \n", List_count(queueMidPriority));
    printDetailedQueue(queueMidPriority);
    printf("\nThere are %d processes in queue of the LOW priority(2): \n", List_count(queueLowPriority));
    printDetailedQueue(queueLowPriority);
    printf("\nThere are %d processes in the SEND waiting queue: \n", List_count(waitingSender));
    printDetailedQueue(waitingSender);
    printf("\nThere are %d processes in the RECEIVE waiting queue: \n", List_count(waitingReceiver));
    printDetailedQueue(waitingReceiver);
    for (int i = 0; i < 5; i++){
        if (sem[i] != NULL){
            printf("\nThere are %d processes in SEMAPHORE ID: %d, VALUE: %d, blocked queue: \n", List_count(sem[i]->waitingProcess), i, sem[i]->valueSemaphore);
            printDetailedQueue(sem[i]->waitingProcess);
        }
    }
    puts("");
}