#include "function.h"
#include "data.h"
#include <unistd.h>

// copy file . suffix
char *alter_suffix(char *str, char suffix) {}
// -s
static char *do_compile(char *filename) {}
// -c
char *do_assemble(char *filename) {}
// -o
void do_link(char *outfilename, char *objlist[]) {}

void init()
{
    Line = 1;
    cache = 0;
    Globals = 0;
    Locls = NSYMBOLS - 1;
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
int main(int argc, char const *argv[])
{

    int outfilename = AOUT;
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

    // if (!(Infile = fopen(argv[i], "r")))
    //     custom_error_chars("open infile", (char *)argv[i]);
    // if (!(Outfile = fopen("out.s", "w")))
    //     custom_error_chars("open Outfile", (char *)argv[i]);

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
    // addglob("printint", P_CHAR, S_FUNCTION, C_GLOBAL, 0, 0);
    // addglob("printchar", P_VOID, S_FUNCTION, C_GLOBAL, 0, 0);

    // scan(&t_instance);
    // genpreamble();
    // global_declarations();

    // genpostamble();
    // fclose(Outfile);
}
