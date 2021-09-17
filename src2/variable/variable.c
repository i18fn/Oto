#include "variable.h"

#include "type.h"
#include "../token/token.h"
#include "../token/tokencode.h"

// トークン番号と変数の場所は対応している
#define MAX_TC 4096
static var_t var_list[MAX_TC] = {0};

var_t *get_var_pointer(tokencode_t tc) {
    return &var_list[tc];
}

type_t get_type(tokencode_t tc) {
    return var_list[tc].type;
}

int64_t get_ivalue(tokencode_t tc) {
    return var_list[tc].value.iVal;
}

double get_fvalue(tokencode_t tc) {
    return var_list[tc].value.fVal;
}

void *get_pvalue(tokencode_t tc) {
    return var_list[tc].value.pVal;
}

void assign_int(tokencode_t tc, type_t type, int64_t value) {
    var_list[tc].tc   = tc;
    var_list[tc].type = type;
    var_list[tc].value.iVal = value;
}

void assign_float(tokencode_t tc, type_t type, double value) {
    var_list[tc].tc   = tc;
    var_list[tc].type = type;
    var_list[tc].value.fVal = value;
}

void assign_pointer(tokencode_t tc, type_t type, void *value) {
    var_list[tc].tc   = tc;
    var_list[tc].type = type;
    var_list[tc].value.pVal = value;
}