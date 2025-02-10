#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 20
#define MAX_LEN 10
#define TERMINALS 128

char grammar[MAX_PROD][MAX_LEN];  // Stores productions
char first[26][TERMINALS], follow[26][TERMINALS];
int parseTable[26][TERMINALS];   // LL(1) Parsing Table
int num_productions;

// Function to check if a character is a Non-Terminal
int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// Add element to FIRST/FOLLOW set
void addToSet(char set[], char val) {
    if (!set[val]) set[val] = 1;
}

// Compute FIRST set
void computeFirst(char c) {
    if (!isNonTerminal(c)) { addToSet(first[c - 'A'], c); return; }

    for (int i = 0; i < num_productions; i++) {
        if (grammar[i][0] == c) {
            for (int j = 2; grammar[i][j] != '\0'; j++) {
                if (isNonTerminal(grammar[i][j])) {
                    computeFirst(grammar[i][j]);
                    for (int k = 0; k < TERMINALS; k++)
                        if (first[grammar[i][j] - 'A'][k]) addToSet(first[c - 'A'], k);
                    if (!first[grammar[i][j] - 'A']['^']) break;  // Stop if no epsilon
                } else {
                    addToSet(first[c - 'A'], grammar[i][j]);
                    break;
                }
            }
        }
    }
}

// Compute FOLLOW set
void computeFollow(char c) {
    if (c == grammar[0][0]) addToSet(follow[c - 'A'], '$'); // Start symbol

    for (int i = 0; i < num_productions; i++) {
        for (int j = 2; grammar[i][j] != '\0'; j++) {
            if (grammar[i][j] == c) {
                int k;
                for (k = j + 1; grammar[i][k] != '\0'; k++) {
                    if (isNonTerminal(grammar[i][k])) {
                        for (int t = 0; t < TERMINALS; t++)
                            if (first[grammar[i][k] - 'A'][t]) addToSet(follow[c - 'A'], t);
                        if (!first[grammar[i][k] - 'A']['^']) break;
                    } else {
                        addToSet(follow[c - 'A'], grammar[i][k]);
                        break;
                    }
                }
                if (grammar[i][k] == '\0' && c != grammar[i][0])
                    computeFollow(grammar[i][0]);
            }
        }
    }
}

// Construct Parsing Table
void constructParseTable() {
    for (int i = 0; i < num_productions; i++) {
        char lhs = grammar[i][0];
        for (int j = 2; grammar[i][j] != '\0'; j++) {
            if (isNonTerminal(grammar[i][j])) {
                for (int k = 0; k < TERMINALS; k++)
                    if (first[grammar[i][j] - 'A'][k]) parseTable[lhs - 'A'][k] = i;
                if (!first[grammar[i][j] - 'A']['^']) break;
            } else {
                parseTable[lhs - 'A'][grammar[i][j]] = i;
                break;
            }
        }
    }
}

// Print FIRST and FOLLOW sets
void printSets() {
    for (int i = 0; i < num_productions; i++) {
        char c = grammar[i][0];
        printf("FIRST(%c) = { ", c);
        for (int j = 0; j < TERMINALS; j++) if (first[c - 'A'][j]) printf("%c ", j);
        printf("}\n");

        printf("FOLLOW(%c) = { ", c);
        for (int j = 0; j < TERMINALS; j++) if (follow[c - 'A'][j]) printf("%c ", j);
        printf("}\n");
    }
}

// Print Parsing Table
void printParseTable() {
    printf("\nLL(1) Parsing Table:\n");
    for (char nt = 'A'; nt <= 'Z'; nt++) {
        if (!isNonTerminal(nt)) continue;
        printf("%c: ", nt);
        for (int t = 0; t < TERMINALS; t++)
            if (parseTable[nt - 'A'][t]) printf("%c -> %s | ", t, grammar[parseTable[nt - 'A'][t]]);
        printf("\n");
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &num_productions);
    printf("Enter productions (e.g., S=AB):\n");
    for (int i = 0; i < num_productions; i++)
        scanf("%s", grammar[i]);

    for (int i = 0; i < num_productions; i++) computeFirst(grammar[i][0]);
    for (int i = 0; i < num_productions; i++) computeFollow(grammar[i][0]);
    constructParseTable();

    printSets();
    printParseTable();
    return 0;
}
