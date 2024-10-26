#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_PARTS 6
#define MAX_PART_LEN 50
#define MAX_ROOMS 50

extern char rooms[MAX_ROOMS][MAX_PART_LEN];
extern char timeslots[1000][MAX_PART_LEN];
extern int roomCount;
extern int timeSlotCount;

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
void deleteStrayTimeSlots(int roomIndex);

// core funcs
int addRoom(char buildingName[MAX_PART_LEN], char roomId[4]);
int removeRoom(char buildingName[MAX_PART_LEN], char roomId[4]);
int reserveRoom(char buildingName[MAX_PART_LEN], char roomId[4], char startTime[3], char endTime[3]);
int cancelRoom(char buildingName[MAX_PART_LEN], char roomId[4], char time[3], char endTime[3]);
void displayRooms(void);
void displayTimeSlots(void);
void helpCommand(char command[], int listout);