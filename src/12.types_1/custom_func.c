void custom_error_line()
{
    printf("The source code error is on line %d\n", Line);
    exit(1);
}

void custom_error_char(char *t, char c)
{
    printf("ERROR: %s ,char: %c\n", t, c);
    custom_error_line();
}

void custom_error_chars(char *c1, char *c2)
{
    printf("ERROR: %s ,char*: %s\n", c1, c2);
    custom_error_line();
};

void custom_error_int(char *c1, int i)
{
    printf("ERROR: %s ,int: %d\n", c1, i);
    custom_error_line();
}