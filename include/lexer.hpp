#ifndef LEXER_HPP
#define LEXER_HPP

#include <stdio.h>
#include <cstdlib>
#include <string.h>

#include "types.hpp"

class Lexer
{
private:
    static const int m_HASH_SIZE = 200000;
    char *m_table[m_HASH_SIZE];

public:
    Lexer();
    ~Lexer();

    int hash(char *s);
    int index(char *s, bool insert);
    UL lex(FILE *fp);
    void print_ul(UL ul);
};

#endif
