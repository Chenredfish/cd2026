#include <stdio.h>
#include <string.h>

/*
 * HW#3 : Recursive-Descent Parser
 *
 * Grammar:
 *   S  -> E S'
 *   S' -> e          (epsilon)
 *   S' -> + S
 *   E  -> num
 *   E  -> ( S )
 *
 * Scanner is based on HW#1's scanner structure.
 * Only #include <stdio.h> and <string.h> are used.
 */

/* ── Token types (same naming style as HW#1) ───────────────────── */
typedef enum {
    LITERAL_TOKEN,      /* integer number          */
    PLUS_TOKEN,         /* +                       */
    LEFTPAREN_TOKEN,    /* (                       */
    RIGHTPAREN_TOKEN,   /* )                       */
    EOF_TOKEN           /* end of input            */
} TokenType;

/* ── Global scanner state ──────────────────────────────────────── */
static TokenType cur_token;
static int       cur_num;   /* value when LITERAL_TOKEN */
static int       err;       /* 1 = parse error          */
static FILE     *input_fp;  /* file to read from        */

/* ── Helper (same as HW#1) ─────────────────────────────────────── */
int isDigit(int c)
{
    return c >= '0' && c <= '9';
}

/* ── Scanner: reads from input_fp, same logic as HW#1 ───────────── */
TokenType scan(void)
{
    int c;
    int len;
    char buf[256];

    /* skip whitespace */
    while ((c = fgetc(input_fp)) != EOF) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
    }

    if (c == EOF) return EOF_TOKEN;

    if (isDigit(c)) {
        /* read integer literal (same as HW#1 LITERAL_TOKEN) */
        len = 0;
        buf[len++] = (char)c;
        while ((c = fgetc(input_fp)) != EOF && isDigit(c)) {
            buf[len++] = (char)c;
        }
        buf[len] = '\0';
        if (c != EOF) ungetc(c, input_fp);

        /* convert string to int without atoi (no stdlib.h) */
        cur_num = 0;
        len = 0;
        while (buf[len] != '\0') {
            cur_num = cur_num * 10 + (buf[len] - '0');
            len++;
        }
        return LITERAL_TOKEN;
    }

    switch (c) {
        case '+': return PLUS_TOKEN;
        case '(': return LEFTPAREN_TOKEN;
        case ')': return RIGHTPAREN_TOKEN;
        default:
            err = 1;
            return EOF_TOKEN;
    }
}

/* ── Print 2*d spaces for tree indentation ──────────────────────── */
static void do_indent(int d)
{
    int i;
    for (i = 0; i < d * 2; i++) putchar(' ');
}

/* ── Forward declarations ──────────────────────────────────────── */
static void parse_S(int d);
static void parse_Sp(int d);
static void parse_E(int d);

/* ── S -> E S' ──────────────────────────────────────────────────── */
static void parse_S(int d)
{
    if (err) return;

    switch (cur_token) {
        case LITERAL_TOKEN:
        case LEFTPAREN_TOKEN:
            do_indent(d);
            printf("S -> E S'\n");
            parse_E(d + 1);    /* E is one level deeper  */
            parse_Sp(d);       /* S' stays at same level */
            break;
        default:
            err = 1;
            printf("syntax error\n");
    }
}

/* ── S' -> +S | epsilon ─────────────────────────────────────────── */
static void parse_Sp(int d)
{
    if (err) return;

    switch (cur_token) {
        case PLUS_TOKEN:
            do_indent(d);
            printf("S' -> + S\n");
            cur_token = scan();   /* consume '+' */
            parse_S(d + 1);
            break;
        case RIGHTPAREN_TOKEN:
        case EOF_TOKEN:
            do_indent(d);
            printf("S' -> e\n");
            break;
        default:
            err = 1;
            printf("syntax error\n");
    }
}

/* ── E -> num | (S) ─────────────────────────────────────────────── */
static void parse_E(int d)
{
    if (err) return;

    switch (cur_token) {
        case LITERAL_TOKEN:
            do_indent(d);
            printf("%d\n", cur_num);
            cur_token = scan();   /* consume number */
            break;
        case LEFTPAREN_TOKEN:
            do_indent(d);
            printf("E -> ( S )\n");
            cur_token = scan();   /* consume '(' */
            parse_S(d + 1);
            if (!err && cur_token != RIGHTPAREN_TOKEN) {
                err = 1;
                printf("syntax error\n");
                break;
            }
            cur_token = scan();   /* consume ')' */
            break;
        default:
            err = 1;
            printf("syntax error\n");
    }
}

/* ── Entry point (same argument style as HW#1) ──────────────────── */
int main(int argc, char *argv[])
{
    if (argc >= 2) {
        input_fp = fopen(argv[1], "r");
        if (input_fp == NULL) {
            printf("Cannot open file\n");
            return 1;
        }
    } else {
        input_fp = stdin;
    }

    err = 0;
    cur_token = scan();
    parse_S(0);
    if (!err && cur_token != EOF_TOKEN)
        printf("syntax error\n");

    if (input_fp != stdin)
        fclose(input_fp);
    return 0;
}
