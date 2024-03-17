#include "../include/lexer.hpp"
#include <exception>

// CONSTRUCTOR
Lexer::Lexer() {}
Lexer::~Lexer() {}

// METHODS
int Lexer::hash(char *s)
{
    // Initialize the hash
    int h = 11;

    // While the string is not empty hash the string
    while (*s != 0)
        h = ((h * 19) ^ int(*s++)) % m_HASH_SIZE;

    // If the hash is negative, make it positive
    if (h < 0)
        h = h + m_HASH_SIZE;

    return h;
}

int Lexer::index(char *s, bool insert)
{
    // Get the hash of the string
    int h = hash(s);

    // Search for the string in the table
    for (int n = 0; n < m_HASH_SIZE; n++)
    {
        // Get the string at the index 'h' of the table
        char *t = m_table[h];

        // If 't' is zero, the string is not in the table
        if (t == 0)
        {
            if (!insert)
                return -1;

            // Insert the string in the table, and return the index
            m_table[h] = strdup(s);
            return h;
        }

        // If the string is in the table, return the index
        if (strcmp(s, t) == 0)
            return h;

        // Otherwise, go to the next index to search for the string
        h = (h + 41) % m_HASH_SIZE;
    }

    return -2;
}

UL Lexer::lex(FILE *fp)
{
    // Define variables
    int charac = 0;
    int state = STD;
    int i = 0;
    char buffer[1024] = {0};

    // Loop through the file
    for (;;)
    {
        charac = getc(fp);

        // The end of the file
        if (charac == EOF)
            return UL{END_OF_FILE, 0};

        // The end of the line
        if (charac == '\n')
            return UL{END, 0};

        // Automata to extract the tokens
        switch (state)
        {
        case STD:
            // Ignore the spaces and the new lines
            if (charac == ' ' or charac == '\t')
                continue;

            // Extract ponctuation
            if (charac == ';')
                return UL{PONCT, Ponct::SEMICOLON};

            if (charac == ':')
                return UL{PONCT, Ponct::COLON};

            if (charac == '+')
                return UL{PONCT, Ponct::PLUS};

            if (charac == '|')
                return UL{PONCT, Ponct::VBAR};

            if (charac == ',')
                return UL{PONCT, Ponct::COMMA};

            if (charac == '(')
                return UL{PONCT, Ponct::PARENTHESIS_OPEN};

            if (charac == ')')
                return UL{PONCT, Ponct::PARENTHESIS_CLOSE};

            if (charac == '=')
                return UL{PONCT, Ponct::EQUAL};

            // Extract the - and the ->
            if (charac == '-')
            {
                state = PROBABLY_ARROW;
                buffer[i++] = charac;
                continue;
            }

            // Extract the identifiers starting with a '"'
            if (charac == '"')
            {
                state = IDENT;
                continue;
            }

            // Extract the identifiers starting with a letter
            if (charac == 'u')
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

            if (charac == 'm')
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

            // Extract the identifiers starting with a letter
            if ((charac >= 'a' and charac <= 'z') or (charac >= 'A' and charac <= 'Z') or charac == '_')
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

            // Extract the numbers
            if (charac == '.' or (charac >= '0' and charac <= '9'))
            {
                state = NUM;
                buffer[i++] = charac;
                continue;
            }

            // Extract the comments
            if (charac == '/')
            {
                state = PROBABLY_COMMENT;
                buffer[i++] = charac;
                continue;
            }

            // Return error if the character is not recognized
            return UL{ERROR, 0};

        case NUM:
            // Extract the numbers
            if (charac == '.' or charac == 'e' or (charac >= '0' and charac <= '9'))
            {
                state = NUM;
                buffer[i++] = charac;
                continue;
            }

            // If the number is finished, return the token
            state = STD;
            ungetc(charac, fp);
            buffer[i] = 0;

            return UL{NUM, std::atof(buffer)};

        case IDENT:
            // Extract the identifiers starting with a '"'
            if (charac == '"')
            {
                int id = index(buffer, false);

                // If the identifier is not in the table, insert it
                if (id == -1)
                    return UL(IDENT, index(buffer, true));

                // Otherwise, return the index of the identifier
                else
                    return UL{IDENT, id};
            }

            // Return error if the character is not finished
            else if (charac == EOF)
                return UL{ERROR, 0};

            // Extract the identifiers starting with a letter
            else
            {
                buffer[i++] = charac;
                continue;
            }

        case IDENT_P:
            // Extract the identifiers starting with a letter
            // Search for the unit uM
            if (buffer[i - 1] == 'u' and charac == 'M')
                return UL{UNIT, Unit::uM};

            // Search for the unit mM
            if (buffer[i - 1] == 'm' and charac == 'M')
                return UL{UNIT, Unit::mM};

            // Search for the keywords init
            if (!strcmp(buffer, "ini") and charac == 't')
                return UL{KEYWORD, Keyword::INIT};

            // Search for the keywords diametre
            if (!strcmp(buffer, "diametr") and charac == 'e')
                return UL{KEYWORD, Keyword::DIAMETER};

            // Search for the keywords vitesse
            if (!strcmp(buffer, "vitess") and charac == 'e')
                return UL{KEYWORD, Keyword::SPEED};

            // Extract the identifiers starting with a letter
            if ((charac >= 'a' and charac <= 'z') or (charac >= 'A' and charac <= 'Z') or (charac >= '0' and charac <= '9'))
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

            else
            // If the identifier is finished, return the token
            {
                ungetc(charac, fp);

                int id = index(buffer, false);

                if (id == -1)
                    return UL{IDENT, index(buffer, true)};

                return UL{IDENT, id};
            }

        case PROBABLY_ARROW:
            // Extract the ->
            if (charac == '>')
                return UL{PONCT, Ponct::ARROW};

            // Extract the -
            else
                return UL{PONCT, Ponct::MINUS};

        case PROBABLY_COMMENT:
            // Extract the comments starting with //
            if (buffer[i - 1] == '/' and charac == '/')
            {
                state = COMMENT;
                continue;
            }

            else
                return UL{ERROR, 0};

        case COMMENT:
            // Ignore the comments
            if (charac == '\n')
                state = STD;

            else
                continue;

        default:
            break;
        }
    }
}

std::vector<UL> Lexer::lex_all(FILE *fp)
{
    // Define the vector of tokens
    std::vector<UL> tokens;

    // Loop through the file
    for (;;)
    {
        // Add the token to the vector
        UL token = lex(fp);
        tokens.push_back(token);

        if (token.type == END_OF_FILE)
            break;
    }

    return tokens;
}

void Lexer::print_ul(UL ul)
{
    switch (ul.type)
    {
    case END:
        printf("END\n");
        break;

    case IDENT:
        printf("IDENT(%d) ", int(ul.valeur));
        break;

    case PONCT:
        switch (int(ul.valeur))
        {
        case Ponct::ARROW:
            printf("-> ");
            break;

        case Ponct::SEMICOLON:
            printf("; ");
            break;

        case Ponct::COLON:
            printf(": ");
            break;

        case Ponct::PLUS:
            printf("+ ");
            break;

        case Ponct::MINUS:
            printf("- ");
            break;

        case Ponct::VBAR:
            printf("| ");
            break;

        case Ponct::COMMA:
            printf(", ");
            break;

        case Ponct::PARENTHESIS_OPEN:
            printf("( ");
            break;

        case Ponct::PARENTHESIS_CLOSE:
            printf(") ");
            break;

        case Ponct::EQUAL:
            printf("= ");
            break;

        default:
            break;
        }
        break;

    case NUM:
        printf("NUM(%f) ", ul.valeur);
        break;

    case ERROR:
        printf("ERROR ");
        break;

    case UNIT:
        switch (int(ul.valeur))
        {
        case Unit::uM:
            printf("uM ");
            break;

        case Unit::mM:
            printf("mM ");
            break;

        default:
            break;
        }
        break;

    case KEYWORD:
        switch (int(ul.valeur))
        {
        case Keyword::INIT:
            printf("init ");
            break;

        case Keyword::DIAMETER:
            printf("diameter ");
            break;

        case Keyword::SPEED:
            printf("speed ");
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}
