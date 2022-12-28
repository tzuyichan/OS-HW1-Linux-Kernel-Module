#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048
#define FAIL_IF(EXP,MSG)({if(EXP){fprintf(stderr,MSG"\n");exit(EXIT_FAILURE);}})

struct offset
{
    const char *begin;
    const char *end;
};

enum {V, C, M, T, NUM_OF_MODES} mode = V;

void get_sysinfo_offsets(struct offset *pos, const char *buf);

int main()
{
    FILE *file;
    char buf[BUF_SIZE];
    struct offset pos[NUM_OF_MODES];
    char cmd, ch;

    while (1)
    {
        printf("Which information do you want?\n");
        printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
        scanf(" %c", &cmd);

        if (cmd == 'e') return 0;

        memset(buf, 0, BUF_SIZE);  // clear buffer so all chars are initialized

        file = fopen("/proc/my_info", "r");
        FAIL_IF(!file, "Failed to open file.");
        fread(buf, sizeof(char), BUF_SIZE, file);
        fclose(file);

        get_sysinfo_offsets(pos, buf);

        switch (cmd)
        {
        case 'v':
            printf("\nVersion: %.*s\n\n",
                   (int)(pos[V].end - pos[V].begin), pos[V].begin);
            break;
        case 'c':
            printf("\nCPU information:\n%.*s\n\n",
                   (int)(pos[C].end - pos[C].begin), pos[C].begin);
            break;
        case 'm':
            printf("\nMemory information:\n%.*s\n\n",
                   (int)(pos[M].end - pos[M].begin), pos[M].begin);
            break;
        case 't':
            printf("\nTime information:\n%.*s\n\n",
                   (int)(pos[T].end - pos[T].begin), pos[T].begin);
            break;
        case 'a':
            printf("%s", buf);
            break;
        default:
            ;
        }
        printf("------------------------------------------------------------\n\n");
        while ((ch = getchar()) != '\n' && ch != EOF);  // flush input buffer
        mode = V;
    }

    return 0;
}

void get_sysinfo_offsets(struct offset *pos, const char *buf)
{
    const char *p = buf + 2;  // skip first 2 chars '\n='
    while (*p != '\0')
    {
        if (*(p - 1) == '\n' && *(p - 2) == '=')
            pos[mode].begin = p;

        if (*(p + 1) == '\n' && *(p + 2) == '=')
            pos[mode++].end = p;

        p++;
    }
    pos[mode].end = p - 2;  // skip last 2 chars '\n\0'
}
