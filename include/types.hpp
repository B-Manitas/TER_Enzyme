#ifndef TYPES_HPP
#define TYPES_HPP

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

struct UL
{
    State type;
    union
    {
        int i;
        float f;
    } valeur;

    // Constructors
    UL(State t, int v) : type(t), valeur{.i = v} {}
    UL(State t, float v) : type(t), valeur{.f = v} {}
    UL(State t, double v) : type(t), valeur{.f = (float)v} {}
};

#endif
