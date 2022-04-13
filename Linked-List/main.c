#include <stdio.h>
#include "list.c"
bool comparator(void* pItem, void* pComparisonArg){
    int* a = pItem;
    int* b = pComparisonArg;

    if (*a == *b){
        return 1;
    }
    else{
        return 0;
    }
}

void func(void* pItem){
    pItem = NULL;
}

void print_the_whole_list(List* pList){
    Node* temp = pList->current;
    if (pList->length == 0){
        printf("The list is empty.\n");
    }
    else{
        printf("The whole list is: {");
        int* current_print = List_first(pList);
        for (int i = 0; i < pList->length; i++){
            if (i == pList->length-1){
                printf("%d", *current_print);
                List_next(pList);
            }
            else{
                printf("%d, ", *current_print);
                current_print = List_next(pList);
            }
        }
        List_prev(pList);
        printf("}\n");
        pList->current = temp;
    }
}

void test_list_create(){
    printf("***************TEST_CREATE_LIST***************\n");
    
    List* array_List[LIST_MAX_NUM_HEADS];
    for(int i = 0; i<LIST_MAX_NUM_HEADS; i++){
        array_List[i] = List_create();
    }
    if(array_List[LIST_MAX_NUM_HEADS-1] != NULL){
        printf("The max number of lists created successfully!\n");
    }
    List* list = List_create();
    if(list != NULL){
        printf("Failed.\n");
    }
    else{
        printf("No list head left!\n");
    }
}

void test_list_count(){
    printf("***************TEST_LIST_COUNT***************\n");
    // list create, size max
    int array_saves_numbers[LIST_MAX_NUM_NODES];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_append(list1, &array_saves_numbers[i]);
    }
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));
    // remove half numbers from the list
    for (int i = LIST_MAX_NUM_NODES; i > LIST_MAX_NUM_NODES/2; i--){
        List_remove(list1);
        List_prev(list1);
    }
    printf("After remove the last half of the nodes, ");
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));
}

void test_list_first(){
    printf("***************TEST_LIST_FIRST***************\n");
    // list create, size 10
    int array_saves_numbers[LIST_MAX_NUM_NODES/10];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES/10; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES/10; i++){
        List_append(list1, &array_saves_numbers[i]);
    }
    int* x = List_curr(list1);
    print_the_whole_list(list1);
    printf("Before using List_first, the current is: %d\n", *x);
    List_first(list1);
    x = List_curr(list1);
    printf("After using List_first, the current is: %d\n", *x);
}

void test_list_last(){
    printf("***************TEST_LIST_LAST***************\n");
    // list create, size 10
    int array_saves_numbers[LIST_MAX_NUM_NODES/10];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES/10; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES/10; i++){
        List_prepend(list1, &array_saves_numbers[i]);
    }
    int* x = List_curr(list1);
    print_the_whole_list(list1);
    printf("Before using List_last, the current is: %d\n", *x);
    List_last(list1);
    x = List_curr(list1);
    printf("After using List_last, the current is: %d\n", *x);
}

void test_list_next(){
    printf("***************TEST_LIST_NEXT***************\n");
    List* list1 = List_create();
    int arr[5] = {0, 1, 2, 3, 4};
    List_append(list1, &arr[0]); // {0}
    List_prepend(list1, &arr[1]); // {1, 0}
    List_append(list1, &arr[2]); // {1, 0, 2}
    List_prepend(list1, &arr[3]); // {3, 1, 0, 2}
    List_append(list1, &arr[4]); // {3, 1, 0, 2, 4}
    print_the_whole_list(list1);
    int* x = List_first(list1);
    printf("The first data in the list is: %d\n", *x);
    x = List_next(list1);
    printf("The next data in the list is: %d\n", *x);
    x = List_next(list1);
    printf("The next data in the list is: %d\n", *x);
    x = List_next(list1);
    printf("The next data in the list is: %d\n", *x);
    x = List_next(list1);
    printf("The next data in the list is: %d\n", *x);
    x = List_curr(list1);
    printf("The last data in the list is: %d\n", *x);
}

void test_list_prev(){
    printf("***************TEST_LIST_PREV***************\n");
    List* list1 = List_create();
    int arr[5] = {0, 1, 2, 3, 4};
    List_append(list1, &arr[0]); // {0}
    List_prepend(list1, &arr[1]); // {1, 0}
    List_append(list1, &arr[2]); // {1, 0, 2}
    List_prepend(list1, &arr[3]); // {3, 1, 0, 2}
    List_append(list1, &arr[4]); // {3, 1, 0, 2, 4}
    print_the_whole_list(list1);
    int* x = List_last(list1);
    printf("The last data in the list is: %d\n", *x);
    x = List_prev(list1);
    printf("The prev data in the list is: %d\n", *x);
    x = List_prev(list1);
    printf("The prev data in the list is: %d\n", *x);
    x = List_prev(list1);
    printf("The prev data in the list is: %d\n", *x);
    x = List_prev(list1);
    printf("The prev data in the list is: %d\n", *x);
    x = List_curr(list1);
    printf("The first data in the list is: %d\n", *x);
}

void test_list_curr(){
    printf("***************TEST_LIST_CURR***************\n");
    List* list1 = List_create();
    int arr[5] = {0, 1, 2, 3, 4};
    List_append(list1, &arr[0]); // {0}
    List_prepend(list1, &arr[1]); // {1, 0}
    List_append(list1, &arr[2]); // {1, 0, 2}
    List_prepend(list1, &arr[3]); // {3, 1, 0, 2}
    List_append(list1, &arr[4]); // {3, 1, 0, 2, 4}
    print_the_whole_list(list1);
    List_first(list1);
    int* x = List_curr(list1);
    printf("The first data in the list is: %d\n", *x);
    List_next(list1);
    List_next(list1);
    x = List_curr(list1);
    printf("The third data in the list is: %d\n", *x);
}

void test_list_insert_after(){
    printf("***************TEST_LIST_INSERT_AFTER***************\n");
    // list create, size max
    int array_saves_numbers[LIST_MAX_NUM_NODES];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_insert_after(list1, &array_saves_numbers[i]);
    }
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));

}

void test_list_insert_before(){
    printf("***************TEST_LIST_INSERT_BEFORE***************\n");
    // list create, size max
    int array_saves_numbers[LIST_MAX_NUM_NODES];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_insert_before(list1, &array_saves_numbers[i]);
    }
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));
}

void test_list_append(){
    printf("***************TEST_LIST_APPEND***************\n");
    // list create, size max
    int array_saves_numbers[LIST_MAX_NUM_NODES];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_append(list1, &array_saves_numbers[i]);
    }
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));
}

void test_list_prepend(){
    printf("***************TEST_LIST_PREPEND***************\n");
    // list create, size max
    int array_saves_numbers[LIST_MAX_NUM_NODES];
    List* list1 = List_create();
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        array_saves_numbers[i] = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++){
        List_prepend(list1, &array_saves_numbers[i]);
    }
    print_the_whole_list(list1);
    printf("The length of the list is: %d\n", List_count(list1));
}

void test_list_remove(){
    printf("***************TEST_LIST_REMOVE***************\n");
    List* list1 = List_create();
    int arr[5] = {0, 1, 2, 3, 4};
    List_append(list1, &arr[0]); // {0}
    List_prepend(list1, &arr[1]); // {1, 0}
    List_append(list1, &arr[2]); // {1, 0, 2}
    List_prepend(list1, &arr[3]); // {3, 1, 0, 2}
    List_append(list1, &arr[4]); // {3, 1, 0, 2, 4}
    print_the_whole_list(list1);
    int* x = List_curr(list1);
    printf("The current data is: %d\n", *x);

    List_remove(list1);
    print_the_whole_list(list1);

    if (list1->current == NULL){
        printf("the curren data is out of bound!\n");
    }
    else{
        x = List_curr(list1);
        printf("The current data is: %d\n", *x);
    }
}

void test_list_trim(){
    printf("***************TEST_LIST_TRIM***************\n");
    List* list1 = List_create();
    int arr[5] = {0, 1, 2, 3, 4};
    List_append(list1, &arr[0]); // {0}
    List_prepend(list1, &arr[1]); // {1, 0}
    List_append(list1, &arr[2]); // {1, 0, 2}
    List_prepend(list1, &arr[3]); // {3, 1, 0, 2}
    List_append(list1, &arr[4]); // {3, 1, 0, 2, 4}
    print_the_whole_list(list1);
    List_trim(list1);
    print_the_whole_list(list1);
    List_trim(list1);
    print_the_whole_list(list1);
    List_trim(list1);
    print_the_whole_list(list1);
    List_trim(list1);
    print_the_whole_list(list1);
    List_trim(list1);
    print_the_whole_list(list1);

}

void test_list_concat(){
    printf("***************TEST_LIST_CONCAT***************\n");
    List* list1 = List_create();
    List* list2 = List_create();

    int a[5] = {0, 1, 2, 3, 4};

    List_append(list1, &a[0]);
    List_append(list1, &a[1]);
    List_append(list2, &a[2]);
    List_append(list2, &a[3]);
    List_append(list2, &a[4]);

    print_the_whole_list(list1);
    print_the_whole_list(list2);

    List_concat(list1, list2);

    print_the_whole_list(list1);
}

void test_list_free(){
    printf("***************TEST_LIST_FREE***************\n");
    // list create, size 10
    List* list1 = List_create();
    List* list2 = List_create();
    List* list3 = List_create();
    List* list4 = List_create();
    List* list5 = List_create();
    List* list6 = List_create();
    List* list7 = List_create();
    List* list8 = List_create();
    List* list9 = List_create();
    List* list10 = List_create();
    printf("Created 10 lists!\n");

    List* list11 = List_create();
    if(list11 == NULL){
        printf("List11 is NULL before free().\n");
    }
    else{
        printf("List11 is created before free(). ???????\n");
    }
    
    FREE_FN ii = func;
    List_free(list1, ii);
    List* list12 = List_create();
    if(list12 == NULL){
        printf("List12 is NULL after free(). XXXXXXXXXXXXX\n");
    }
    else{
        printf("Created List12 list successfully\n");
    }
}

void test_list_search(){
    printf("***************TEST_LIST_SEARCH***************\n");
    COMPARATOR_FN ii = comparator;
    List* list1 = List_create();
    List* list2 = List_create();

    int a[1] = {1};
    int b[1] = {2};
    int c[1] = {3};
    int d[1] = {4};
    int e[1] = {5};

    List_append(list1, a);
    List_append(list1, b);
    List_append(list1, c);
    List_append(list2, d);
    List_append(list2, e);

    printf("Printing list1: ");
    print_the_whole_list(list1);

    printf("Printing list2: ");
    print_the_whole_list(list2);

    List_first(list1);
    
    printf("Searching for (3)...\n");

    int* temp;
    temp = List_search(list1, ii, c);

    if(temp != NULL){
        printf("The 3 had been found in list1!\n");
    }
    else{
        printf("Searching failed.");
    }
    temp = List_search(list2, ii, c);
    if(temp != NULL){
        printf("The 3 had been found in list2!\n");
    }
    else{
        printf("Searching failed.\n");
    }
}
    

int main(){
    int num;
    printf("Number's guidance: \n1: test_list_create  2: test_list_count  3: test_list_first  4: test_list_last\n5: test_list_next  6: test_list_prev  7: test_list_curr  8: test_list_insert_after\n9: test_list_insert_before  10: test_list_append  11: test_list_prepend  12: test_list_remove\n13: test_list_trim  14: test_list_concat  15: test_list_free  16: test_list_search\n");
    printf("Type the according number of the test you want: ");
    scanf("%d", &num);
    switch(num){
        case 1: test_list_create(); break;
        case 2: test_list_count(); break;
        case 3: test_list_first(); break;
        case 4: test_list_last(); break;
        case 5: test_list_next(); break;
        case 6: test_list_prev(); break;
        case 7: test_list_curr(); break;
        case 8: test_list_insert_after(); break;
        case 9: test_list_insert_before(); break;
        case 10: test_list_append(); break;
        case 11: test_list_prepend(); break;
        case 12: test_list_remove(); break;
        case 13: test_list_trim(); break;
        case 14: test_list_concat(); break;
        case 15: test_list_free(); break;
        case 16: test_list_search(); break;
        default: printf("\nType 1-16 only!\n");
    }
    printf("Test End!!\n");
    return 0;
}

