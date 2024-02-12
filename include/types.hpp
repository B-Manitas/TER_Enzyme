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
    UNIT
};

enum Unit
{
    uM,
    mM
};

enum Ponct
{
    ARROW,
    COLON,
    COMMA,
    MINUS,
    PLUS,
    SEMICOLON,
    VBAR
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
