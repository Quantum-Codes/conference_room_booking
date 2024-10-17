#include <stdio.h>

int main(void) {
    char rawcommand[50];
    char command[6][50];
    while (1) {
        scanf(" %[^\n]", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        printf("%s\n", rawcommand);


    }
}

void tokenize(char rawcommand[], char command[6][50]) {
    int word = 0, k = 0; // k = wordcounter
    for (int i = 0; rawcommand[i] != '\0'; i++) {
        if (rawcommand[i] == ' ') {
            command[word][k] = '\0';
            k = 0;
            word++;
            continue;
        }
        command[word][k] = rawcommand[i];
    }
}