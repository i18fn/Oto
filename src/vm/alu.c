#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ic.h"
#include "../errorHandle.h"
#include "../utils/util.h"
#include "../variable/variable.h"

var_t add2(var_t v1, var_t v2) {
    var_t t;
    t.type = TyConstF;

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal + v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal + v2.value.fVal;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal + (double)v2.value.iVal;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal + v2.value.fVal;
        }
    }

    return t;
}

var_t sub2(var_t v1, var_t v2) {
    var_t t;
    t.type = TyConstF;

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal - v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal - v2.value.fVal;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal - (double)v2.value.iVal;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal - v2.value.fVal;
        }
    }

    return t;
}

var_t mul2(var_t v1, var_t v2) {
    var_t t;
    t.type = TyConstF;

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal * v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal * v2.value.fVal;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal * (double)v2.value.iVal;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal * v2.value.fVal;
        }
    }

    return t;
}

var_t div2(var_t v1, var_t v2) {
    var_t t;
    t.type       = TyConstF;

    if (v2.value.iVal == 0) {
        callException(ZERO_DIVISION_ERROR);
    }

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal / v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal / v2.value.fVal;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal / (double)v2.value.iVal;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal / v2.value.fVal;
        }
    }
    return t;
}

var_t mod2(var_t v1, var_t v2) {
    var_t t;

    if (v2.value.iVal == 0) {
        callException(ZERO_DIVISION_ERROR);
    }

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal % v2.value.iVal;
            t.type       = TyConstI;
        } else {
            callException(FLOAT_MODULO_ERROR);
        }
    } else {
        callException(FLOAT_MODULO_ERROR);
    }

    return t;
}

var_t calculation(var_t v1, var_t v2, int32_t opcode) {
    var_t t;

    switch (opcode) {
    case OpAdd:
        t = add2(v1, v2);
        break;
    case OpSub:
        t = sub2(v1, v2);
        break;
    case OpMul:
        t = mul2(v1, v2);
        break;
    case OpDiv:
        t = div2(v1, v2);
        break;
    case OpMod:
        t = mod2(v1, v2);
        break;
    case OpAnd:
        t.type = TyConstI;
        t.value.iVal = v1.value.iVal & v2.value.iVal;
        break;
    case OpOr:
        t.type = TyConstI;
        t.value.iVal = v1.value.iVal | v2.value.iVal;
        break;
    default:
        callException(ERROR);
    }

    return t;
}

int32_t compare(var_t v1, var_t v2, int32_t opcode) {
    int64_t v1Val;
    int64_t v2Val;

    if (v1.type == TyFloat || v1.type == TyConstF) {
        v1Val = (int64_t)v1.value.fVal;
    } else {
        v1Val = v1.value.iVal;
    }

    if (v2.type == TyFloat || v2.type == TyConstF) {
        v2Val = (int64_t)v2.value.fVal;
    } else {
        v2Val = v2.value.iVal;
    }

    switch (opcode) {
        case OpEq:
            return v1Val == v2Val;
        case OpNEq:
            return v1Val != v2Val;
        case OpLtCmp:
            return v1Val < v2Val;
        case OpLtEqCmp:
            return v1Val <= v2Val;
        case OpRiCmp:
            return v1Val > v2Val;
        case OpRiEqCmp:
            return v1Val >= v2Val;
        default:
            return 0;
    }
}