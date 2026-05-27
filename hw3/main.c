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
 * Output rule (depth d, 2 spaces per level):
 *   parse_S(d)  prints "S -> E S'" at d, calls parse_E(d+1), parse_Sp(d)
 *   parse_Sp(d) prints "S' -> + S" / "S' -> e"  at d
 *   parse_E(d)  prints the number / "E -> ( S )" at d
 */

/* ── Token types ───────────────────────────────────────────────── */
#define TOK_NUM    0
#define TOK_PLUS   1
#define TOK_LPAREN 2
#define TOK_RPAREN 3
#define TOK_EOF    4

static int cur_tok;   /* current token type          */
static int cur_num;   /* numeric value when TOK_NUM  */
static int err;       /* error flag                  */

/* ── Scanner (reads from stdin) ────────────────────────────────── */
static int next_token(void)
{
    int c;

    /* skip whitespace */
    while ((c = getchar()) == ' ' || c == '\t' || c == '\n' || c == '\r')
        ;

    if (c == EOF) return TOK_EOF;

    if (c >= '0' && c <= '9') {
        cur_num = c - '0';
        while ((c = getchar()) != EOF && c >= '0' && c <= '9')
            cur_num = cur_num * 10 + (c - '0');
        if (c != EOF) ungetc(c, stdin);
        return TOK_NUM;
    }

    switch (c) {
        case '+': return TOK_PLUS;
        case '(': return TOK_LPAREN;
        case ')': return TOK_RPAREN;
        default:
            err = 1;
            return TOK_EOF;
    }
}

/* ── Print 2*d spaces ───────────────────────────────────────────── */
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

    switch (cur_tok) {
        case TOK_NUM:
        case TOK_LPAREN:
            do_indent(d);
            printf("S -> E S'\n");
            parse_E(d + 1);   /* E is one level deeper  */
            parse_Sp(d);      /* S' is at the same level as S */
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

    switch (cur_tok) {
        case TOK_PLUS:
            do_indent(d);
            printf("S' -> + S\n");
            cur_tok = next_token();   /* consume '+' */
            parse_S(d + 1);
            break;
        case TOK_RPAREN:
        case TOK_EOF:
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

    switch (cur_tok) {
        case TOK_NUM:
            do_indent(d);
            printf("%d\n", cur_num);
            cur_tok = next_token();   /* consume number */
            break;
        case TOK_LPAREN:
            do_indent(d);
            printf("E -> ( S )\n");
            cur_tok = next_token();   /* consume '(' */
            parse_S(d + 1);
            if (!err && cur_tok != TOK_RPAREN) {
                err = 1;
                printf("syntax error\n");
                break;
            }
            cur_tok = next_token();   /* consume ')' */
            break;
        default:
            err = 1;
            printf("syntax error\n");
    }
}

/* ── Entry point ────────────────────────────────────────────────── */
int main(void)
{
    err = 0;
    cur_tok = next_token();
    parse_S(0);
    if (!err && cur_tok != TOK_EOF)
        printf("syntax error\n");
    return 0;
}
