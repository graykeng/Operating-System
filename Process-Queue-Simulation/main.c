#include "list.h"
#include "simulation.h"

int main() {
    puts("Process Scheduling Simulation\n");
    puts("                   -----List of Commands-----");
    puts("C: Create        F: Fork          K: Kill          E: Exit");
    puts("Q: Quantum       S: Send          R: Receive       Y: Reply");
    puts("N: New Semaphore P: Semaphore P   V: Semaphore V   I: Procinfo");
    puts("T: Totalinfo     !: Quit");

    char cmd[MAX_MESSAGE_LENGTH];
    char temp[MAX_LIST_HEAD];
    char tempMsg[MAX_MESSAGE_LENGTH];
    int return_pid;
    PCB* processNew;

    int quit = 1;
    init();
    while(quit){
        puts("=======================================");
        printf("Enter the Command: ");
        fgets(cmd, MAX_MESSAGE_LENGTH, stdin);
        if (strlen(cmd) != 2){
            puts("You can only enter one char!");
            continue;
        }
        cmd[0] = toupper(cmd[0]);
        switch (cmd[0])
        {
        case 'C':
            printf("*****Running Create*****\n");
            printf("Enter the priority(0, 1, 2): ");
            while(true){
                fgets(temp, MAX_MESSAGE_LENGTH, stdin);
                if (temp[1] != '\n'||(temp[0] != '0' ^ temp[0] != '1' ^ temp[0] != '2')){
                    if (temp[0] == '\n'){
                        puts("Enter something!");
                    }
                    else{
                        puts("You can only enter 0, 1, 2!");
                    }
                    printf("Enter the priority(0, 1, 2): ");
                }
                else { break; }
            }
            replace_Newline(temp);
            int PID_CREATED = createPCB(atoi(temp), processNew);
            printf("New process created. PID: %d. Priority: %d.\n", PID_CREATED, atoi(temp));
            break;
        case 'F':
            printf("*****Running Fork*****\n");
            return_pid = fork();
            if(return_pid == -1){
                puts("Forking failed.");
            }else{
                printf("Forking successfully. New Process PID: %d.\n", return_pid);
            }
            break;
        case 'K':
            printf("*****Running Kill*****\n");
            printf("Enter the PID you want to kill: ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the PID you want to kill: ");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the PID you want to kill: ");
                }
                else {
                    break;
                }
            }
            return_pid = kill(atoi(temp));
            if (return_pid == -1){
                puts("Initial process has been killed. The program has been terminated.\n");
                exit(0);
            }
            else if (return_pid == 0){
                printf("Fail to kill the initial process. There is other process running.\n");
            }
            else if (return_pid == -2){
                printf("The PID %s does not exist.\nFailed to kill the process with the invalid PID.\n", temp);
            }
            else{
                printf("Killed the PID %d successfully.\n", return_pid);
            }
            break;
        case 'E':
            printf("*****Running Exit*****\n");
            return_pid = exitA();
            if (return_pid == -1){
                printf("Initial process has been killed. The program has been terminated.\n");
                exit(0);
            }
            else if (return_pid == 0){
                printf("Failed killing the processRun.\n");
            }
            else{
                // Report with detail is in the function
                printf("Killing successfully.\n");
            }
            break;
        case 'Q':
            printf("*****Running Quantum*****\n");
            // Report with detail is in the function
            return_pid = quantum();
            break;
        case 'S':
            printf("*****Running Send*****\n");
            printf("Enter the PID you want to send to: ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the PID you want to send to: ");
                }
                else if (temp[0] == '0'){
                    puts("Failed sending!");
                    puts("The initial process cannot send message.");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the PID you want to send to: ");
                }
                else {
                    break;
                }
            }
            printf("Enter the message you want to send to PID %d: ", atoi(temp));
            fgets(tempMsg, MAX_MESSAGE_LENGTH+2, stdin);
            replace_Newline(tempMsg);
            return_pid = send(atoi(temp), tempMsg);

            if (return_pid == -1){
                printf("Sendind failed. There's no process PID %d\n", atoi(temp));
            }
            else{
                printf("Sending succeeded!\n");
            }
            break;
        case 'R':
            printf("*****Running Receive*****\n");
            return_pid = receive();
            break;
        case 'Y':
            printf("*****Running Reply*****\n");
            printf("Enter the PID you want to reply to: ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the PID you want to reply to: ");
                }
                else if (temp[0] == '0'){
                    puts("Failed replying!");
                    puts("The initial process cannot get replied message.");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the PID you want to reply to: ");
                }
                else {
                    break;
                }
            }
            printf("Enter the message you want to send to PID %d: ", atoi(temp));
            fgets(tempMsg, MAX_MESSAGE_LENGTH+2, stdin);
            replace_Newline(tempMsg);
            return_pid = reply(atoi(temp), tempMsg);
            if (return_pid == -1){
                printf("Reply failed. There's no PID %d \n", atoi(temp));
            }
            else{
                puts("Reply succeeded!");
            }
            break;
        case 'N':
            printf("*****Running New Semaphore*****\n");
            printf("Enter the SID you want to create(0-4): ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the SID you want to create(0-4): ");
                }
                else if (atoi(temp) >= 5 || atoi(temp) <= -1){
                    puts("You can only enter 0-4!");
                    printf("Enter the SID you want to create(0-4): ");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the SID you want to create(0-4): ");
                }
                else {
                    break;
                }
            }
            printf("Enter the Value for the SID %d: ", atoi(temp));
            while(true){
                fgets(tempMsg, MAX_LIST_HEAD, stdin);
                replace_Newline(tempMsg);
                // Check if input is NULL
                if (tempMsg[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the Value for the SID %d: ", atoi(temp));
                }
                else if (atoi(tempMsg) < 0){
                    puts("The value has to be greater or equal than 0!");
                    printf("Enter the Value for the SID %d: ", atoi(temp));
                }
                else if (isNumber(tempMsg) == false){
                    puts("Enter numbers only!");
                    printf("Enter the Value for the SID %d: ", atoi(temp));
                }
                else {
                    break;
                }
            }
            return_pid = semaphoreNew(atoi(temp), atoi(tempMsg));
            if (return_pid == -1){
                puts("Created semaphore failed.");
                puts("The SID has been created.");
            }
            else{
                puts("Created semaphore succeeded!");
            }
            break;
        case 'P':
            printf("*****Running Semaphore P*****\n");
            printf("Enter the SID(0-4): ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the SID(0-4): ");
                }
                else if (atoi(temp) >= 5 || atoi(temp) <= -1){
                    puts("You can only enter 0-4!");
                    printf("Enter the SID(0-4): ");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the SID(0-4): ");
                }
                else {
                    break;
                }
            }
            return_pid = semaphoreP(atoi(temp));
            if (return_pid == -1){
                printf("Failed to use the semaphoreP SID %d.\n", atoi(temp));
            }
            break;
        case 'V':
            printf("*****Running Semaphore V*****\n");
            printf("Enter the SID(0-4): ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the SID(0-4): ");
                }
                else if (atoi(temp) >= 5 || atoi(temp) <= -1){
                    puts("You can only enter 0-4!");
                    printf("Enter the SID(0-4): ");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the SID(0-4): ");
                }
                else {
                    break;
                }
            }
            return_pid = semaphoreV(atoi(temp));
            if (return_pid == -1){
                printf("Failed to use the semaphoreV SID %d.\n", atoi(temp));

            }
            break;
        case 'I':
            printf("*****Running Procinfo*****\n");
            printf("Enter the PID you want to check: ");
            while(true){
                fgets(temp, MAX_LIST_HEAD, stdin);
                replace_Newline(temp);
                // Check if input is NULL
                if (temp[0] == '\0'){
                    puts("Enter something!");
                    printf("Enter the PID you want to check: ");
                }
                else if (isNumber(temp) == false){
                    puts("Enter numbers only!");
                    printf("Enter the PID you want to check: ");
                }
                else {
                    break;
                }
            }
            replace_Newline(temp);
            return_pid = procInfo(atoi(temp));
            if (return_pid == -1){
                printf("The PID %s does not exist.\n", temp);
            }
            break;
        case 'T':
            printf("*****Running Totalinfo*****\n");
            totalInfo();
            break;
        case '!':
            printf("*****Terminating Program*****\n");
            exit(0);
        }  
    }
}