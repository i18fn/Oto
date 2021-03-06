#pragma once

#pragma once

#include <stdint.h>

// language
enum {
    LANG_JPN_KANJI = 0,
    LANG_JPN_HIRAGANA,
    LANG_ENG
};
typedef int64_t language_t;

// Errorcodes
enum {
    OTO_NO_ERROR = 0,
    OTO_UNKNOWN_ERROR = -10000,
    OTO_INTERNAL_ERROR,
    
    OTO_SYNTAX_ERROR,
    OTO_UNAVAILABLE_OPERATOR_ERROR,

    OTO_PREPROCESS_ERROR,
    OTO_FILE_NOT_FOUND_ERROR,
    OTO_INCLUDE_FILE_NOT_FOUND_ERROR,
    OTO_CIRCULAR_REFERENCE_ERROR,
    
    OTO_INVALID_SYNTAX_ERROR,
    OTO_DEFINE_ERROR,
    OTO_NAME_ERROR,
    OTO_FILTER_ERROR,
    OTO_ASSIGN_TO_LITERAL_ERROR,
    OTO_TOO_MANY_ARGUMENTS_ERROR,
    OTO_MISSING_ARGUMENTS_ERROR,
    OTO_ARGUMENTS_TYPE_ERROR,

    // 将来削除予定
    OTO_EXIST_SOUND_OBJECT_ERROR,

    OTO_STACK_OVERFLOW_ERROR,
    OTO_ZERO_DIVISION_ERROR,
    OTO_SOUND_PLAYER_ERROR,
    OTO_SYHTH_OUT_OF_NUMBER_ERROR,

    OTO_REPL_ERROR
};
typedef int64_t errorcode_t;

#define DEFAULT_MAX_TC 1024

// Tokencodes
enum {
    TC_LF = 0,    // \n
    TC_COMMA,     // ,
    TC_COLON,     // :
    TC_SQBROPN,   // [
    TC_SQBRCLS,   // ]
    TC_BROPN,     // (
    TC_BRCLS,     // )
    TC_LARROW,    // <-
    TC_RARROW,    // ->
    TC_EQU,       // =
    TC_PLUS,      // +
    TC_MINUS,     // -
    TC_ASTER,     // *
    TC_SLASH,     // /
    TC_PERCE,     // %
    TC_EEQ,       // '=='
    TC_NEQ,       // '!='
    TC_GT,        // '>'
    TC_LT,        // '<'
    TC_GE,        // '>='
    TC_LE,        // '<='
    TC_PLUSEQ,    // +=
    TC_MINUEQ,    // -=
    TC_ASTEEQ,    // *=
    TC_SLASEQ,    // /=
    TC_PERCEQ,    // %=

    TC_BEGIN,     // begin BEGIN
    TC_END,       // end END
    TC_DEFINE,    // define DEFINE
    TC_IF,        // if IF
    TC_ELSIF,     // elsif ELSIF
    TC_ELSE,      // else ELSE
    TC_THEN,      // then THEN
    TC_LOOP,      // loop LOOP
    TC_AND,       // and AND
    TC_OR,        // or OR
    TC_NOT,       // not NOT
    TC_FUNC,      // func FUNC
    TC_TRACK,     // track TRACK
    TC_FILTER,    // filter FILTER
    TC_OSCIL,     // oscil OSCIL
    TC_SOUND,     // sound SOUND

    TC_PRINT,     // print PRINT
    TC_BEEP,      // beep BEEP
    TC_PLAY,      // play PLAY
    TC_PRINTWAV,  // printwav PRINTWAV
    TC_PRINTVAR,  // printvar PRINTVAR
    TC_SLEEP,     // sleep SLEEP
    TC_SETSYNTH,  // setsynth SETSYNTH
    TC_SETLOOP,   // setloop SETLOOP
    TC_STOP,      // stop STOP

    TC_EXIT       // exit EXIT
};
typedef int64_t tokencode_t;

// Token types
enum {
    TK_TY_SYMBOL = 0,
    TK_TY_RSVWORD,
    TK_TY_LITERAL,
    TK_TY_STRING,
    TK_TY_VARIABLE
};
typedef int64_t tokentype_t;

// Variable types
enum {
    TY_UNABLE = -1,  // 変数として使用不可能なもの
    TY_VOID = 0,
    TY_INITVAL,  // 引数が初期値であるもの
    TY_CONST,
    TY_FLOAT,
    TY_ARRAY,
    TY_STRING,
    TY_OSCIL,
    TY_SOUND,
    TY_FILTER,
    TY_FUNC
};
typedef int64_t vartype_t;

// operation codes
enum {
    OP_NOP = 0,    // 何もしない

    /**
     * 変数への単純代入
     * 
     * example:
     *   CPYD Var1 Var2
     *   Var1にVar2の内容をコピーして代入する
     */
    OP_CPYD,

    /**
     * スタックからポップしたものを変数に代入
     * 
     * example:
     *   CPYD Var1
     *   Var1にスタックの先頭の内容をコピーして代入する
     */
    OP_CPYP,

    /* スタックにプッシュする */
    OP_PUSH,

    /**
     * 引数が省略されたので初期値を用いる
     */
    OP_PUSH_INITVAL,
    
    /**
     * スタックからポップしたものを変数に代入
     * 
     * example:
     *   CPYP Var1
     *   Var1にスタックからポップしてきたものを代入する
     */
    OP_POP,

    /**
     * 算術演算命令
     * スタックの上から2つをポップして演算し, プッシュする
     * 
     * note:
     *   先にプッシュしたほうが初めの項となる
     */
    OP_ADD,        // 加算
    OP_SUB,        // 引算
    OP_MUL,        // 掛算
    OP_DIV,        // 割算
    OP_MOD,        // 余り
    OP_AND,        // 論理積
    OP_OR,         // 論理和
    OP_EQ,         // == 
    OP_NEQ,        // !=
    OP_LTCMP,      // <
    OP_LTEQCMP,    // <=
    OP_RICMP,      // >
    OP_RIEQCMP,    // >=
    
    /**
     * 二項演算命令
     * 引数に指定した変数同士を演算をする. 算術演算命令より速い.
     * 
     * example:
     *   ADD Var1 Var2 Var3
     *   Var1に, Var2とVar3を加算したものを代入する
     */
    OP_ADD2,       // 二項加算
    OP_SUB2,       // 二項引算
    OP_MUL2,       // 二項掛算
    OP_DIV2,       // 二項割算
    OP_MOD2,       // 二項余り

    /**
     * ループ命令
     * 指定した回数ループを行う
     * 
     * example:
     *   LOOP Addr Loop_Cnt Loop_Var
     *   Addrをループ場所として, Loop_Cnt回分ループする.
     *   Loop_Varは制御変数である.
     */
    OP_LOOP,

    /**
     * 分岐命令
     * 
     * 条件が成立していれば, 指定した場所へジャンプする
     */
    OP_JMP,        // 無条件ジャンプ
    OP_JZ,         // スタックの上が0ならジャンプ
    OP_JNZ,        // スタックの上が0でないならジャンプ

    OP_OSCILDEF,
    OP_SOUNDDEF,
    OP_ARRAYDEF,

    OP_CPYS,

    /**
     * フィルターの接続
     * 
     * CONNFILTER <sound> <filter>
     */
    OP_CONNFILTER,

    OP_PRINT,
    OP_BEEP,
    OP_PLAY,
    OP_PRINTWAV,
    OP_PRINTVAR,
    OP_SLEEP,
    OP_SETSYNTH,
    OP_SETLOOP,
    OP_STOP,

    OP_EXIT
};
typedef int64_t opcode_t;

#define FILTER_NUM 13
enum {
    CLIP = 0,
    FADE_IN,
    FADE_OUT,
    FADE,
    AMP,
    TREMOLO,
    DETUNE,
    CHOP,
    LPF,
    HPF,
    WAH,
    RADIO,
    VIBRATO
};
typedef int64_t filtercode_t;