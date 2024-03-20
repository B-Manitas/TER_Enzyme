#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <tuple>
#include <cmath>
#include <map>
#include <algorithm>
#include <random>

#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"

class Simulation
{
private:
    // PRIVATE ATTRIBUTES
    std::vector<instr> m_instructions = std::vector<instr>{};
    std::vector<react> m_reactions = std::vector<react>{};

    std::vector<std::tuple<float, float, float>> m_start_positions = std::vector<std::tuple<float, float, float>>{};

    // PRIVATE METHODS
    /**
     * @brief Get the instructions from the tokenized data
     * map<ident, <count, diameter, speed>>
     *
     * @param data_tokenized The tokenized data
     * @return std::vector<instr> The instructions
     */
    std::map<int, std::tuple<int, int, int>> __map_instructions();

public:
    // PUBLIC ATTRIBUTES
    std::vector<Molecule> m_molecules = std::vector<Molecule>{};
    std::vector<int> m_ident_molecules = std::vector<int>{};

    const float vesicle_diameter = 620;
    float max_diameter = 0;
    float n_types_molecule = 0;
    float n_molecules = 0;

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
     * @brief Read the file and parse it, to get the instructions and reactions of the simulation
     *
     * @param data_path The path to the data file
     */
    void read_file(char *data_path);

    // OPERATORS
    Simulation &operator=(const Simulation &other);
};

#endif // SIMULATION_HPP