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
 *
 * @param ident The id of the enzyme
 * @param substrates The substrates represented by their id. (0 if not present)
 * @param products The products represented by their id. (0 if not present)
 * @param mM The quantity in mM of sub_1 and sub_2.
 * @param kcat The kcat of the enzyme
 */
struct react
{
    // The id of the enzyme
    float ident = 0;

    // The substrates represented by their id. (0 if not present)
    std::tuple<float, float> substrates = {0, 0};

    // The products represented by their id. (0 if not present)
    std::tuple<float, float> products = {0, 0};

    // The quantity in mM of sub_1 and sub_2.
    std::tuple<float, float> mM = {0, 0};

    float kcat = 0;
};

/**
 * @brief The instr struct represents an instruction.
 *
 * @param type The type of the instruction
 * @param ident The id of the molecule
 */
struct instr
{
    Keyword type;

    float ident = 0, value = 0;
};

// Classe des coordonnées d'une molécule
struct Coord
{
    // The x, y, z position of the molecule
    float x = 0, y = 0, z = 0;

    // Constructors
    Coord() = default;
    /**
     * @brief Construct a new Coord object
     *
     * @param x The x position of the molecule
     * @param y The y position of the molecule
     * @param z The z position of the molecule
     */
     Coord(float x, float y, float z) : x(x), y(y), z(z) {}

    // Operators ==
    bool operator==(const Coord &c) const
    {
        return x == c.x && y == c.y && z == c.z;
    }

    // Operators !=
    bool operator!=(const Coord &c) const
    {
        return x != c.x || y != c.y || z != c.z;
    }
};

/**
 * @brief The Molecule struct represents a molecule.
 *
 * @param ident The type identifier of the molecule
 * @param diameter The diameter of the molecule
 * @param speed The speed of the molecule
 * @param x The x position of the molecule
 * @param y The y position of the molecule
 * @param z The z position of the molecule
 * @param is_seen Boolean to check if the molecule has been seen
 * @param next Pointer to the next molecule in the same zone
 * @param prev Pointer to the prev molecule in the same zone
 */
struct Molecule
{
    // The type identifier of the molecule
    int ident = 0;

    // The diameter and speed of the molecule
    float diameter = 1, speed = 1;

    // Position of the molecule with class Coord
    Coord position;

    // Boolean to check if the molecule has been seen
    bool is_seen = false;

    // Pointer to the next and prev molecule in the same zone
    Molecule *next = 0, *prev = 0;

    // Constructors
    Molecule() = default;
    /**
     * @brief Construct a new Molecule object
     *
     * @param ident The type identifier of the molecule
     */
    Molecule(int ident) : ident(ident) {}
};

#endif
