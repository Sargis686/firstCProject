#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned long long characters_with_spaces;
    unsigned long long characters_without_spaces;
    unsigned long long words;
    unsigned long long lines;
    unsigned long long sentences;
    unsigned long long vowels;
    unsigned long long letters[26];
} TextAnalysis;

static void analyze_text(FILE *input, TextAnalysis *result)
{
    int ch;
    int in_word = 0;
    int saw_character = 0;
    int last_character = '\n';

    while ((ch = fgetc(input)) != EOF) {
        unsigned char current = (unsigned char)ch;

        saw_character = 1;
        last_character = ch;
        result->characters_with_spaces++;

        if (ch != ' ') {
            result->characters_without_spaces++;
        }

        if (ch == '\n') {
            result->lines++;
        }

        if (isspace(current)) {
            in_word = 0;
        } else if (!in_word) {
            result->words++;
            in_word = 1;
        }

        if (ch == '.'  ch == '!'  ch == '?') {
            result->sentences++;
        }

        if (isalpha(current)) {
            int letter = tolower(current);

            /* This program reports frequencies for the English alphabet. */
            if (letter >= 'a' && letter <= 'z') {
                result->letters[letter - 'a']++;
                if (letter == 'a'  letter == 'e'  letter == 'i' 
                    letter == 'o'  letter == 'u') {
                    result->vowels++;
                }
            }
        }
    }

    /* A final line does not have to end with a newline character. */
    if (saw_character && last_character != '\n') {
        result->lines++;
    }
}

static void print_report(FILE *output, const TextAnalysis *result)
{
    int i;
    int most_frequent = -1;

    fprintf(output, "\nTEXT ANALYSIS\n");
    fprintf(output, "-------------\n");
    fprintf(output, "Characters (including spaces): %llu\n",
            result->characters_with_spaces);
    fprintf(output, "Characters (excluding spaces): %llu\n",
            result->characters_without_spaces);
    fprintf(output, "Words:                         %llu\n", result->words);
    fprintf(output, "Lines:                         %llu\n", result->lines);
    fprintf(output, "Sentences:                     %llu\n", result->sentences);
    fprintf(output, "Vowels:                        %llu\n", result->vowels);

    fprintf(output, "\nLETTER FREQUENCIES\n");
    fprintf(output, "------------------\n");
    for (i = 0; i < 26; i++) {
        fprintf(output, "%c: %llu%s", 'A' + i, result->letters[i],
                (i % 4 == 3  i == 25) ? "\n" : "\t");

        if (most_frequent == -1 
            result->letters[i] > result->letters[most_frequent]) {
            most_frequent = i;
        }
    }

    if (most_frequent >= 0 && result->letters[most_frequent] > 0) {
        fprintf(output, "\nMost frequent letter: %c (%llu occurrence%s)\n",
                'A' + most_frequent,
                result->letters[most_frequent],
                result->letters[most_frequent] == 1 ? "" : "s");
    } else {
        fprintf(output, "\nMost frequent letter: none\n");
    }
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output = stdout;
    TextAnalysis result = {0};

    if (argc > 3) {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            perror("Could not open input file");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr,
                "Enter or paste text, then signal end-of-input "
                "(Ctrl+Z then Enter on Windows):\n");
    }

    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (output == NULL) {
            perror("Could not open output file");
            fclose(input);
            return EXIT_FAILURE;
        }
    }

    analyze_text(input, &result);
    print_report(output, &result);

    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        if (fclose(output) != 0) {
            perror("Could not finish writing output file");
            return EXIT_FAILURE;
        }
        printf("Analysis saved to %s\n", argv[2]);
    }

    return EXIT_SUCCESS;
}