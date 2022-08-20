#ifndef OU_COMPARING_H
#define OU_COMPARING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int ou_comparing_size(int);
int ou_comparing_trailing(double, int);

typedef struct ComparingEditor {
    char *name;
    union {
        int *n;
        double *d;
        struct {
            int **p;
            unsigned int *n;
        } s;
    } info;
    enum {
        e_n,
        e_d,
        e_s,
    } type;
} ComparingEditor;
char *ou_comparing_editor(ComparingEditor *, unsigned int);

typedef struct ComparingDifficulty {
    char *name;
    double *info;
} ComparingDifficulty;
char *ou_comparing_difficulty(ComparingDifficulty *, unsigned int);

typedef struct ComparingGeneral {
    char *name;
    union {
        char **cp;
        int *n;
        double *d;
        bool *b;
    } info;
    union {
        char **cp;
        int *n;
        double *d;
        bool *b;
    } original;
    enum {
        g_cp,
        g_n,
        g_d,
        g_b,
    } type;
} ComparingGeneral;
char *ou_comparing_general(ComparingGeneral *, unsigned int);

typedef struct ComparingMetadata {
    char *name;
    union {
        char **cp;
        struct {
            char ***cpp;
            unsigned int *ui;
        } s;
        int *n;
    } info;
    enum {
        m_cp,
        m_s,
        m_n,
    } type;
} ComparingMetadata;
char *ou_comparing_metadata(ComparingMetadata *, unsigned int);

#endif