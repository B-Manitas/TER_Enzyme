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
    float valeur;
    // union
    // {
    //     int i;
    //     float f;
    // } valeur;

    // Constructors
    /**
     * @brief Construct a new UL object
     * 
     * @param t The type of the token
     * @param v The value of the token
     */
    UL(State t, int v) : type(t), valeur{(float)v} {}
    /**
     * @brief Construct a new UL object
     * 
     * @param t The type of the token
     * @param v The value of the token
     */
    UL(State t, float v) : type(t), valeur{v} {}
    /**
     * @brief Construct a new UL object
     * 
     * @param t The type of the token
     * @param v The value of the token
     */
    UL(State t, double v) : type(t), valeur{(float)v} {}
};

#endif
