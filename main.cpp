#include "include/lexer.hpp"

int main(const int argc, const char **argv)
{
    // Open the file
    FILE *fp;

    // Check if the file name is missing
    if (argc < 2)
    {
        printf("Error: the file name is missing\n");
        return 1;
    }

    else
        fp = fopen(argv[1], "r");

    // Check if the file could not be opened
    if (fp == NULL)
    {
        printf("Error: the file could not be opened\n");
        return 1;
    }

    Lexer lexer = Lexer();

    // Read the file
    UL ul = lexer.lex(fp);
    while (ul.type != END_OF_FILE)
    {
        lexer.print_ul(ul);
        ul = lexer.lex(fp);
    }

    // Close the file
    fclose(fp);

    // Lexer lexer = Lexer();
    // int id = lexer.index("NADP", true);
    // printf("id: %d\n", id);

    // id = lexer.index("NADPj", false);
    // printf("id: %d\n", id);

    return 0;
}