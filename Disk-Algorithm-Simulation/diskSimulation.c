#include "diskSimulation.h"

track* Track_create(){
    track* t = (track*)malloc(sizeof(track));
    t->value = -1;
    t->orderBefore = -1;
    t->orderAfter = -1;
    return t;
}



void init(){
    twoHundred = List_create();
    original = List_create();
    originalBackup = List_create();
    FCFS = List_create();
    SCAN = List_create();
    CSCAN = List_create();
}

void fillOriginalList(int MAX){   
    List_first(original);
    srand((unsigned) time(&t));
    for (int i = 0; i < MAX; i++){
        //srand((unsigned) time(&t));
        int ran = rand()%MAX_TRACK;
        track* t = Track_create();
        t->value = ran;
        t->orderBefore = i;
        List_append(original, t);
    }
}

void listPrinter(List* list){
    if (List_count(list)!= 0){
        track* item = List_first(list);
        printf("{");
        for (int i = 0; i < List_count(list); i++){
            printf("%d ", item->value);
            item = List_next(list);
        }
        printf("}\n");
    }
}

void traversedPrinterFCFS(){
    track* temp = (track*)List_first(FCFS);
    int curr = 0;
    int next = 0;
    int diff = 0;
    int result = 0;
    for (int i = 0; i < FCFS->count-1; i++){
        int curr = temp->value;
        temp = (track*)List_next(FCFS);
        int next = temp->value;
        diff = abs(next - curr);
        result += diff;
    }
    printf("The tracks traversed by the r/w are for FCFS is: %d\n", result);
}

void traversedPrinterSCAN(){
    track* temp = (track*)List_first(SCAN);
    track* temp2 = (track*)List_last(SCAN);
    int head = temp->value;
    int last = temp2->value;
    int traversed = head + last;
    printf("The tracks traversed by the r/w are for SCAN is: %d\n", traversed);
}

void longestDelaySCAN(){
    track* temp = (track*)List_first(SCAN);
    int result = 0;
    track* result_t = Track_create();
    for (int i = 0; i < SCAN->count; i++){
        int orderB = temp->orderBefore;
        int orderA = temp->orderAfter;
        int diff = orderA-orderB;
        if (diff > result){
            result = diff;
            result_t->value = temp->value;
            result_t->orderAfter = temp->orderAfter;
            result_t->orderBefore = temp->orderBefore;
        }
        temp = (track*)List_next(SCAN);
    }
    printf("The longest delayed track in SCAN algorithm is in position %d in the original, the value is %d and delayed %d\n", result_t->orderBefore, result_t->value, result);
}

void averageDelaySCAN(){
    track* temp = (track*)List_first(SCAN);
    int result = 0;
    int num = 0;
    float re;
    for (int i = 0; i < SCAN->count; i++){
        int orderB = temp->orderBefore;
        int orderA = temp->orderAfter;
        int diff = orderA-orderB;
        if (diff > 0){
            result += diff;
            num++;
        }
        temp = (track*)List_next(SCAN);
    }
    re = result/num;
    printf("The average delay for SCAN is %f\n", re);
}

void traversedPrinterCSCAN(){
    track* temp = (track*)List_first(CSCAN);
    track* temp2 = (track*)List_last(CSCAN);
    int head = temp->value;
    int last = temp2->value;
    int traversed = head + (MAX_TRACK - last);
    printf("The tracks traversed by the r/w are for CSCAN is: %d\n", traversed);
}

void longestDelayCSCAN(){
    track* temp = (track*)List_first(CSCAN);
    int result = 0;
    track* result_t = Track_create();
    for (int i = 0; i < SCAN->count; i++){
        int orderB = temp->orderBefore;
        int orderA = temp->orderAfter;
        int diff = orderA-orderB;
        if (diff > result){
            result = diff;
            result_t->value = temp->value;
            result_t->orderAfter = temp->orderAfter;
            result_t->orderBefore = temp->orderBefore;
        }
        temp = (track*)List_next(CSCAN);
    }
    printf("The longest delayed track in CSCAN algorithm is in position %d in the original, the value is %d and delayed %d\n", result_t->orderBefore, result_t->value, result);
}

void averageDelayCSCAN(){
    track* temp = (track*)List_first(CSCAN);
    int result = 0;
    int num = 0;
    float re;
    for (int i = 0; i < CSCAN->count; i++){
        int orderB = temp->orderBefore;
        int orderA = temp->orderAfter;
        int diff = orderA-orderB;
        if (diff > 0){
            result += diff;
            num++;
        }
        temp = (track*)List_next(CSCAN);
    }
    re = result/num;
    printf("The average delay for CSCAN is %f\n", re);
}

void makeFCFS(List* copyTo, List* toBeCopy){
    track* item = (track*)List_first(toBeCopy);
    int index = 0;
    for (int i = 0; i < List_count(toBeCopy); i++){
        item->orderAfter = index;
        List_append(copyTo, item);
        index++;
        item = (track*)List_next(toBeCopy);
    }
}

void makeSCAN(){
    track* temp = (track*)List_first(original);
    int head = temp->value;
    int index = 0;

    while(List_count(SCAN) < List_count(original)){
        while(true){
            while(true){
                if (temp->value - head == 0){
                    temp->orderAfter = index;
                    List_append(SCAN, temp);
                    index++;
                }
                if (original->pCurrentNode->pNext == NULL){
                    temp = (track*)List_first(original);
                    break;
                }
                else{
                    temp = (track*)List_next(original);
                }
            }

            if (head == 0){
                head = temp->value+1;
                break;
            }
            else{
                head--;
            }
        }
        while(true){
            while(true){
                if (temp->value - head == 0){
                    temp->orderAfter = index;
                    List_append(SCAN, temp);
                    index++;
                }
                if (original->pCurrentNode->pNext == NULL){
                    temp = (track*)List_first(original);
                    break;
                }
                else{
                    temp = (track*)List_next(original);
                }
            }

            if (head == 199){
                break;
            }
            else{
                head++;
            }
        }
    }
}

void makeCSCAN(){
    track* temp = (track*)List_first(original);
    int head = temp->value;
    int headStatic = temp->value;
    bool firstTimeZero = true;
    int index = 0;

    while(List_count(CSCAN) < List_count(original)){
        while(true){
            while(true){
                if (temp->value - head == 0){
                    temp->orderAfter = index;
                    List_append(CSCAN, temp);
                    index++;
                }
                if (original->pCurrentNode->pNext == NULL){
                    temp = (track*)List_first(original);
                    break;
                }
                else{
                    temp = (track*)List_next(original);
                }
            }

            if (head == 0){
                if (firstTimeZero){
                    head = 199;
                    firstTimeZero = false;
                }
                else{
                    break;
                }
            }
            else if (firstTimeZero == false && head == headStatic+1){
                break;
            }
            else{
                head--;
            }
        }
    }
}