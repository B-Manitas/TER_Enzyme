#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <tuple>
#include <cmath>
#include <map>
#include <algorithm>
#include <random>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"

class Simulation
{
private:
    // PRIVATE ATTRIBUTES
    std::vector<instr> m_instructions = std::vector<instr>{};
    std::vector<react> m_reactions = std::vector<react>{};
    std::map<int, std::tuple<int, int, int>> m_map_instructions = std::map<int, std::tuple<int, int, int>>{};

    std::vector<Coord> m_start_positions = std::vector<Coord>{};

    // PRIVATE METHODS
    /**
     * @brief Get the instructions from the tokenized data
     * map<ident, <count, diameter, speed>>
     *
     * @param data_tokenized The tokenized data
     * @return std::vector<instr> The instructions
     */
    std::map<int, std::tuple<int, int, int>> __map_instructions();
    /**
     * @brief Generate a random movement for a molecule
     *
     * @param position The current position of the molecule
     * @param speed The speed of the molecule
     * @return Coord The new position of the molecule
     */
    Coord __rand_movement(const Coord &position, float speed);
    /**
     * @brief Check if a molecule is hit by another molecule
     *
     * @param m The molecule to check
     * @return int The index of the molecule that hit the molecule. -1 if no molecule hit the molecule
     */
    int __is_hit(const Molecule &m);
    /**
     * @brief Check if two molecules are reacting
     *
     * @param molecule The first molecule
     * @param molecule_hit The second molecule
     * @param reaction The reaction that will be performed
     * @return true If the two molecules are reacting
     * @return false If the two molecules are not reacting
     */
    bool __is_reacting(Molecule &molecule, Molecule &molecule_hit, react &reaction);
    /**
     * @brief Perform a reaction of fusion between two molecules
     *
     * @param enzyme The enzyme molecule
     * @param substrate The substrate molecule
     * @param reaction The reaction to perform
     */
    void __reacting_fusion(Molecule &enzyme, Molecule &substrate, const react &reaction);
    /**
     * @brief Perform a reaction of unfusion, where the enzyme molecule is separated from the substrate molecule
     * type: Es -> e + s
     *
     * @param enzyme The enzyme molecule
     * @param ident_product The identifier of the product molecule
     */
    void __reacting_unfusion(Molecule &enzyme, const int &ident_product);

    /**
     * @brief Compute the distance between two coordinates
     *
     * @param a The first coordinate
     * @param b The second coordinate
     * @return float The distance between the two coordinates
     */
    float __distance(const Coord &a, const Coord &b);
    /**
     * @brief Compute the probability of a reaction 'e: s -> p'
     * [p1]: E + s -> Es
     *
     * @param reaction The reaction to compute the probability
     * @return float The probability of the reaction
     */
    float __compute_probability_1(const react &reaction, const float &probability_2, const float &probability_3);
    /**
     * @brief Compute the probability of a reaction 'e: s -> p'
     * [p2]: Es -> E + s
     *
     * @param reaction The reaction to compute the probability
     * @return float The probability of the reaction
     */
    float __compute_probability_2(const react &reaction, const float &probability_3);
    /**
     * @brief Compute the probability of a reaction 'e: s -> p'
     * [p3]: Es -> E + p
     *
     * @param reaction The reaction to compute the probability
     * @return float The probability of the reaction
     */
    float __compute_probability_3(const react &reaction);

public:
    // PUBLIC ATTRIBUTES
    std::vector<Molecule> m_molecules = std::vector<Molecule>{};
    std::map<int, std::string> m_names = std::map<int, std::string>{};
    std::vector<int> m_ident_molecules = std::vector<int>{};

    const float vesicle_diameter = 620;
    float max_diameter = 0;
    unsigned int m_time = 0;

    bool m_inverse_direction = false;

    // PUBLIC METHODS
    /**
     * @brief Initialize the simulation
     *
     * @param data_path The path to the data file
     */
    void init(char *data_path);
    /**
     * Initialize the maximum diameter of the molecules
     */
    void init_max_diameter();
    /**
     * Initialize the count of the molecules and the types of molecules
     */
    void init_count_molecules();
    /**
     * Initialize the equidistant positions of the molecules
     */
    void init_equidistant_positions();
    /**
     * Initialize the molecules
     */
    void init_molecules();
    /**
     * Initialize the reactions
     */
    void init_reactions();
    /**
     * @brief Move all the molecules in the simulation
     */
    void move_all_molecules();

    /**
     * @brief Read the file and parse it, to get the instructions and reactions of the simulation
     *
     * @param data_path The path to the data file
     */
    void read_file(char *data_path);

    // OPERATORS
    Simulation &operator=(const Simulation &other);
};

#endif // SIMULATION_HPP