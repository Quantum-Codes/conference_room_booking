#include "globalVars.h"

int addRoom(char buildingName[MAX_PART_LEN], char roomId[4]) {
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
       deleteStrayTimeSlots(index);
       printf("Successfully deleted room: %s\n", roomEntry);
    }

    return 0;
}

int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]) {
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

    if(timeslot_funcs(roomEntry, startTime, endTime, "check")) {
        return 1;
    }

    timeslot_funcs(roomEntry, startTime, endTime, "reserve");
    zfill(startTime, 2); // these zfills are purely for aesthetics
    zfill(endTime, 2); 

    printf("Successfully booked room '%s' for time %s:00 to %s:00\n", roomEntry, startTime, endTime);
    return 0;
}

int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]) {
    //validate input
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

    timeslot_funcs(roomEntry, startTime, endTime, "cancel");
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
   int k = 1;
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i][0] != '\0') {
            printf("%d.\t%s\n", k, rooms[i]);
            k++;
        }
    }
    if (k == 1) {
        printf("No records found.\n");
    }
    return;
}

void displayTimeSlots(void) {
    /*
        Just print all timeslots with rooms neatly. No need of returning anything
        ignore deleted entries
    */
   int k = 1;
    char roomEntry[2][MAX_PART_LEN];
    for (int i = 0; i < timeSlotCount; i++) {
        if (timeslots[i][0] != '\0') {
            tokenizer(timeslots[i], roomEntry);
            printf("%d.\t%s %s:00\n", k, rooms[atoi(roomEntry[0])], roomEntry[1]);
            k++;
        }
    }
    if (k == 1) {
        printf("No records found.\n");
    }
   return;
}

void helpCommand(char command[], int listout) {
    int no_of_commands = 8; // manually updated
    char commands[][MAX_PART_LEN] = {"add", "remove", "reserve", "cancel", "rooms", "timeslots", "help", "exit"};
    char shortDesc[][100] = {
        "[ADMIN] Adds a room to pool of bookable rooms.",
        "[ADMIN] Deletes a room and all its timeslots booked.",
        "Reserve a room for the given time.",
        "Cancel a previous booking.",
        "List out all rooms.",
        "List out all timeslots already booked.",
        "Get help on a specific command ('help [command]') or just get a list of commands. (this)",
        "Exit out of program."
    };
    char longDesc[][500] = {
        "Syntax: 'add <buildingName> <roomNo>'\n\n[Admin command]\nAdds the given room to be booked by other users.\nTypically the roomnumber needs to be of 3 digits, but if less is supplied then the program manages it by prefixing zeros to the start.\nConverts all letters to lowercase.\n\nExample Usage: add 'LHC 211', 'Abheri 4'",
        "Syntax: 'remove <buildingName> <roomNo>'\n\n[Admin command]\nRemoves the given room from rooms list. All booked timeslots are also removed.\nTypically the roomnumber needs to be of 3 digits, but if less is supplied then the program manages it by prefixing zeros to the start.\nConverts all letters to lowercase.\n\nExample Usage: 'remove LHC 211'",
        "Syntax: 'reserve <buildingName> <roomNo> <startTime> <endTime>'\n\nReserves the room for you in the given time.\nNote that your meeting cannot end at 12am(00:00) or continue through 12am\nTypically the roomnumber needs to be of 3 digits, but if less is supplied then the program manages it by prefixing zeros to the start.\nConverts all letters to lowercase.\n\nExample Usage: 'reserve LHC 211 8 17'",
        "Syntax: 'cancel <buildingName> <roomNo> <startTime> <endTime>'\n\nCancels the booking for a room.\nNote that your meeting cannot end at 12am(00:00) or continue through 12am\nTypically the roomnumber needs to be of 3 digits, but if less is supplied then the program manages it by prefixing zeros to the start.\nConverts all letters to lowercase.\n\nExample Usage: 'cancel LHC 211 9 12'",
        "Syntax: 'rooms'\n\nLists out all rooms available.",
        "Syntax: 'timeslots'\n\nLists out all the timeslots already booked for all rooms.",
        "Syntax: 'help [command]'\n\nGet a list of all commands.\nIf optional param 'command' is supplied then it gives the long description of the command.\n\nExample Usage: 'help', 'help reserve'",
        "Syntax: 'exit'\n\nStops the program immediately. Doesnt care if any extra arguments are passed."
    };
    
    if (listout) {
        printf("List of commands:\n");
        for (int i = 0; i < no_of_commands; i++) {
            printf("%d. %s - %s\n", i+1, commands[i], shortDesc[i]);
        }
        return;
    }
    else {
        int found = 0;
        for (int i = 0; i < no_of_commands; i++) {
            if (strcmp(commands[i], command) == 0) {
                found = 1;
                printf("Man Page of %s:\n\n%s\n", commands[i], longDesc[i]);
            }
        }
        if (!found) {
            printf("No such command. See all commands by typing 'help'.\n");
            return;
        }
    }
} 