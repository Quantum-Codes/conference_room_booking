#include <stdio.h>
#define MAX_COMMAND_PARTS 6
#define MAX_PART_LEN 50

int tokeniser(char rawstring[], char tokenised[MAX_PART_LEN][MAX_COMMAND_PARTS]) {
    /*
        Tokenises the rawstring.
        rawstring = source
        tokenised = destination
        return val = length of tokenised array. output in tokenised parameter passed by reference
        return 0 if error (invalid input)
    */
    int word = 0, k = 0; // k = wordcounter
    int i = 0;
    for (; rawstring[i] != '\0'; i++) {
        if (rawstring[i] == ' ') {
            if (i-1 >= 0 && rawstring[i-1] != ' ') { // ignore consecutive spaces. also using short circuit evaluation to avoid undefined behaviour. if 1st confition is false then never evaluates 2nd in &&
                tokenised[word][k] = '\0';
                word++;
                k = 0;
                printf("%s word=%d\n", tokenised[word-1], word-1);
            }
            continue;
        }
        printf("scanned: %i - %c\n", i, rawstring[i]);
        tokenised[word][k] = rawstring[i];
        k++;
        if (k == MAX_PART_LEN) { // == as we catch error when it exceeds, not after.
            printf("error. max len of command part exceeded\n");
            return 0; // len cant be 0 in normal cases
        }
    }
    if (rawstring[i-1] != ' ') { // ignore contiguous spaces
        tokenised[word][k] = '\0';
        word++;
    }
    return word;
}

int main(void) {
    char rawcommand[MAX_COMMAND_PARTS * MAX_PART_LEN]; // store user input
    char command[MAX_PART_LEN][MAX_COMMAND_PARTS];     // store tokenised input
    int len_command;                                   // store length of the tokenised input
    
    while (1) {
        scanf(" %[^\n]", rawcommand); // ignore buffer newlines by space. else the previuos scanf run left a \n in buffer
        len_command = tokeniser(rawcommand, command);
        if (len_command == 0) {
            return 1; // exit with error
        }

        for (int i = 0; i < len_command; i++) {
            printf("(%i %s) ", i, command[i]);
        }
        printf("\n");
    }
}