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
    void init(char *data_path);
    void init_max_diameter();
    void init_count_molecules();
    void init_equidistant_positions();
    void init_molecules();

    void read_file(char *data_path);

    Simulation &operator=(const Simulation &other)
    {
        if (this != &other)
        {
            // Copier les membres de other vers this
            m_instructions = other.m_instructions;
            m_reactions = other.m_reactions;
            m_start_positions = other.m_start_positions;
            m_molecules = other.m_molecules;
            m_ident_molecules = other.m_ident_molecules;
            max_diameter = other.max_diameter;
            n_types_molecule = other.n_types_molecule;
            n_molecules = other.n_molecules;
        }
        return *this;
    }
};

#endif // SIMULATION_HPP