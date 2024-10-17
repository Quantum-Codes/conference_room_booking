#include <stdio.h>

int tokenize(char rawcommand[], char command[6][50]);

int main(void) {
    char rawcommand[50];
    char command[6][50];
    int len_command;
    while (1) {
        scanf(" %[^\n]", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        printf("%s\n", rawcommand);

        len_command = tokenize(rawcommand, command);
        for (int i = 0; i < len_command; i++) {
            printf("%s ", command[i]);
        }
    }
}

int tokenize(char rawcommand[], char command[6][50]) { // return val = length of command array
    int word = 0, k = 0; // k = wordcounter
    int i = 0;
    for (;rawcommand[i] != '\0'; i++) {
        if (rawcommand[i] == ' ') {
            command[word][k] = '\0';
            k = 0;
            word++;
            continue;
        }
        command[word][k] = rawcommand[i];
        k++;
    }
    command[word][k] = '\0';
    word++;
    return word;
}