#include "../include/simulation.hpp"

// PRIVATE METHODS

std::map<int, std::tuple<int, int, int>> Simulation::__map_instructions()
{
    // Ident: {Count, Diameter, Speed}
    std::map<int, std::tuple<int, int, int>> map;

    for (int i = 0; i < m_instructions.size(); i++)
    {
        switch (m_instructions[i].type)
        {
        case Keyword::INIT:
            if (map.find(m_instructions[i].ident) != map.end())
                std::get<0>(map[m_instructions[i].ident]) += m_instructions[i].value;
            else
                map[m_instructions[i].ident] = {m_instructions[i].value, 0, 0};

            continue;

        case Keyword::DIAMETER:
            if (map.find(m_instructions[i].ident) != map.end())
                std::get<1>(map[m_instructions[i].ident]) = m_instructions[i].value;
            else
                map[m_instructions[i].ident] = {0, m_instructions[i].value, 0};

            continue;

        case Keyword::SPEED:
            if (map.find(m_instructions[i].ident) != map.end())
                std::get<2>(map[m_instructions[i].ident]) = m_instructions[i].value;
            else
                map[m_instructions[i].ident] = {0, 0, m_instructions[i].value};

            continue;
        }
    }

    return map;
}

// ========================
// INITIALIZATION METHODS
void Simulation::init(char *data_path)
{
    // Read the file and parse it, to get the instructions and reactions of the simulation
    read_file(data_path);

    // Initialize the attributes of the simulation
    init_max_diameter();
    init_count_molecules();
    init_equidistant_positions();
    init_molecules();
}

void Simulation::init_max_diameter()
{
    for (auto &&i : m_instructions)
        if (i.type == Keyword::DIAMETER)
            max_diameter = std::max(max_diameter, i.value);
}

void Simulation::init_count_molecules()
{
    for (auto &&i : m_instructions)
        if (i.type == Keyword::INIT)
        {
            n_types_molecule++;
            n_molecules += i.value;
            m_ident_molecules.push_back(i.ident);
        }
}

void Simulation::init_equidistant_positions()
{
    float offset = 10;
    float vesicle_radius = vesicle_diameter / 2 - max_diameter / 2 - offset;
    float bounding_volume = (4.0 / 3.0) * M_PI * vesicle_radius * vesicle_radius * vesicle_radius;

    // Calculer le volume total occupé par toutes les sphères intérieures
    float molecule_volume = (4.0 / 3.0) * M_PI * max_diameter * max_diameter * max_diameter / 8;
    int n_spheres = static_cast<int>(bounding_volume / molecule_volume);

    // Utiliser une distribution cubique régulière à l'intérieur de la sphère englobante
    float sphere_distance = max_diameter; // Distance entre les centres de deux sphères adjacentes
    float cube_root_n = std::cbrt(n_spheres);
    for (int x = 0; x < cube_root_n; ++x)
    {
        for (int y = 0; y < cube_root_n; ++y)
        {
            for (int z = 0; z < cube_root_n; ++z)
            {
                float sphere_x = (2 * x + 1 - cube_root_n) * sphere_distance / 2;
                float sphere_y = (2 * y + 1 - cube_root_n) * sphere_distance / 2;
                float sphere_z = (2 * z + 1 - cube_root_n) * sphere_distance / 2;

                // Vérifier que chaque sphère reste à l'intérieur de la sphère englobante
                float distance_from_center = std::sqrt(sphere_x * sphere_x + sphere_y * sphere_y + sphere_z * sphere_z);
                if (distance_from_center <= vesicle_radius)
                    m_start_positions.push_back({sphere_x, sphere_y, sphere_z});
            }
        }
    }

    std::shuffle(m_start_positions.begin(), m_start_positions.end(), std::default_random_engine());
}

void Simulation::init_molecules()
{
    std::map<int, std::tuple<int, int, int>> map = __map_instructions();
    int idx = 0;

    for (auto &&i : map)
    {
        for (int j = 0; j < std::get<0>(i.second); j++)
        {

            Molecule molecule = Molecule(i.first);
            molecule.diameter = std::get<1>(i.second);
            molecule.speed = std::get<2>(i.second);

            std::tie(molecule.x, molecule.y, molecule.z) = m_start_positions[idx];

            m_molecules.push_back(molecule);
            idx++;
        }
    }
}

// ========================
// OTHER METHODS

void Simulation::read_file(char *data_path)
{
    FILE *fp = fopen(data_path, "r");

    if (fp == NULL)
        throw std::runtime_error("The file could not be opened");

    Lexer lexer = Lexer();
    Parser parser = Parser();

    parser.parse(lexer.lex_all(fp), m_reactions, m_instructions);
    fclose(fp);
}
