#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "variable.h"

int initVar(var_t **var, int i, int type, int value)
{
    var_t *v = (var_t *)malloc(sizeof(var_t));
    if (v == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }

    v->type  = type;
    v->value = value;

    var[i] = v;

    return 0;
}