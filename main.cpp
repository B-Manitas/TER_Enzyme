#include "include/lexer.hpp"
#include "include/parser.hpp"

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
    Parser parser = Parser();
    std::vector<UL> data_tokenized = lexer.lex_all(fp);

    std::vector<instr> instructions = std::vector<instr>{};
    std::vector<react> reactions = std::vector<react>{};

    parser.parse(data_tokenized, reactions, instructions);

    parser.print_react(reactions);
    parser.print_instr(instructions);

    fclose(fp);

    return 0;
}