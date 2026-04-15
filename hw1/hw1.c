#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TYPE_TOKEN,
    MAIN_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    ID_TOKEN,
    LITERAL_TOKEN,
    LEFTPAREN_TOKEN,
    REFTPAREN_TOKEN,
    LEFTBRACE_TOKEN,
    REFTBRACE_TOKEN,
    SEMICOLON_TOKEN,
    EQUAL_TOKEN,
    GREATEREQUAL_TOKEN,
    LESSEQUAL_TOKEN,
    GREATER_TOKEN,
    LESS_TOKEN,
    ASSIGN_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN
} TokenType;

const char *tokenName(TokenType t) {
    switch (t) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case REFTPAREN_TOKEN:    return "REFTPAREN_TOKEN";
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case REFTBRACE_TOKEN:    return "REFTBRACE_TOKEN";
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        default:                 return "UNKNOWN";
    }
}

TokenType classifyId(const char *s) {
    if (strcmp(s, "int")   == 0) return TYPE_TOKEN;
    if (strcmp(s, "main")  == 0) return MAIN_TOKEN;
    if (strcmp(s, "if")    == 0) return IF_TOKEN;
    if (strcmp(s, "else")  == 0) return ELSE_TOKEN;
    if (strcmp(s, "while") == 0) return WHILE_TOKEN;
    return ID_TOKEN;
}

int isLetter(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int isDigit(int c) {
    return c >= '0' && c <= '9';
}

void scan(FILE *file) {
    int ch;
    char buf[256];
    int len;

    while ((ch = fgetc(file)) != EOF) {
        /* skip whitespace */
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            continue;
        }

        if (isLetter(ch)) {
            /* read identifier or keyword */
            len = 0;
            buf[len++] = (char)ch;
            while ((ch = fgetc(file)) != EOF && (isLetter(ch) || isDigit(ch))) {
                buf[len++] = (char)ch;
            }
            buf[len] = '\0';
            if (ch != EOF) ungetc(ch, file);
            TokenType t = classifyId(buf);
            printf("%s: %s\n", buf, tokenName(t));

        } else if (isDigit(ch)) {
            /* read integer literal */
            len = 0;
            buf[len++] = (char)ch;
            while ((ch = fgetc(file)) != EOF && isDigit(ch)) {
                buf[len++] = (char)ch;
            }
            buf[len] = '\0';
            if (ch != EOF) ungetc(ch, file);
            printf("%s: %s\n", buf, tokenName(LITERAL_TOKEN));

        } else {
            switch (ch) {
                case '(': printf("(: %s\n", tokenName(LEFTPAREN_TOKEN));  break;
                case ')': printf("): %s\n", tokenName(REFTPAREN_TOKEN));  break;
                case '{': printf("{: %s\n", tokenName(LEFTBRACE_TOKEN));  break;
                case '}': printf("}: %s\n", tokenName(REFTBRACE_TOKEN));  break;
                case ';': printf(";: %s\n", tokenName(SEMICOLON_TOKEN));  break;
                case '+': printf("+: %s\n", tokenName(PLUS_TOKEN));       break;
                case '-': printf("-: %s\n", tokenName(MINUS_TOKEN));      break;
                case '=':
                    ch = fgetc(file);
                    if (ch == '=') {
                        printf("==: %s\n", tokenName(EQUAL_TOKEN));
                    } else {
                        if (ch != EOF) ungetc(ch, file);
                        printf("=: %s\n", tokenName(ASSIGN_TOKEN));
                    }
                    break;
                case '<':
                    ch = fgetc(file);
                    if (ch == '=') {
                        printf("<=: %s\n", tokenName(LESSEQUAL_TOKEN));
                    } else {
                        if (ch != EOF) ungetc(ch, file);
                        printf("<: %s\n", tokenName(LESS_TOKEN));
                    }
                    break;
                case '>':
                    ch = fgetc(file);
                    if (ch == '=') {
                        printf(">=: %s\n", tokenName(GREATEREQUAL_TOKEN));
                    } else {
                        if (ch != EOF) ungetc(ch, file);
                        printf(">: %s\n", tokenName(GREATER_TOKEN));
                    }
                    break;
                default:
                    /* unrecognized character, skip */
                    break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    scan(file);

    fclose(file);
    return 0;
}
