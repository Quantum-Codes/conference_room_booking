#include "globalVars.h"
#include <math.h>

int tokenizer(char rawstring[], char tokenized[][MAX_PART_LEN]) {
    /*
        tokenizes the rawstring.
        rawstring = source (also modified to lower case in this func)
        tokenized = destination
        return val = length of tokenized array. output in tokenized parameter passed by reference
        return 0 if error (invalid input)
    */
    int word = 0, k = 0; // k = wordcounter
    int i = 0;
    for (; rawstring[i] != '\0'; i++) {
        if (rawstring[i] >= 'A' && rawstring[i] <= 'Z') {
            rawstring[i] += 'a' - 'A';
        }
        else if (!((rawstring[i] >= 'a' && rawstring[i] <= 'z')||(rawstring[i] >= '0' && rawstring[i] <= '9')||(rawstring[i] == ' ')||(rawstring[i] == '\0'))) {
            printf("Invalid characters used.\n");
            return 0;
        }
        if (rawstring[i] == ' ') {
            if (i-1 >= 0 && rawstring[i-1] != ' ') { // ignore consecutive spaces. also using short circuit evaluation to avoid undefined behaviour. if 1st confition is false then never evaluates 2nd in &&
                tokenized[word][k] = '\0';
                word++;
                k = 0;
            }
            continue;
        }
        tokenized[word][k] = rawstring[i];
        k++;
        if (k == MAX_PART_LEN) { // == as we catch error when it exceeds, not after.
            printf("error. max len of command part exceeded\n");
            return 0; // len cant be 0 in normal cases
        }
    }
    if (rawstring[i-1] != ' ') { // ignore contiguous spaces
        tokenized[word][k] = '\0';
        word++;
    }
    return word;
}

int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command) { // the getFields() func
    /*
        Runs the appropriate function based on the input command (command[0])
    */

    if (strcmp(command[0], "add") == 0) {
        if (len_command != 3){
            printf("Expected 2 arguments, got %d\n", len_command-1);
            return 1;
        } 
        addRoom(command[1], command[2]);
    }
    else if (strcmp(command[0], "remove") == 0) {
        if (len_command != 3){
            printf("Expected 2 arguments, got %d\n", len_command-1);
            return 1;
        } 
        removeRoom(command[1], command[2]);
    }
    else if (strcmp(command[0], "reserve") == 0) {
        if (len_command != 5){
            printf("Expected 4 arguments, got %d\n", len_command-1);
            return 1;
        } 
        reserveRoom(command[1], command[2], command[3], command[4]);
    }
    else if (strcmp(command[0], "cancel") == 0) {
        if (len_command != 5){
            printf("Expected 4 arguments, got %d\n", len_command-1);
            return 1;
        } 
        cancelRoom(command[1], command[2], command[3], command[4]);
    }
    else if (strcmp(command[0], "rooms") == 0) {
        if (len_command != 1){
            printf("Expected 0 arguments, got %d\n", len_command-1);
            return 1;
        } 
        displayRooms();
    }
    else if (strcmp(command[0], "timeslots") == 0) {
        if (len_command != 1){
            printf("Expected 0 arguments, got %d\n", len_command-1);
            return 1;
        } 
        displayTimeSlots();
    }
    else if (strcmp(command[0], "help") == 0) {
        if (len_command != 1 && len_command != 2){
            printf("Expected 0 or 1 arguments, got %d\n", len_command-1);
            return 1;
        }
        if (len_command == 1) {
            char a[] = "";
            helpCommand(a, 1);
        }
        else {
            helpCommand(command[1], 0);
        }
    }
    else if (strcmp(command[0], "credits") == 0) {
        printf("\
Team 3:\n\
1. CE24B003 Ankit Sinha (lead)\n\
2. EE24B058 Vennam Blessy Manaswitha\n\
3. CS24B004 Bachu Vishnu Vardhan\n\
4. EE24B003 Ardhanala Deepika\n\
5. ME24B003 Anguluri Venkata Rathnam\n");
    }
    else if (strcmp(command[0], "exit") == 0) {
        printf("Exiting program...\n");
        return 999;
    }
    else {
        //for (int i = 0; i < len_command; i++) { // print tokenized output for debugging
        //    printf("(%i %s) ", i, command[i]);
        //}
        //printf("\n");
        printf("No such command... Type \"help\" to see all commands\n");
    }
    return 0;
}

int searchInArray(char array[][MAX_PART_LEN], char element[], int lengthOfArray) {
    /*
        Return index of position of element
        Return -1 if not found
    */
    for (int i = 0; i < lengthOfArray; i++) {
        if (strcmp(array[i], element) == 0) {
            return i;
        }
    }
    return -1;
}

void deleteFromArray(char array[][MAX_PART_LEN], int index) {
    array[index][0] = '\0'; // passed by reference, so will reflect in global var
}

void zfill(char dest[], int len) {
    /*
    ZeroFill like in python.
    Pads zeroes in a number to reach a target length.
    */
    if (strlen(dest) == len) {
        return;
    }

    char src[len];
    strcpy(src, dest);

    for (int i = 0; i < len; i++) {
        if (len - strlen(src) > i) {
            dest[i] = '0';
        }
        else {
            dest[i] = src[i - (len - strlen(src))];
        }
    }
    dest[len] = '\0';
}

int is_valid_number(char number[4]) {
    int found_termination = 0;
    for (int i = 0; i < 4; i++) {
        if (number[i] == '\0') {
            found_termination = 1;
            break;
        }
    }
    if (!found_termination) { // which means number overflowed 
        return 0;
    }

    for (int i = 0; number[i] != '\0'; i++) {
        if (number[i] < '0' || number[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void int_to_str(char dest[], int number) {
    int len;
    if (number != 0) {
        double length = log10((double) number) + 1; // truncated for floor of log
        len = (int) length;
    }
    else {
        strcpy(dest, "0");
        return;
    }
    for (int i = 0; i < len; i++) {
        if (number != 0) {
            dest[len - i - 1] = (number % 10) + '0';
        }
        else {
            dest[len - i - 1] = 'a';
        }
        number /= 10;
    }
    dest[len] = '\0';
}

int timeslot_funcs(char roomEntry[], char startTime[], char endTime[], char mode[]) {
    /*
    Multipurpose func, a merger of multiple similar funcs
    Mode: Check if any booked ("check"), Add records ("reserve"), Remove records ("cancel")
    return val 1 = error, 0 = success/valid
    Note that each mode assumes that other necessary modes have already been run.
    */
    int startTime_int = atoi(startTime);
    int endTime_int = atoi(endTime);
    char roomData[MAX_PART_LEN];
    char startTime_str[3];
    char endTime_str[3];

    for (int i = startTime_int; i < endTime_int; i++) {
        // new start time
        int_to_str(startTime_str, i);
        zfill(startTime_str, 2);

        // reset roomData
        strcpy(roomData, roomEntry);

        //create entry to check
        strcat(roomData, " ");
        strcat(roomData, startTime_str);

        if (strcmp(mode, "check") == 0) {
            if (searchInArray(timeslots, roomData, timeSlotCount) != -1) {
                int_to_str(endTime_str, i+1);
                zfill(endTime_str, 2);
                printf("Slot from %s:00 to %s:00 is already booked. Please try to reschedule or pick another room.\n", startTime_str, endTime_str);
                return 1;
            }
        }
        else if (strcmp(mode, "reserve") == 0) {
            int index = searchInArray(rooms, roomEntry, roomCount);
            int_to_str(roomData, index);
            strcat(roomData, " ");
            strcat(roomData, startTime_str);
            strcpy(timeslots[timeSlotCount++], roomData);
        }
        else if (strcmp(mode, "cancel") == 0) {
            int index = searchInArray(rooms, roomEntry, roomCount);
            int_to_str(roomData, index);
            strcat(roomData, " ");
            strcat(roomData, startTime_str);

            index = searchInArray(timeslots, roomData, timeSlotCount);
            if (index != -1) {
                deleteFromArray(timeslots, index);
            }
        }
    }
    return 0;
}

void deleteStrayTimeSlots(int roomIndex) {
    char timeSlot[2][MAX_PART_LEN]; // supposed to be [24 * MAX_ROOMS + 4](max index from room array + space + 2 digits + \0) but assuming only 46 digit index at max
    for (int i = 0; i < timeSlotCount; i++) {
        tokenizer(timeslots[i], timeSlot);
        if (atoi(timeSlot[0]) == roomIndex) {
            deleteFromArray(timeslots, i);
        }
    }
}