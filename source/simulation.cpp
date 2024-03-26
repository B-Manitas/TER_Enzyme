#include "../include/simulation.hpp"
#include <stdexcept>

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

Coord Simulation::__rand_movement(const Coord &position, float speed)
{
    // Generate a random angle in radians
    float angle = (rand() % 360) * M_PI / 180;

    float x_new = position.x + speed * cos(angle);
    float y_new = position.y + speed * sin(angle);
    float z_new = position.z + speed * (rand() % 2 == 0 ? 1 : -1);

    return {x_new, y_new, z_new};
}

int Simulation::__is_hit(const Molecule &m)
{
    for (size_t i = 0; i < m_molecules.size(); i++)
    {
        // If the molecule has already been seen, skip it
        // If the molecule is the same as the current molecule, skip it
        if (m_molecules[i].is_seen)
            continue;

        if (__distance(m.position, m_molecules[i].position) < (m.diameter + m_molecules[i].diameter) / 2)
            return i;
    }

    return -1;
}

bool Simulation::__is_reacting(Molecule &molecule, Molecule &molecule_hit, react &reaction)
{
    for (const react &r : m_reactions)
    {
        // Check if molecule is an enzyme and the hit is a substrate
        const bool molecule_is_reacting = r.ident == molecule.ident && r.substrate == molecule_hit.ident;
        
        // Check if hit is an enzyme and the molecule is a substrate
        const bool hit_is_reacting = r.ident == molecule_hit.ident && r.substrate == molecule.ident;

        if (molecule_is_reacting || hit_is_reacting)
        {
            reaction = r;
            return true;
        }
    }

    return false;
}

float Simulation::__distance(const Coord &a, const Coord &b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
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
    std::set<float> set_ident;

    for (auto &&r : m_reactions)
        set_ident.insert({r.ident, r.substrate, r.product});

    m_ident_molecules = std::vector<int>(set_ident.begin(), set_ident.end());
}

void Simulation::init_equidistant_positions()
{
    float offset = 10;
    float vesicle_radius = vesicle_diameter / 2 - max_diameter / 2 - offset;
    float bounding_volume = (4.0 / 3.0) * M_PI * vesicle_radius * vesicle_radius * vesicle_radius;

    // Compute the volume of a molecule (sphere)
    float molecule_volume = (4.0 / 3.0) * M_PI * max_diameter * max_diameter * max_diameter / 8;
    int n_spheres = static_cast<int>(bounding_volume / molecule_volume);

    // Use a cubic grid to place the spheres
    // The distance between the centers of two adjacent spheres is equal to the diameter of the sphere
    float sphere_distance = max_diameter;
    float cube_root_n = std::cbrt(n_spheres);
    for (int x = 0; x < cube_root_n; ++x)
    {
        for (int y = 0; y < cube_root_n; ++y)
        {
            for (int z = 0; z < cube_root_n; ++z)
            {
                // Compute the position of the sphere
                const Coord &sphere = {
                    (2 * x + 1 - cube_root_n) * sphere_distance / 2,
                    (2 * y + 1 - cube_root_n) * sphere_distance / 2,
                    (2 * z + 1 - cube_root_n) * sphere_distance / 2};

                // Check if the sphere is inside the vesicle
                if (__distance(sphere, Coord()) <= vesicle_radius)
                    m_start_positions.push_back(sphere);
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
            molecule.name = m_ident_molecules[i.first];
            molecule.diameter = std::get<1>(i.second);
            molecule.speed = std::get<2>(i.second);

            molecule.position.x = m_start_positions[idx].x;
            molecule.position.y = m_start_positions[idx].y;
            molecule.position.z = m_start_positions[idx].z;

            m_molecules.push_back(molecule);
            idx++;
        }
    }
}

void Simulation::move_all_molecules()
{
    for (size_t i = 0; i < m_molecules.size(); i++)
    {
        size_t reverse_i = m_molecules.size() - i - 1;
        Molecule &m = m_molecules[m_inverse_direction ? reverse_i : i];

        // If the molecule has already been seen, skip it
        if (m.is_seen)
            continue;

        // Mark the molecule as seen
        m.is_seen = true;

        // Generate a new position for the molecule
        Coord new_pos = __rand_movement(m.position, m.speed);

        // If the molecule is outside the vesicle, skip it
        if (__distance(new_pos, Coord()) > vesicle_diameter / 2 - m.diameter / 2)
            continue;

        // Check if the molecule has collided with another molecule
        int id_hit = __is_hit(m);

        // If the molecule has not collided with another molecule, update its position
        if (id_hit == -1)
            m.position = new_pos;

        // Else, check if a reaction can occur
        else
        {
            react reaction;
            Molecule &molecule_hit = m_molecules[id_hit];

            if (__is_reacting(m, molecule_hit, reaction))
            {
                // Update the position of the molecule
                m.position = new_pos;

                // Update the type of the molecule
                if (reaction.ident == m.ident)
                {
                    if (reaction.substrate == molecule_hit.ident)
                        m.ident = reaction.product;

                    else
                        m.ident = reaction.product;
                }

                else
                {
                    if (reaction.substrate == m.ident)
                        molecule_hit.ident = reaction.product;

                    else
                        molecule_hit.ident = reaction.product;
                }

                molecule_hit.is_seen = true;
                m_molecules[id_hit] = molecule_hit;
            }
        }
    }

    // Reset the is_seen attribute of all molecules
    for (Molecule &m : m_molecules)
        m.is_seen = false;

    m_inverse_direction = !m_inverse_direction;
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

    for (int i=0; i < lexer.m_HASH_SIZE; i++)
    {
        if (lexer.m_table[i] != NULL)
        {
            // std::string molecule_name = ;
            m_names[i] = std::string(lexer.m_table[i]);
        }
    }

    fclose(fp);
}

// ========================
// OPERATORS
Simulation &Simulation::operator=(const Simulation &other)
{
    if (this != &other)
    {
        m_instructions = other.m_instructions;
        m_reactions = other.m_reactions;
        m_start_positions = other.m_start_positions;
        m_molecules = other.m_molecules;
        m_ident_molecules = other.m_ident_molecules;
        max_diameter = other.max_diameter;
        m_inverse_direction = other.m_inverse_direction;
        m_names = other.m_names;
    }
    return *this;
}