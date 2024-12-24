/* This file is licensed under the File-based Public License (FPL) – Version 1.0 (Draft).
 * See the FPL license text in this project’s LICENSE file, or retain this header.
 */
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.c"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.ns>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    /* Get source file length */
    (void)fseek(fp, 0, SEEK_END);
    size_t length = ftell(fp);
    (void)fseek(fp, 0, SEEK_SET);

    /* Read source file into memory */
    char *src = malloc(length + 1);
    (void)fread(src, 1, length, fp);
    src[length] = '\0';
    fclose(fp);

    /* Tokenize the source file */
    TokenList tokens;
    init_token_list(&tokens);
    tokenize(src, &tokens);

    /* Print the tokens */
    print_tokens(&tokens);

    /* Cleanup */
    free(src);
    for (size_t i = 0; i < tokens.count; i++) {
        free(tokens.tokens[i].type);
        free(tokens.tokens[i].value);
    }
    free(tokens.tokens);

    return EXIT_SUCCESS;
}
