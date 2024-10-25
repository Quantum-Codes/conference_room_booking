// note: AFTER COMPLETE, DIVIDE INTO MULTIPLE FILES - core.c, helper.c, main.c

/*
Corner cases to take care of:
    1. If record not existing, then don't perform deletion/editing on it
    2. If record exists, then don't add again
    3. Ignore all "marked as deleted" elements for all operations, but dont decrease length of array.
    4. Check for validity of inputs
    5. Prevent undefined behaviours
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* 
assumptions:
   input is like : xxxx xxxxx xxx xxxxxx xxx 
   here we define MAX_COMMAND_PARTS as number of words in input. here its 6
   MAX_PART_LEN is the maximum length of each word
*/
#define MAX_COMMAND_PARTS 6
#define MAX_PART_LEN 50
#define MAX_ROOMS 50

// global vars
/*
rooms array format: {'AB1 301', 'Abheri 34', '<any name of max MAX_PART_LEN chars> <any number with max 3 digits'}
timeslots format: {'0 10', '<index of room in rooms array> <start time(hour)>'} 
we take slots of 1 hour and just store start time. then book multiple slots for a multi hour meeting

Convention we take: If 1st character of an element is \0, then it is deleted and we ignore it for all purposes
*/
char rooms[MAX_ROOMS][MAX_PART_LEN];
char timeslots[1000][MAX_PART_LEN];
int roomCount = 0, timeSlotCount = 0;

// prototypes

// helpers
int tokenizer(char rawstring[], char tokenized[][MAX_PART_LEN]);
int processInstructions(char command[MAX_COMMAND_PARTS][MAX_PART_LEN], int len_command);
int searchInArray(char array[][MAX_PART_LEN], char element[], int lengthOfArray);
void deleteFromArray(char array[][MAX_PART_LEN], int index);
void zfill(char dest[], int len);
int is_valid_number(char number[4]);
void int_to_str(char dest[], int number);
int timeslot_funcs(char roomEntry[], char startTime[], char endTime[], char mode[]);

// core funcs
int addRoom(char buildingName[MAX_PART_LEN], char roomId[4]);
int removeRoom(char buildingName[MAX_PART_LEN], char roomId[4]);
int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]);
int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[4], char time[3], char endTime[3]);
void displayRooms(void);
void displayTimeSlots(void);
void helpCommand(char command[], int listout);


int main(void) {
    char rawcommand[MAX_COMMAND_PARTS * MAX_PART_LEN]; // store user input
    char command[MAX_COMMAND_PARTS][MAX_PART_LEN];     // store tokenized input
    int len_command;                                   // store length of the tokenized input
    int status;

    printf("\nConference Room Booking (Last Updated: 25/10/2024) [Group 3]\n\
Type 'help' for a list of commands.\nType 'help <commandname>' for usage of a command.\n\
Type 'credits' for team info. Type 'exit' to stop.\n\n");
    while (1) {
        printf(">> ");
        scanf(" %[^\n]s", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        len_command = tokenizer(rawcommand, command);
        if (len_command == 0) {
            printf("\n");
            continue;
        }

        //for (int i = 0; i < len_command; i++) { // for printing tokenized output
        //    printf("(%i %s) ", i, command[i]);
        //}
        //printf("\n");

        status = processInstructions(command, len_command);
        if (status == 999) {
            return 0;
        }
        printf("\n");
    }
}

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
            helpCommand(command[0], 0);
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

int addRoom(char buildingName[MAX_PART_LEN], char roomId[4]) {
    /*
        Return 0 if success
        check for existence before adding using searchInArray() func
        building name length should be <= MAX_PART_LEN - 4 to make space for the 3 numbers and 1 space
        If error then print an error message then return 1
    */

    // validate input
    if (!is_valid_number(roomId)) {
        printf("Invalid room number.\n");
        return 1;
    }

    zfill(roomId, 3);

    char roomEntry[MAX_PART_LEN];
    strcpy(roomEntry, buildingName);

    strcat(roomEntry, " ");
    strcat(roomEntry, roomId);

    if (searchInArray(rooms, roomEntry, roomCount) != -1) {
        printf("Room already exists.\n");
        return 1;
    }
    
    strcpy(rooms[roomCount++], roomEntry);
    printf("Successfully added %s as a room.\n", roomEntry);

    return 0;
}

int removeRoom(char buildingName[MAX_PART_LEN], char roomId[4]) {
    /*
        Return 0 if success
        If error then print an error message then return 1

        to delete, just use the deleteFromArray() function by finding index of item using searchInArray() func
    */

    // validate input
    if (!is_valid_number(roomId)) {
        printf("Invalid room number.\n");
        return 1;
    }

    char roomEntry[MAX_PART_LEN];
    strcpy(roomEntry, buildingName);
    strcat(roomEntry, " ");

    zfill(roomId, 3);
    strcat(roomEntry, roomId);

    int index = searchInArray(rooms, roomEntry, roomCount);
    if(index == -1){
        printf("Room not found\n");
        return 1;
    }
    else {
       deleteFromArray(rooms, index);
       printf("Successfully deleted room: %s\n", roomEntry);
    }

    return 0;
}

int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]) {
    /*
        Return 0 if success
        check for existence beforehand using searchInArray() func
        If error then print an error message then return 1
    */

    if (!is_valid_number(roomId)) {
        printf("Invalid room number.\n");
        return 1;
    }
    if (!is_valid_number(startTime)) {
        printf("Invalid start time.\n");
        return 1;
    }
    if (!is_valid_number(endTime)) {
        printf("Invalid end time.\n");
        return 1;
    }
    if (atoi(startTime) >= atoi(endTime)) {
        printf("Booking only possible within today and for more than 0 hours.\n");
        return 1;
    }

    char roomEntry[MAX_PART_LEN];
    zfill(roomId, 3);

    strcpy(roomEntry, buildingName);
    strcat(roomEntry, " ");
    strcat(roomEntry, roomId);

    int index = searchInArray(rooms, roomEntry, roomCount);
    if (index == -1) {
        printf("Room %s not found.\n", roomEntry);
        return 1;
    }

    if(!timeslot_funcs(roomEntry, startTime, endTime, "check")) {
        return 1;
    }

    timeslot_funcs(roomEntry, startTime, endTime, "reserve");
    zfill(startTime, 2); // these zfills are purely for aesthetics
    zfill(endTime, 2); 

    printf("Successfully booked room '%s' for time %s:00 to %s:00\n", roomEntry, startTime, endTime);
    return 0;
}

int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]) {
    /*
        Return 0 if success
        check for existence beforehand using searchInArray() func
        If error then print an error message then return 1
        to delete, just use the deleteFromArray() function by finding index of item using searchInArray() func
    */

    if (!is_valid_number(roomId)) {
        printf("Invalid room number.\n");
        return 1;
    }
    if (!is_valid_number(startTime)) {
        printf("Invalid start time.\n");
        return 1;
    }
    if (!is_valid_number(endTime)) {
        printf("Invalid end time.\n");
        return 1;
    }
    if (atoi(startTime) >= atoi(endTime)) {
        printf("Booking only possible within today and for more than 0 hours.\n");
        return 1;
    }

    char roomEntry[MAX_PART_LEN];
    zfill(roomId, 3);

    strcpy(roomEntry, buildingName);
    strcat(roomEntry, " ");
    strcat(roomEntry, roomId);

    int index = searchInArray(rooms, roomEntry, roomCount);
    if (index == -1) {
        printf("Room %s not found.\n", roomEntry);
        return 1;
    }

    timeslot_funcs(roomEntry, startTime, endTime, "reserve");
    zfill(startTime, 2); // these zfills are purely for aesthetics
    zfill(endTime, 2); 

    printf("Successfully cancelled room booking on '%s' for time %s:00 to %s:00\n", roomEntry, startTime, endTime);
    return 0;
}

void displayRooms(void) {
    /*
        Just print all rooms and buildings neatly. No need of returning anything
        ignore deleted entries
    */
    printf("%d records found\n", roomCount);
    for (int i = 0, k = 1; i < roomCount; i++) {
        if (rooms[i][0] != '\0') {
            printf("%d. %s\n", k, rooms[i]);
            k++;
        }
    }
}

void displayTimeSlots(void) {
    /*
        Just print all timeslots with rooms neatly. No need of returning anything
        ignore deleted entries
    */
    printf("%d records found\n", timeSlotCount);
    char roomEntry[2][MAX_PART_LEN];
    for (int i = 0, k = 1; i < timeSlotCount; i++) {
        if (timeslots[i][0] != '\0') {
            tokenizer(timeslots[i], roomEntry);
            printf("%d. %s %s:00\n", k, rooms[atoi(roomEntry[0])], roomEntry[1]);
            k++;
        }
    }
   return;
}

void helpCommand(char command[], int listout) {
    if (listout) {
        printf("List of commands:\n");
        char commands[][MAX_PART_LEN] = {"add", "remove", "reserve", "cancel", "rooms", "timeslots", "help"};
        char shortDesc[][500] = {
            "'add <buildingName> <roomNo>'\n[Admin command]\nAdds the given room to be booked by other users.",
            "'add <buildingName> <roomNo>'\n[Admin command]\nRemoves the given room from rooms list. Nobody can book this now.",
            "'reserve <buildingName> <roomNo>'\n[Admin command]\nRemoves the given room from rooms list. Nobody can book this now."
        };
    }
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
        return 1;
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
                printf("Slot from %s:00 to %s:00 is already booked. Please try to reschedule or pick another room.\n", startTime_str);
                return 1;
            }
        }
        else if (strcmp(mode, "reserve") == 0) {
            strcmp(timeslots[timeSlotCount++], roomData);
        }
        else if (strcmp(mode, "cancel") == 0) {
            int index = searchInArray(timeslots, roomData, timeSlotCount);
            if (index != -1) {
                deleteFromArray(timeslots, index);
            }
        }
    }
    return 0;
}