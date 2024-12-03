#include <cstdio>
#include <cctype>
#include <cstdlib>

#ifdef AOC_TEST
#define INPUT_FILENAME "day3/input_test.txt"
#else
#define INPUT_FILENAME "day3/input.txt"
#endif

constexpr char DO_INSTRUCTION[] = {'d', 'o', '(', ')'};
constexpr char DONT_INSTRUCTION[] = {'d', 'o', 'n', '\'', 't', '(', ')'};
constexpr char MUL_INSTRUCTION[] = {'m', 'u', 'l', '('};
constexpr char MUL_SEPARATOR = ',';
constexpr char MUL_TERMINATOR = ')';

bool MUL_ENABLED = true;

void ignore_noop(char *c, FILE *fp) {
    while (*c != EOF) {
        if (*c == MUL_INSTRUCTION[0] || *c == DO_INSTRUCTION[0]) {
            break;
        }
        *c = (char) fgetc(fp);
    }
    ungetc(*c, fp);
}

bool parse_char(FILE *fp, char c) {
    return ((char) fgetc(fp)) == c;
}

constexpr int MUL_STRLEN = sizeof(MUL_INSTRUCTION);
bool parse_mul(FILE *fp) {
    int i = 0;
    char c = (char) fgetc(fp);
    while (i < MUL_STRLEN && c == MUL_INSTRUCTION[i]) {
        c = (char) fgetc(fp);
        ++i;
    }
    ungetc(c, fp);
    if (i != MUL_STRLEN) {
        // backtrack to beginning of parsing if this was not correct
        for (int j = i - 1; j >= 0; --j) {
            ungetc(MUL_INSTRUCTION[j], fp);
        }
    }
    return i == MUL_STRLEN;
}

constexpr int DO_STRLEN = sizeof(DO_INSTRUCTION);
bool parse_do(FILE *fp) {
    int i = 0;
    char c = (char) fgetc(fp);
    while (i < DO_STRLEN && c == DO_INSTRUCTION[i]) {
        c = (char) fgetc(fp);
        ++i;
    }
    ungetc(c, fp);
    if (i != DO_STRLEN) {
        // backtrack to beginning of parsing if this was not correct
        for (int j = i - 1; j >= 0; --j) {
            ungetc(DO_INSTRUCTION[j], fp);
        }
    }
    return i == DO_STRLEN;
}

constexpr int DONT_STRLEN = sizeof(DONT_INSTRUCTION);
bool parse_dont(FILE *fp) {
    int i = 0;
    char c = (char) fgetc(fp);
    while (i < DONT_STRLEN && c == DONT_INSTRUCTION[i]) {
        c = (char) fgetc(fp);
        ++i;
    }
    // IMPORTANT: we do not backtrack here because we are the last; we would need to do so otherwise
    // ungetc(c, fp);
    // if (i != DONT_STRLEN) {
    //     // backtrack to beginning of parsing if this was not correct
    //     for (int j = i - 1; j >= 0; --j) {
    //         ungetc(DONT_INSTRUCTION[j], fp);
    //     }
    // }
    return i == DONT_STRLEN;
}

constexpr int MAX_NUMBERLEN = 3;
int parse_number(FILE *fp) {
    char number[MAX_NUMBERLEN + 1];
    int i = 0;

    int c = fgetc(fp);
    while (i < MAX_NUMBERLEN && isdigit(c)) {
        number[i++] = (char) c;
        c = fgetc(fp);
    }
    ungetc(c, fp);
    number[i] = '\0';

    return i > 0 ? atoi(number) : -1;
}

int main() {
    // read file input
    FILE *fp = nullptr;
    if (!fopen_s(&fp, INPUT_FILENAME, "r")) {}
    long mul_total = 0;

    char c = (char) fgetc(fp);
    while (c != EOF) {
        ignore_noop(&c, fp);
        printf("found potential keyword after %ld\n", ftell(fp));
        if (MUL_ENABLED && parse_mul(fp)) {
            int lvalue = parse_number(fp);
            if (lvalue == -1) {
                c = (char) fgetc(fp);
                continue;
            }

            if (!parse_char(fp, MUL_SEPARATOR)) {
                c = (char) fgetc(fp);
                continue;
            }

            int rvalue = parse_number(fp);
            if (rvalue == -1) {
                c = (char) fgetc(fp);
                continue;
            }

            if (!parse_char(fp, MUL_TERMINATOR)) {
                c = (char) fgetc(fp);
                continue;
            }

            mul_total += lvalue * rvalue;
        } else if (parse_do(fp)) {
            MUL_ENABLED = true;
        } else if (parse_dont(fp)) {
            MUL_ENABLED = false;
        }
        c = (char) fgetc(fp);
    }

    printf("Total of mul ops: %ld\n", mul_total);

    return 0;
}
