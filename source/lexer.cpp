#include "../include/lexer.hpp"
#include <exception>

// CONSTRUCTOR
Lexer::Lexer() {}
Lexer::~Lexer() {}

// METHODS
int Lexer::hash(char *s)
{
    int h = 11;
    while (*s != 0)
        h = ((h * 19) ^ int(*s++)) % m_HASH_SIZE;

    if (h < 0)
        h = h + m_HASH_SIZE;

    return h;
}

int Lexer::index(char *s, bool insert)
{
    int h = hash(s);

    for (int n = 0; n < m_HASH_SIZE; n++)
    {
        char *t = m_table[h];

        if (t == 0)
        {
            if (!insert)
                return -1;

            m_table[h] = strdup(s);
            return h;
        }

        if (strcmp(s, t) == 0)
            return h;

        h = (h + 41) % m_HASH_SIZE;
    }

    return -2;
}

UL Lexer::lex(FILE *fp)
{
    int charac = 0;
    int state = STD;
    int i = 0;
    char buffer[1024] = {0};

    for (;;)
    {
        charac = getc(fp);

        // Stop the program if the file is empty
        if (charac == EOF)
            return UL{END_OF_FILE, 0};

        if (charac == '\n')
            return UL{END, 0};

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
            if (charac == '.' or charac == 'e' or (charac >= '0' and charac <= '9'))
            {
                state = NUM;
                buffer[i++] = charac;
                continue;
            }

            state = STD;
            ungetc(charac, fp);
            buffer[i] = 0;

            return UL{NUM, std::atof(buffer)};

        case IDENT:
            if (charac == '"')
            {
                int id = index(buffer, false);

                if (id == -1)
                    return UL(IDENT, index(buffer, true));

                else
                    return UL{IDENT, id};
            }

            else if (charac == EOF)
                return UL{ERROR, 0};

            else
            {
                buffer[i++] = charac;
                continue;
            }

        case IDENT_P:
            if (buffer[i - 1] == 'u' and charac == 'M')
                return UL{UNIT, Unit::uM};

            if (buffer[i - 1] == 'm' and charac == 'M')
                return UL{UNIT, Unit::mM};

            if (charac == ' ')
            {
                ungetc(charac, fp);

                int id = index(buffer, false);

                if (id == -1)
                    return UL{IDENT, index(buffer, true)};

                return UL{IDENT, id};
            }

            if ((charac >= 'a' and charac <= 'z') or (charac >= 'A' and charac <= 'Z') or (charac >= '0' and charac <= '9'))
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

        case PROBABLY_ARROW:
            if (charac == '>')
                return UL{PONCT, Ponct::ARROW};

            else
                return UL{PONCT, Ponct::MINUS};

        case PROBABLY_COMMENT:
            if (buffer[i - 1] == '/' and charac == '/')
            {
                state = COMMENT;
                continue;
            }

            else
                return UL{ERROR, 0};

        case COMMENT:
            if (charac == '\n')
                state = STD;

            else
                continue;

        default:
            break;
        }
    }
}

void Lexer::print_ul(UL ul)
{
    switch (ul.type)
    {
    case END:
        printf("END\n");
        break;
    case IDENT:
        printf("IDENT(%d) ", ul.valeur.i);
        break;

    case PONCT:
        switch (ul.valeur.i)
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

        default:
            break;
        }
        break;

    case NUM:
        printf("NUM(%f) ", ul.valeur.f);
        break;

    case ERROR:
        printf("ERROR ");
        break;

    case UNIT:
        switch (ul.valeur.i)
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

    default:
        break;
    }
}
