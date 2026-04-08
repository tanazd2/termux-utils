#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <script> -o <output.c>\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {
    if(argc != 4 || strcmp(argv[2], "-o") != 0)
        usage(argv[0]);

    const char *input = argv[1];
    const char *output = argv[3];

    FILE *in = fopen(input, "r");
    if(!in) { perror("Error opening input file"); return 1; }

    FILE *out = fopen(output, "w");
    if(!out) { perror("Error opening output file"); fclose(in); return 1; }

    fprintf(out, "#include <stdlib.h>\n\nint main() {\n");

    char line[4096];
    while(fgets(line, sizeof(line), in)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if(len && line[len-1]=='\n') line[len-1]='\0';

        // Escape quotes and backslashes
        fprintf(out, "system(\"");
        for(char *p=line; *p; p++) {
            if(*p == '\\' || *p == '"') fputc('\\', out);
            fputc(*p, out);
        }
        fprintf(out, "\");\n");
    }

    fprintf(out, "return 0;\n}\n");

    fclose(in);
    fclose(out);

    printf("✅ Generated C file: %s\n", output);
    return 0;
}
