#include <stdio.h>
#include <string.h>

#define MAX_LINE 4096

void write_json_string(FILE *out, const char *str)
{
    while (*str)
    {
        switch (*str)
        {
            case '\\':
                fputs("\\\\", out);
                break;

            case '"':
                fputs("\\\"", out);
                break;

            case '\n':
                fputs("\\n", out);
                break;

            case '\r':
                break;

            case '\t':
                fputs("\\t", out);
                break;

            default:
                fputc(*str, out);
        }

        str++;
    }
}

int main()
{
    FILE *csv = fopen("words.csv", "r");

    if (csv == NULL)
    {
        printf("words.csv bulunamadi.\n");
        return 1;
    }

    FILE *json = fopen("words.json", "w");

    if (json == NULL)
    {
        printf("words.json olusturulamadi.\n");
        fclose(csv);
        return 1;
    }

    char line[MAX_LINE];

    /* Header satırını atla */
    fgets(line, sizeof(line), csv);

    fprintf(json, "[\n");

    int first = 1;

    while (fgets(line, sizeof(line), csv))
    {
        char word[256] = "";
        char meaning[256] = "";
        char example[2048] = "";

        line[strcspn(line, "\r\n")] = '\0';

        char *p = line;

        char *comma = strchr(p, ',');

        if (!comma)
            continue;

        *comma = '\0';

        strcpy(word, p);

        p = comma + 1;

        comma = strchr(p, ',');

        if (!comma)
            continue;

        *comma = '\0';

        strcpy(meaning, p);

        p = comma + 1;

        if (*p == '"')
        {
            p++;

            char *last = strrchr(p, '"');

            if (last)
                *last = '\0';
        }

        strcpy(example, p);

        if (!first)
            fprintf(json, ",\n");

        first = 0;

        fprintf(json, "    {\n");

        fprintf(json, "        \"word\":\"");
        write_json_string(json, word);
        fprintf(json, "\",\n");

        fprintf(json, "        \"meaning\":\"");
        write_json_string(json, meaning);
        fprintf(json, "\",\n");

        fprintf(json, "        \"example\":\"");
        write_json_string(json, example);
        fprintf(json, "\"\n");

        fprintf(json, "    }");
    }

    fprintf(json, "\n]\n");

    fclose(csv);
    fclose(json);

    printf("words.json olusturuldu.\n");

    return 0;
}