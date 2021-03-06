#include "vm.h"

/* 浮動小数点数型で余りや論理演算を行うための共用体 */
typedef union {
    int64_t i;
    double  f;
} i64double_u;

bool is_just_zero(double val) {
    i64double_u v = {0};
    v.f = val;
    return (v.i & -1) == 0;
}

void alu(opcode_t op) {
    /* TODO: float型での比較・論理演算が少し怪しい */
    i64double_u val1 = {0};
    i64double_u val2 = {0};

    /* 先にポップする方が第2項 */
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        val2.f = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        val2.f = vmstack_popf();
    } else {
        oto_error(OTO_UNKNOWN_ERROR);
    }

    if (vmstack_typecheck() == VM_TY_VARPTR) {
        val1.f = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        val1.f = vmstack_popf();
    } else {
        oto_error(OTO_UNKNOWN_ERROR);
    }

    switch (op) {
    case OP_ADD:
        vmstack_pushf(val1.f + val2.f);
        break;

    case OP_SUB:
        vmstack_pushf(val1.f - val2.f);
        break;

    case OP_MUL:
        vmstack_pushf(val1.f * val2.f);
        break;

    case OP_DIV:
        if (is_just_zero(val2.f)) {
            oto_error(OTO_ZERO_DIVISION_ERROR);
        }
        vmstack_pushf(val1.f / val2.f);
        break;

    case OP_MOD:
        if ((int64_t)val2.f == 0) {
            oto_error(OTO_ZERO_DIVISION_ERROR);
        }
        vmstack_pushf((double)((int64_t)val1.f % (int64_t)val2.f));
        break;

    case OP_AND:
        vmstack_pushi(val1.i && val2.i);
        break;

    case OP_OR:
        vmstack_pushi(val1.i || val2.i);
        break;

    case OP_EQ:
        vmstack_pushi(val1.f == val2.f);
        break;

    case OP_NEQ:
        vmstack_pushi(val1.f != val2.f);
        break;

    case OP_LTCMP:
        vmstack_pushi(val1.f < val2.f);
        break;

    case OP_LTEQCMP:
        vmstack_pushi(val1.f <= val2.f);
        break;

    case OP_RICMP:
        vmstack_pushi(val1.f > val2.f);
        break;

    case OP_RIEQCMP:
        vmstack_pushi(val1.f >= val2.f);
        break;

    default:
        oto_error(OTO_UNKNOWN_ERROR);
    }
}
