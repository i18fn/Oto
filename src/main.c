/**
 * Oto - Sound Programming Language
 * 
 * author : Furuta Naoki
 *          2021/05/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "run.h"

/* 使い方表示用の関数 */
void usage(const char *name) {
    // TODO: そのうち真面目に書く
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, const char **argv)
{
    if (argc <= 1) {
        usage(argv[0]);
        exit(1);
    }
    
    // ソースファイルを確保しておく領域
    String src = (String)malloc(sizeof(char) * SOURCE_BUF_SIZE);
    
    if (srcLoad((String)argv[1], src, SOURCE_BUF_SIZE) == 0) {
        run(src);
    }

    free(src);

    return 0;
}