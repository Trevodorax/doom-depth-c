#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

char * custom_itoa(int value, char * str, int base) {
    sprintf(str, (base == 16) ? "%x" : (base == 8) ? "%o" : "%d", value);
    return str;
}

char * custom_strupr(char * str) {
    if (str == NULL) {
        return NULL;
    }

    char * new_str = strdup(str);

    for (size_t i = 0; new_str[i]; i++) {
        new_str[i] = toupper((unsigned char)str[i]);
    }
    return new_str;
}

bool custom_char_check(const char c) {
    if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9')) {
        return true;
    }
    return false;
}

char_type_t get_char_type(char c) {
    if (isdigit(c)) {
        return DIGIT;
    } else if (islower(c)) {
        return LOWERCASE;
    } else if (isupper(c)) {
        return UPPERCASE;
    } else if (c == ' ' || c == '(' || c == ')' || c == '>'){
        return SPECIAL;
    } else {
            return INVALID;
    }
}

void delay(ui_type_t ui_type, int ms) {
    switch (ui_type) {
        case CLI:
            cli_delay(ms);
            break;
        case GUI:
            SDL_Delay(ms);
            break;
    }
}
