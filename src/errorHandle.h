#pragma once

#include <stdint.h>
#include "utils/util.h"
#include "lexer/token.h"
#include "variable/variable.h"

// exception
enum {
    ERROR = 0,              // 謎のエラー        
    TYPE_ERROR,             // 型の不一致
    FLOAT_MODULO_ERROR,     // float型の余りを計算
    ZERO_DIVISION_ERROR,    // 0除算
    STACK_OVERFLOW_ERROR    // 内部スタックのオーバーフロー
};

void setFname(str_t filename);
void setSrc(str_t src);
void setTcBuf(tokenBuf_t *tcBuf);
void setVar(var_t *var);
void setIc(var_t **ic);

void exception(int32_t errorCode);