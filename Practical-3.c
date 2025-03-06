#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CODE_LENGTH 10000
#define MAX_TOKEN_LENGTH 100
#define MAX_SYMBOL_TABLE_SIZE 100


const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while",
    "_Atomic", "_Bool"
};
const int numKeywords = sizeof(keywords) / sizeof(keywords[0]);


const char operators[] = "+-*/=<>!&|^%";
const char symbols[] = ";,(){}[]";

char symbolTable[MAX_SYMBOL_TABLE_SIZE][MAX_TOKEN_LENGTH];
int symbolCount = 0;


bool isKeyword(const char* word) {
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isOperator(char ch) {
    for (int i = 0; operators[i] != '\0'; i++) {
        if (operators[i] == ch) {
            return true;
        }
    }
    return false;
}


bool isSymbol(char ch) {
    for (int i = 0; symbols[i] != '\0'; i++) {
        if (symbols[i] == ch) {
            return true;
        }
    }
    return false;
}

void addToSymbolTable(const char* identifier) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i], identifier) == 0) {
            return;
        }
    }
    strcpy(symbolTable[symbolCount++], identifier);
}

void removeComments(const char* code, char* result) {
    bool singleLineComment = false, multiLineComment = false;
    int j = 0;
    for (int i = 0; code[i] != '\0'; i++) {
        if (singleLineComment && code[i] == '\n') {
            singleLineComment = false;
            result[j++] = '\n';
        } else if (multiLineComment && code[i] == '*' && code[i + 1] == '/') {
            multiLineComment = false;
            i++;
        } else if (!singleLineComment && !multiLineComment) {
            if (code[i] == '/' && code[i + 1] == '/') {
                singleLineComment = true;
                i++;
            } else if (code[i] == '/' && code[i + 1] == '*') {
                multiLineComment = true;
                i++;
            } else {
                result[j++] = code[i];
            }
        }
    }
    result[j] = '\0';
}

void lexicalAnalyzer(const char* code) {
    int i = 0;
    int length = strlen(code);

    while (i < length) {
        char ch = code[i];

        if (isspace(ch)) {
            i++;
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            char word[MAX_TOKEN_LENGTH] = {0};
            int j = 0;
            while (isalnum(ch) || ch == '_') {
                word[j++] = ch;
                ch = code[++i];
            }
            word[j] = '\0';
            if (isKeyword(word)) {
                printf("Keyword: %s\n", word);
            } else {
                printf("Identifier: %s\n", word);
                addToSymbolTable(word);
            }
        }

        else if (isdigit(ch)) {
            char number[MAX_TOKEN_LENGTH] = {0};
            int j = 0;
            while (isdigit(ch)) {
                number[j++] = ch;
                ch = code[++i];
            }
            number[j] = '\0';
            printf("Constant: %s\n", number);
        }

        else if (ch == '"') {
            char str[MAX_TOKEN_LENGTH] = {0};
            int j = 0;
            str[j++] = ch;
            ch = code[++i];
            while (i < length && ch != '"') {
                str[j++] = ch;
                ch = code[++i];
            }
            if (ch == '"') {
                str[j++] = ch;
                printf("String: %s\n", str);
                i++;
            } else {
                printf("Lexical Error: Unterminated string literal.\n");
            }
        }

        else if (isOperator(ch)) {
            char op[3] = {ch, '\0', '\0'};
            if (isOperator(code[i + 1])) {
                op[1] = code[++i];
            }
            printf("Operator: %s\n", op);
            i++;
        }

        else if (isSymbol(ch)) {
            printf("Punctuation: %c\n", ch);
            i++;
        }
        else {
            printf("Lexical Error: Unknown token '%c'\n", ch);
            i++;
        }
    }
}

int main() {
    char code[MAX_CODE_LENGTH];
    char processedCode[MAX_CODE_LENGTH];

    printf("Enter your C code (end input with a single line containing 'END'):\n");
    char line[MAX_CODE_LENGTH];
    code[0] = '\0';

    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "END\n") == 0) {
            break;
        }
        strcat(code, line);
    }

    removeComments(code, processedCode);

    printf("\nLexical Analysis:\n");
    lexicalAnalyzer(processedCode);


    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("Identifier: %s\n", symbolTable[i]);
    }

    printf("\nModified Source Code:\n%s\n", processedCode);

    return 0;
}

