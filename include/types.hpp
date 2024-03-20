#include <map>
#include "enums.hpp"

#ifndef TYPES_HPP
#define TYPES_HPP

/**
 * @brief The UL struct represents a token.
 *
 * @param type The type of the token
 * @param valeur The value of the token
 */
struct UL
{
    State type;
    float valeur;

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

/**
 * @brief The react struct represents a reaction.
 */
struct react
{
    // The id of the enzyme
    float ident = 0;

    // The substrates represented by their id. (0 if not present)
    // map<substrate_id, quantity> substrates
    std::map<float, float> substrates = {{0, 0}, {1, 0}};

    // The products represented by their id. (0 if not present)
    // map<product_id, quantity> products
    std::map<float, float> products = {{0, 0}, {1, 0}};

    // The quantity in mM of sub_1 and sub_2.
    // map<substrate_id, mM> mM
    std::map<float, float> mM = {{0, 0}, {1, 0}};

    float kcat = 0;
};

/**
 * @brief The instr struct represents an instruction.
 */
struct instr
{
    float ident = 0;
    Keyword type;
    float value = 0;
};

struct Molecule
{
    int ident = 0;
    float diameter = 1;
    float speed = 1;
    float x = 0, y = 0, z = 0;

    // Permet de gerer la position de départ
    // bool is_finished_move = false;
    // Permet de savoir si une molécule a été vue par un enzyme
    bool is_seen = false;

    Molecule *next; // Pointeur vers la prochaine molécule d'une même zone
    Molecule *prev; // Pointeur vers la molécule précédente d'une même zone

    Molecule(int ident) : ident(ident) {}
};

#endif
