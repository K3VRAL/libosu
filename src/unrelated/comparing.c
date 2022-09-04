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

char *ou_comparing_editor(ComparingEditor *data, unsigned int num) {
    char *output = strdup("");
    int len = 0;
    for (int i = 0; i < num; i++) {
        switch ((data + i)->type) {
            case e_n: {
                int size_floor = ou_comparing_size(*(data + i)->info.n);
                int len_buffer = strlen((data + i)->name) + 1 + 1 + size_floor + (1 + 1);
                char *buffer = malloc(len_buffer * sizeof(char));
                snprintf(buffer, len_buffer, "%s: %d\n", (data + i)->name, *(data + i)->info.n);

                len += len_buffer;
                output = realloc(output, len * sizeof(char));
                strcat(output, buffer);
                free(buffer);
                break;
            }

            case e_d: {
                int size_floor = ou_comparing_size((int) *(data + i)->info.d);
                int trailing_zeros = ou_comparing_trailing(*(data + i)->info.d, size_floor);
                int len_buffer = strlen((data + i)->name) + 1 + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + (1 + 1);
                char *buffer = malloc(len_buffer * sizeof(char));
                snprintf(buffer, len_buffer, "%s: %.*f\n", (data + i)->name, trailing_zeros, *(data + i)->info.d);

                len += len_buffer;
                output = realloc(output, len * sizeof(char));
                strcat(output, buffer);
                free(buffer);
                break;
            }

            case e_s:
                if (*(data + i)->info.s.p != NULL || *(data + i)->info.s.n > 0) {
                    int len_buffer = strlen((data + i)->name) + 1 + 1 + (1 + 1);
                    char *buffer = malloc(len_buffer * sizeof(char));
                    snprintf(buffer, len_buffer, "%s: ", (data + i)->name);
                    if (*(data + i)->info.s.p != NULL || *(data + i)->info.s.n != 0) {
                        for (int j = 0; j < *(data + i)->info.s.n; j++) {
                            int len_buffer2 = 1 + (j != (*(data + i)->info.s.n - 1) ? 1 : 0) + ou_comparing_size(*(*(data + i)->info.s.p + j));
                            char *buffer2 = malloc(len_buffer2 * sizeof(char));
                            snprintf(buffer2, len_buffer2, (j != (*(data + i)->info.s.n - 1) ? "%d," : "%d"), *(*(data + i)->info.s.p + j));
                            
                            len_buffer += len_buffer2;
                            buffer = realloc(buffer, len_buffer * sizeof(char));
                            strcat(buffer, buffer2);
                            free(buffer2);
                        }
                    }
                    strcat(buffer, "\n");

                    len += len_buffer;
                    output = realloc(output, len * sizeof(char));
                    strcat(output, buffer);
                    free(buffer);
                }
                break;
        }
    }
    return output;
}

char *ou_comparing_difficulty(ComparingDifficulty *data, unsigned int num) {
    char *output = strdup("");
    int len = 0;
    for (int i = 0; i < num; i++) {
        int size_floor = ou_comparing_size((int) *(data + i)->info);
        int trailing_zeros = ou_comparing_trailing(*(data + i)->info, size_floor);
        int len_buffer = strlen((data + i)->name) + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + (1 + 1);
        char *buffer = malloc(len_buffer * sizeof(char));
        snprintf(buffer, len_buffer, "%s:%.*f\n", (data + i)->name, trailing_zeros, *(data + i)->info);
        
        len += len_buffer;
        output = realloc(output, len * sizeof(char));
        strcat(output, buffer);
        free(buffer);
    }
    return output;
}

char *ou_comparing_general(ComparingGeneral *data, unsigned int num) {
    char *output = strdup("");
    int len = 0;
    for (int i = 0; i < num; i++) {
        switch ((data + i)->type) {
            case g_cp:
                if (*(data + i)->info.cp == NULL && *(data + i)->original.cp != NULL) {
                    *(data + i)->info.cp = strdup(*(data + i)->original.cp);
                }
                if ((*(data + i)->info.cp != NULL && *(data + i)->original.cp == NULL)
                    || (*(data + i)->info.cp != NULL && *(data + i)->original.cp != NULL && strcmp(*(data + i)->info.cp, *(data + i)->original.cp) != 0)) {
                    int len_buffer = strlen((data + i)->name) + 1 + 1 + strlen(*(data + i)->info.cp) + (1 + 1);
                    char *buffer = malloc(len_buffer * sizeof(char));
                    snprintf(buffer, len_buffer, "%s: %s\n", (data + i)->name, *(data + i)->info.cp);

                    len += len_buffer;
                    output = realloc(output, len * sizeof(char));
                    strcat(output, buffer);
                    free(buffer);
                }
                break;

            case g_n:
                if (*(data + i)->info.n != *(data + i)->original.n) {
                    int size_floor = ou_comparing_size(*(data + i)->info.n);
                    int len_buffer = strlen((data + i)->name) + 1 + 1 + size_floor + (1 + 1);
                    char *buffer = malloc(len_buffer * sizeof(char));
                    snprintf(buffer, len_buffer, "%s: %d\n", (data + i)->name, *(data + i)->info.n);

                    len += len_buffer;
                    output = realloc(output, len * sizeof(char));
                    strcat(output, buffer);
                    free(buffer);
                }
                break;

            case g_d:
                if (*(data + i)->info.d != *(data + i)->original.d) {
                    int size_floor = ou_comparing_size((int) *(data + i)->info.d);
                    int trailing_zeros = ou_comparing_trailing(*(data + i)->info.d, size_floor);
                    int len_buffer = strlen((data + i)->name) + 1 + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + (1 + 1);
                    char *buffer = malloc(len_buffer * sizeof(char));
                    snprintf(buffer, len_buffer, "%s: %.*f\n", (data + i)->name, trailing_zeros, *(data + i)->info.d);
                    
                    len += len_buffer;
                    output = realloc(output, len * sizeof(char));
                    strcat(output, buffer);
                    free(buffer);
                }
                break;

            case g_b:
                if (*(data + i)->info.b != *(data + i)->original.b) {
                    int len_buffer = strlen((data + i)->name) + 1 + 1 + 1 + (1 + 1);
                    char *buffer = malloc(len_buffer * sizeof(char));
                    snprintf(buffer, len_buffer, "%s: %d\n", (data + i)->name, *(data + i)->info.b);
                    
                    len += len_buffer;
                    output = realloc(output, len * sizeof(char));
                    strcat(output, buffer);
                    free(buffer);
                }
                break;
        }
    }
    return output;
}

char *ou_comparing_metadata(ComparingMetadata *data, unsigned int num) {
    char *output = strdup("");
    int len = 0;
    for (int i = 0; i < num; i++) {
        switch ((data + i)->type) {
            case m_cp: {
                int len_buffer = strlen((data + i)->name) + 1 + (1 + 1);
                char *buffer = malloc(len_buffer * sizeof(char));
                snprintf(buffer, len_buffer, "%s:", (data + i)->name);
                if (*(data + i)->info.cp != NULL) {
                    len_buffer += strlen(*(data + i)->info.cp);
                    buffer = realloc(buffer, len_buffer * sizeof(char));
                    strcat(buffer, *(data + i)->info.cp);
                }
                strcat(buffer, "\n");

                len += len_buffer;
                output = realloc(output, len * sizeof(char));
                strcat(output, buffer);
                free(buffer);
                break;
            }

            case m_s: {
                int len_buffer = strlen((data + i)->name) + 1 + (1 + 1);
                char *buffer = malloc(len_buffer * sizeof(char));
                snprintf(buffer, len_buffer, "%s:", (data + i)->name);
                if (*(data + i)->info.s.cpp != NULL || *(data + i)->info.s.ui != 0) {
                    for (int j = 0; j < *(data + i)->info.s.ui; j++) {
                        int len_buffer2 = 1 + (j != (*(data + i)->info.s.ui - 1) ? 1 : 0) + strlen(*(*(data + i)->info.s.cpp + j));
                        char *buffer2 = malloc(len_buffer2 * sizeof(char));
                        snprintf(buffer2, len_buffer2, (j != (*(data + i)->info.s.ui - 1) ? "%s " : "%s"), *(*(data + i)->info.s.cpp + j));
                        
                        len_buffer += len_buffer2;
                        buffer = realloc(buffer, len_buffer * sizeof(char));
                        strcat(buffer, buffer2);
                        free(buffer2);
                    }
                }
                strcat(buffer, "\n");

                len += len_buffer;
                output = realloc(output, len * sizeof(char));
                strcat(output, buffer);
                free(buffer);
                break;
            }

            case m_n: {
                int size_floor = (*(data + i)->info.n == 0 ? 1 : (floor(log10(abs(*(data + i)->info.n))) + 1 + (*(data + i)->info.n < 0 ? 1 : 0)));
                int len_buffer = strlen((data + i)->name) + 1 + size_floor + (1 + 1);
                char *buffer = malloc(len_buffer * sizeof(char));
                snprintf(buffer, len_buffer, "%s:%d\n", (data + i)->name, *(data + i)->info.n);

                len += len_buffer;
                output = realloc(output, len * sizeof(char));
                strcat(output, buffer);
                free(buffer);
                break;
            }
        }
    }
    return output;
}