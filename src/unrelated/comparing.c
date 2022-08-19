#include "unrelated/comparing.h"

int ou_comparing_size(int measure) {
    return (measure == 0 ? 1 : (floor(log10(abs(measure))) + 1 + (measure < 0 ? 1 : 0)));
}

int ou_comparing_trailing(double measure, int size_floor) {
    int trailing_zeros = 12;
    int len_size = size_floor + trailing_zeros + 1; 
    char analysing_trail[len_size];
    snprintf(analysing_trail, len_size, "%.*f", trailing_zeros, measure);
    bool found_decimal = false;
    for (int i = 0; i < strlen(analysing_trail); i++) {
        if (!found_decimal) {
            if (analysing_trail[i] == '.') {
                found_decimal = true;
                trailing_zeros--;
            }
            continue;
        }
        if (analysing_trail[i] == '0') {
            trailing_zeros--;
        } else {
            trailing_zeros = 12 - 1;
        }
    }
    return trailing_zeros;
}

void ou_comparing_editor(ComparingEditor *data, unsigned int num, FILE *fp) {
    for (int i = 0; i < num; i++) {
        char *output;
        switch ((data + i)->type) {
            case e_n: {
                int size_floor = ou_comparing_size(*(data + i)->info.n);
                int len = strlen((data + i)->name) + 1 + 1 + size_floor + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s: %d", (data + i)->name, *(data + i)->info.n);
                strcat(output, "\n");
                break;
            }

            case e_d: {
                int size_floor = ou_comparing_size((int) *(data + i)->info.d);
                int trailing_zeros = ou_comparing_trailing(*(data + i)->info.d, size_floor);
                int len = strlen((data + i)->name) + 1 + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s: %.*f", (data + i)->name, trailing_zeros, *(data + i)->info.d);
                strcat(output, "\n");
                break;
            }

            case e_s: {
                int len = strlen((data + i)->name) + 1 + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s: ", (data + i)->name);
                if (*(data + i)->info.s.p != NULL || *(data + i)->info.s.n != 0) {
                    for (int j = 0; j < *(data + i)->info.s.n; j++) {
                        int len_buffer = /*What is this for?*/1 + (j != (*(data + i)->info.s.n - 1) ? 1 : 0) + ou_comparing_size(*(*(data + i)->info.s.p + j));
                        len += len_buffer;
                        char *buffer = malloc((len_buffer + 1) * sizeof(char));
                        snprintf(buffer, len_buffer, (j != (*(data + i)->info.s.n - 1) ? "%d," : "%d"), *(*(data + i)->info.s.p + j));
                        output = realloc(output, (len + 1) * sizeof(char));
                        strcat(output, buffer);
                        free(buffer);
                    }
                }
                strcat(output, "\n");
                break;
            }

            default:
                printf("Defaulted Typed Hit. Exiting.");
                exit(1);
        }
        fputs(output, fp);
        free(output);
    }
}

void ou_comparing_difficulty(ComparingDifficulty *data, unsigned int num, FILE *fp) {
    for (int i = 0; i < num; i++) {
        int size_floor = ou_comparing_size((int) *(data + i)->info);
        int trailing_zeros = ou_comparing_trailing(*(data + i)->info, size_floor);
        int len = strlen((data + i)->name) + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + 1;
        char *output = malloc((len + 1) * sizeof(char));
        snprintf(output, len, "%s:%.*f", (data + i)->name, trailing_zeros, *(data + i)->info);
        strcat(output, "\n");
        fputs(output, fp);
        free(output);
    }
}

void ou_comparing_general(ComparingGeneral *data, unsigned int num, FILE *fp) {
    for (int i = 0; i < num; i++) {
        char *output;
        bool put_tofile = false;
        switch ((data + i)->type) {
            case g_cp:
                if (*(data + i)->info.cp == NULL && *(data + i)->original.cp != NULL) {
                    *(data + i)->info.cp = strdup(*(data + i)->original.cp);
                }
                if ((*(data + i)->info.cp != NULL && *(data + i)->original.cp == NULL)
                    || (*(data + i)->info.cp != NULL && *(data + i)->original.cp != NULL && strcmp(*(data + i)->info.cp, *(data + i)->original.cp) != 0)) {
                    int len = strlen((data + i)->name) + 1 + 1 + strlen(*(data + i)->info.cp) + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%s: %s", (data + i)->name, *(data + i)->info.cp);
                    strcat(output, "\n");
                    put_tofile = true;
                }
                break;

            case g_n:
                if (*(data + i)->info.n != *(data + i)->original.n) {
                    int size_floor = ou_comparing_size(*(data + i)->info.n);
                    int len = strlen((data + i)->name) + 1 + 1 + size_floor + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%s: %d", (data + i)->name, *(data + i)->info.n);
                    strcat(output, "\n");
                    put_tofile = true;
                }
                break;

            case g_d:
                if (*(data + i)->info.d != *(data + i)->original.d) {
                    int size_floor = ou_comparing_size((int) *(data + i)->info.d);
                    int trailing_zeros = ou_comparing_trailing(*(data + i)->info.d, size_floor);
                    int len = strlen((data + i)->name) + 1 + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%s: %.*f", (data + i)->name, trailing_zeros, *(data + i)->info.d);
                    strcat(output, "\n");
                    put_tofile = true;
                }
                break;

            case g_b:
                if (*(data + i)->info.b != *(data + i)->original.b) {
                    int len = strlen((data + i)->name) + 1 + 1 + 1 + 1;
                    output = malloc((len + 1) * sizeof(char));
                    snprintf(output, len, "%s: %d", (data + i)->name, *(data + i)->info.b);
                    strcat(output, "\n");
                    put_tofile = true;
                }
                break;
        }
        if (put_tofile) {
            fputs(output, fp);
            free(output);
        }
    }
}

void ou_comparing_metadata(ComparingMetadata *data, unsigned int num, FILE *fp) {
    for (int i = 0; i < num; i++) {
        char *output;
        switch ((data + i)->type) {
            case m_cp: {
                int len = strlen((data + i)->name) + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s:", (data + i)->name);
                if (*(data + i)->info.cp != NULL) {
                    output = realloc(output, (len + strlen(*(data + i)->info.cp) + 1) * sizeof(char));
                    strcat(output, *(data + i)->info.cp);
                    strcat(output, "\n");
                }
                break;
            }

            case m_s: {
                int len = strlen((data + i)->name) + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s:", (data + i)->name);
                if (*(data + i)->info.s.cpp != NULL || *(data + i)->info.s.ui != 0) {
                    for (int j = 0; j < *(data + i)->info.s.ui; j++) {
                        int len_buffer = /*What is this for?*/1 + (j != (*(data + i)->info.s.ui - 1) ? 1 : 0) + strlen(*(*(data + i)->info.s.cpp + j));
                        len += len_buffer;
                        char *buffer = malloc((len_buffer + 1) * sizeof(char));
                        snprintf(buffer, len_buffer, (j != (*(data + i)->info.s.ui - 1) ? "%s " : "%s"), *(*(data + i)->info.s.cpp + j));
                        output = realloc(output, (len + 1) * sizeof(char));
                        strcat(output, buffer);
                        free(buffer);
                    }
                }
                strcat(output, "\n");
                break;
            }

            case m_n: {
                int size_floor = (*(data + i)->info.n == 0 ? 1 : (floor(log10(abs(*(data + i)->info.n))) + 1 + (*(data + i)->info.n < 0 ? 1 : 0)));
                int len = strlen((data + i)->name) + 1 + size_floor + 1;
                output = malloc((len + 1) * sizeof(char));
                snprintf(output, len, "%s:%d", (data + i)->name, *(data + i)->info.n);
                strcat(output, "\n");
                break;
            }
        }
        fputs(output, fp);
        free(output);
    }
}