#include "unrelated/reading_line.h"

static char *ou_readingline_removecrlf(char *line, int read) {
    if (*(line + read - 2) == '\r') {
        *(line + read - 2) = '\0';
    } else if (*(line + read - 1) == '\n') {
        *(line + read - 1) = '\0';
    }
    return line;
}

static bool ou_readingline_iscomment(char *line) {
    return strncmp(line, "//", 2) == 0;
}

char *ou_readingline_line(FILE *fp) {
    static int state = 0;
    switch (state) {
        case 0: {
            state = 1;
            char *line;
            size_t len = 0;
            int read;
            while ((read = getline(&line, &len, fp)) != -1) {
                ou_readingline_removecrlf(line, read);
                if (ou_readingline_iscomment(line)
                    || (*(line + 0) == '[' && *(line + read - 2) == ']')) {
                    continue;
                }
                return line;
        case 1:;
            }
            state = 0;
            if (line != NULL) {
                free(line);
            }
        }
    }
    return NULL;
}