#include <oto/oto.h>

void usage(const char *name) {
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, char **argv) {    
    uint32_t i = 1;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
        return 0;
    }

    // ファイル名が指定されていない場合
    if (argc != i + 1) {
        oto_init(NULL);
        oto_run(NULL);
    } else {
        oto_init(argv[i]);
        oto_run(argv[i]);
    }

    return 0;
}
