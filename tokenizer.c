/* This file is licensed under the File-based Public License (FPL) – Version 1.0 (Draft).
 * See the FPL license text in this project’s LICENSE file, or retain this header.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *type;  /* Token type: IDENTIFIER, KEYWORD, NUMBER, STRING, SYMBOL */
    char *value; /* Token value: print, argv, etc. */
} Token;

typedef struct {
    Token *tokens;   /* Array of tokens */
    int count;       /* Number of tokens */
    size_t capacity; /* Allocated capacity */
} TokenList;

void
add_token(TokenList *list, const char *type, const char *value)
{
    if (list->count == list->capacity) {
        list->capacity = list->capacity * 2;
        list->tokens = realloc(list->tokens, list->capacity * sizeof(Token));
    }

    list->tokens[list->count].type = strdup(type);
    list->tokens[list->count].value = strdup(value);
    list->count++;
}

void
init_token_list(TokenList *list)
{
    list->tokens = malloc(10 * sizeof(Token));
    list->count = 0;
    list->capacity = 10;
}

void
print_tokens(const TokenList *list)
{
    for (int i = 0; i < list->count; i++)
        printf("Type: %-10s Value: %s\n", list->tokens[i].type, list->tokens[i].value);
}

void
tokenize(const char *src, TokenList *list)
{
    const char *current = src;

    while (*current) {
        /* Skip whitespace */
        if (isspace(*current)) {
            current++;
            continue;
        }

        /* Recognize keywords and identifiers */
        if (isalpha(*current)) {
            const char *start = current;

            while (isalnum(*current) || *current == '.' || *current == '_')
                current++;

            char *value = strndup(start, current - start);

            if (strncmp(value, "else", sizeof(*value)) == 0 ||
                strncmp(value, "for", sizeof(*value)) == 0 ||
                strncmp(value, "if", sizeof(*value)) == 0 ||
                strncmp(value, "print", sizeof(*value)) == 0) {
                add_token(list, "KEYWORD", value);
            } else {
                add_token(list, "IDENTIFIER", value);
            }

            free(value);
            continue;
        }

        /* Recognize numbers */
        if (isdigit(*current)) {
            const char *start = current;

            while (isdigit(*current)) {
                current++;
            }
            
            char *value = strndup(start, current - start);
            add_token(list, "NUMBER", value);
            free(value);
            continue;
        }

        /* Recognize strings */
        if (*current == '"') {
            current++;
            const char *start = current;

            while (*current && *current != '"')
                current++;

            if (*current == '"') {
                char *value = strndup(start, current - start);
                add_token(list,"STRING", value);
                free(value);
                current++; /* Skip closing quotes */
            } else {
                fprintf(stderr, "Unterminated string literal\n");
                exit(1);
            }

            continue;
        }

        /* Recognize symbols and operators */
        if (strchr("=+-,{}()[]||&&!", *current)) {
            char symbol[2] = {*current, '\0'};
            add_token(list, "SYMBOL", symbol);
            current++;
            continue;
        }

        fprintf(stderr, "Unknown character: %c\n", *current);
        exit(1);
    }
}
