#include "function.h"
#include "data.h"
#include <unistd.h>

// copy file alter .xx suffix
char *alter_suffix(char *str, char suffix)
{
    char *outfilename = my_strdup(str);
    char *p;
    if (!outfilename)
        return NULL;
    if ((p = strchr(outfilename, '.')) == NULL)
        return NULL;
    p++;
    if (*p == '\0')
        return NULL;
    *p++ = suffix;
    *p = '\0';
    return outfilename;
}
// -s
static char *do_compile(char *filename)
{
    char *sfile = alter_suffix(filename, 's');
    if (sfile == NULL)
        custom_error_chars("Error: %s has no suffix, try .c on the end\n", filename);

    if (!(Infile = fopen(filename, "r")))
        custom_error_chars("open infile", filename);
    if (!(Outfile = fopen(sfile, "w")))
        custom_error_chars("open Outfile", sfile);

    Line = 1;
    cache = 0;
    clear_symtable();
    if (O_verbose)
        printf("compiling %s\n", filename);

    scan(&t_instance);
    genpreamble();
    global_declarations();
    genpostamble();
    fclose(Outfile);
    return sfile;
}
// -c
char *do_assemble(char *filename)
{
    char cmd[TEXTLEN], err;
    char *ofile = alter_suffix(filename, 'o');
    if (ofile == NULL)
        custom_error_chars("Error: %s has no suffix, try .s on the end\n", ofile);
    snprintf(cmd, TEXTLEN, "%s %s %s", ASCMD, ofile, filename);
    if (O_verbose)
        printf("assembling %s\n", cmd);
    err = system(cmd);
    if (err)
        custom_error_chars("Assembly of %s failed\n", filename);
    return ofile;
}
// -o
void do_link(char *outfilename, char *objlist[])
{
    char cmd[TEXTLEN], cnt, err;
    int size = TEXTLEN;
    char *p = cmd;
    cnt = snprintf(p, size, "%s %s ", LDCMD, outfilename);
    size -= cnt;
    p += cnt;
    while (*objlist)
    {
        cnt = snprintf(p, size, "%s ", *objlist);
        size -= cnt;
        p += cnt;
        objlist++;
    }
    if (O_verbose)
        printf("linking %s\n", cmd);
    err = system(cmd);
    if (err)
    {

        fprintf(stderr, "");
        custom_error_chars("Linking failed", "");
    }
}

// Print out a usage if started incorrectly
static void usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-vcST] [-o outfile] file [file ...]\n", prog);
    fprintf(stderr, "       -v give verbose output of the compilation stages\n");
    fprintf(stderr, "       -c generate object files but don't link them\n");
    fprintf(stderr, "       -S generate assembly files but don't link them\n");
    fprintf(stderr, "       -T dump the AST trees for each input file\n");
    fprintf(stderr, "       -o outfile, produce the outfile executable file\n");
    exit(1);
}

#define MAXOBJ 100 // max sourcefile
int main(int argc, char *argv[])
{
    char *outfilename = AOUT;
    char *asmfile, *objfile;
    char *objlist[MAXOBJ]; // List of.obj files
    int objcnt = 0;
    O_dumpAST = O_keepasm = O_assemble = O_verbose = 0;
    O_dolink = 1;
    int i, j;
    for (i = 1; i < argc; i++)
    {
        if (*argv[i] != '-')
            break;
        for (j = 1; *argv[i] == '-' && argv[i][j]; j++)
        {
            switch (argv[i][j])
            {
            case 'o':
                outfilename = argv[++i];
                break;
            case 'S':
                O_keepasm = 1;
                O_dolink = O_assemble = 0;
                break;
            case 'c':
                O_assemble = 1;
                O_dolink = O_keepasm = 0;
                break;
            case 'v':
                O_verbose = 1;
                break;
            case 'T':
                O_dumpAST = 1;
                break;
            default:
                usage(argv[0]);
            }
        }
    }
    if (i >= argc)
        custom_error_chars("May not have input infile", (char *)argv[0]);

    while (i < argc)
    {
        asmfile = do_compile(argv[i]);
        if (O_dolink || O_assemble)
        {
            objfile = do_assemble(asmfile);
            if (objcnt >= MAXOBJ - 2)
                custom_error_int("The source file is out of the parsing range", objcnt);
            objlist[objcnt++] = objfile;
            objlist[objcnt] = NULL;
        }
        if (!O_keepasm)
            unlink(asmfile);
        i++;
    }

    if (O_dolink)
        do_link(outfilename, objlist);
    if (!O_assemble)
        for (i = 0; i < objcnt; i++)
            unlink(objlist[i]);

    return 0;
}
