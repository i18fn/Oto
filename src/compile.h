#ifndef INCLUDED_COMPILE
#define INCLUDED_COMPILE 1

#include "util.h"
#include "token.h"
#include "variable.h"

/**
 * ic[]に書き込むための関数
 * 
 * args:
 *      ic      : 内部コード列
 *      icp     : 現在指しているicのインデックス
 *                中で加算したりするのでポインタを渡してね
 *      op      : 処理を表す内部コード
 *      v1 ~ v4 : 渡す値(ポインタ)
 */
void putIc(int **ic, int *icp, int op, int *v1, int *v2, int *v3, int *v4);

/* 内部コードに変換する関数 */
int compile(String s, tokenBuf_t *tcBuf, var_t **var, int **ic);

#endif