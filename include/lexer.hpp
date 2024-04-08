#ifndef LEXER_HPP
#define LEXER_HPP

#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <vector>

#include "types.hpp"

/**
 * @brief The Lexer class
 * 
 * This class is responsible for the lexical analysis of the input file.
 * The lexical analysis is the process of converting a sequence of characters into a sequence of tokens.
 * The tokens are the smallest units of the language.
 * 
 * @param m_table The hash table
 */
class Lexer
{
public:
    /* The size of the hash table */
    static const int m_HASH_SIZE = 200000;

    /* The hash table */
    char *m_table[m_HASH_SIZE] = {0};

    // Constructor and destructor
    Lexer();
    ~Lexer();

    // Methods
    /**
     * @brief The hash function
     *
     * @param s The string to hash
     * @return int The hash of the string
     */
    int hash(char *s);
    /**
     * @brief The index function
     *
     * @param s The string to index
     * @param insert If the string should be inserted in the table
     * @return int The index of the string. -1 if the string is not in the table. -2 if the table is full
     */
    int index(char *s, bool insert);
    /**
     * @brief The lex function
     *
     * @param fp The file to lex
     * @return UL The next token
     */
    UL lex(FILE *fp);
    /**
     * @brief The lex_all function
     *
     * @param fp The file to lex
     * @return std::vector<UL> The vector of tokens
     */
    std::vector<UL> lex_all(FILE *fp);
    /**
     * @brief The print_ul function
     *
     * @param ul The token to print
     */
    void print_ul(UL ul);
};

#endif
