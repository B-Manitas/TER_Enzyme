#ifndef ENUM_HPP
#define ENUM_HPP

/**
 * @brief The State enum represents the different types of tokens.
 */
enum State
{
    COMMENT,
    END,
    END_OF_FILE,
    ERROR,
    IDENT,
    IDENT_P,
    NUM,
    PONCT,
    PROBABLY_ARROW,
    PROBABLY_COMMENT,
    STD,
    UNIT,
    KEYWORD
};

/**
 * @brief The Unit enum represents the different units.
 */
enum Unit
{
    uM,
    mM
};

/**
 * @brief The Keyword enum represents the different keywords used in the language.
 */
enum Keyword
{
    INIT,
    DIAMETER,
    SPEED
};

/**
 * @brief The Ponct enum corresponds to the different punctuation marks.
 */
enum Ponct
{
    ARROW,
    COLON,
    COMMA,
    MINUS,
    PLUS,
    SEMICOLON,
    VBAR,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    EQUAL
};

#endif // ENUM_HPP