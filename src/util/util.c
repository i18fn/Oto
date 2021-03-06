#include <oto/oto_util.h>

static FILE *open_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (IS_NULL(fp)) {
        return NULL;
    }
    return fp;
}

size_t count_file_size(const char *path) {
    FILE *fp = open_file(path);
    if (IS_NULL(fp)) {
        return 0;
    }

    size_t size = 0;
    for (;;) {
        int32_t ch = fgetc(fp);
        if (ch == EOF) {
            break;
        }
        size++;
    }

    fclose(fp);

    return size;
}

char *src_open(const char *path) {
    if (IS_NULL(path)) {
        return NULL;
    }
    
    FILE *fp = open_file(path);
    if (fp == 0) {
        return NULL;
    }

    size_t fsize = count_file_size(path);
    if (fsize == 0) {
        return NULL;
    }

    // srcの終端に'\0'を入れるために(fsize + 1)
    char *src = MYMALLOC((fsize + 1), char);
    if (IS_NULL(src)) {
        return NULL;
    }

    fread(src, sizeof(char), fsize, fp);
    src[fsize] = 0;

    fclose(fp);

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %I64d bytes\n\n", path, fsize);
#endif

    return src;
}

bool is_otofile(const char *path) {
    const char *ext = strrchr(path, '.');

    if (strcmp(ext, ".oto") == 0) return true;
    return false;
}

/* String functions */

char to_lower(char ch) {
    if ('A' <= ch && ch <= 'Z') {
        return ch + 0x20;
    }
    return ch;
}

char to_upper(char ch) {
    if ('a' <= ch && ch <= 'z') {
        return ch - 0x20;
    }
    return ch;
}

int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount) {
    int64_t i = 0;
    while (i < maxcount) {
        if (to_lower(str1[i]) != to_lower(str2[i])) {
            return str1[i] - str2[i];
        }
        i++;
    }

    return 0;
}

int32_t strcmp_cs(const char *str1, const char *str2) {
    int64_t i = 0;
    
    while (to_lower(str1[i]) == to_lower(str2[i])) {
        i++;
        if (str1[i] == '\0' && str2[i] == '\0') {
            return 0;
        }
    }

    return str1[i] - str2[i];
}

bool is_str_equal_cs(const char *str1, size_t s1len, const char *str2, size_t s2len) {
    if (s1len == s2len && strncmp_cs(str1, str2, s1len) == 0) {
        return true;
    }
    return false;
}

void print_upto_char(char *str, char endch) {
    int64_t i = 0;
    while (str[i] != endch && str[i] != '\0') {
        printf("%c", str[i]);
        i++;
    }
}

void print_line(char *src, int64_t line) {
    int64_t l = 1;

    int64_t i = 0;
    while (l != line) {
        if (src[i] == '\n') {
            l++;
        } else if (src[i] == '\0') {
            break;
        }
        i++;
    }

    print_upto_char(&src[i], '\n');
    printf("\n");
}

int64_t get_current_line(char *src, int64_t idx) {
    int64_t line = 1;
    
    int64_t i = 0;
    while (i < idx) {
        if (src[i] == '\n') {
            line++;
        } else if (src[i] == '\0') {
            break;
        }
        i++;
    }

    return line;
}

/* idxは最初の「"」の位置 */
char *new_string_literal(char *src, int64_t idx) {
    if (src[idx] != '"') {
        return NULL;
    }
    idx++;

    size_t len = 0;
    while (src[idx + len] != '"') {
        len++;
        if (src[idx + len] == '\0') {
            return NULL;
        }
    }

    char *str = MYMALLOC(len + 1, char);
    if (IS_NULL(str)) {
        return NULL;
    }
    
    strncpy(str, &src[idx], len);

    return str;
}