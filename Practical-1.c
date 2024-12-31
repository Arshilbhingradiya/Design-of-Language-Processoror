#include <stdio.h>
#include <string.h>
#include <stdbool.h>
bool isValidString(const char *str) {
    int len = strlen(str);

   
    if (len < 2 || str[len - 1] != 'b' || str[len - 2] != 'b') {
        return false;
    }


    for (int i = 0; i < len - 2; i++) {
        if (str[i] != 'a') {
            return false;
        }
    }

    return true;
}

int main() {
    char input[100];
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Invalid input.\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    if (isValidString(input)) {
        printf("Valid String\n");
    } else {
        printf("Invalid String\n");
    }

    return 0;
}
