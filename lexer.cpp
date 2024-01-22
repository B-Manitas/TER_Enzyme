#include <stdio.h>
#include <cstdlib>
#include <string.h>

// Define the hash table
const int HASH_SIZE = 150000;
static char *table[HASH_SIZE];

// Define the state of the automata
enum state
{
    STD,
    IDENT,
    IDENT_P,
    PONCT,
    NUM,
    ERROR,
    UNIT,
    END,
    PF
};

struct UL
{
    state type;
    union
    {
        int i;
        float f;
    } valeur;

    // Constructors
    UL(state t, int v) : type(t), valeur{.i = v} {}
    UL(state t, float v) : type(t), valeur{.f = v} {}
    UL(state t, double v) : type(t), valeur{.f = (float)v} {}
};

int hash(char *s)
{
    int h = 11;
    while (*s != 0)
        h = ((h * 19) ^ int(*s++)) % HASH_SIZE;

    if (h < 0)
        h = h + HASH_SIZE;

    return h;
}

int index(char *s, bool enter)
{
    int h = hash(s);

    for (int n = 0; n < HASH_SIZE; n++)
    {
        char *t = table[h];

        if (t == 0)
        {
            if (!enter)
                return -1;

            table[h] = strdup(s);
            return h;
        }

        if (strcmp(s, t) == 0)
            return h;

        h = (h + 41) % HASH_SIZE;
    }

    return -2;
}

UL parser(FILE *fp)
{
    int charac;
    int state = STD;
    int i = 0;
    char buffer[1024];

    for (;;)
    {
        charac = getc(fp);

        // Stop the program if the file is empty
        if (charac == EOF)
        {
            return UL{END, 0};
        }

        switch (state)
        {
        case STD:
            // Ignore the spaces and the new lines
            if (charac == ' ' or charac == '\n')
                continue;

            // Extract ponctuation
            if (charac == ';' or charac == ':' or charac == '+' or charac == '|')
                return UL{PONCT, charac};

            // Extract the - and the ->
            if (charac == '-')
            {
                state = PF;
                buffer[i++] = charac;
                continue;
            }

            // Extract the identifiers starting with a '"'
            if (charac == '"')
            {
                state = IDENT;
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
                state = STD;
                UL ul = UL{IDENT, index(buffer, true)};
                buffer[i] = 0;
                return ul;
            }

            buffer[i++] = charac;
            continue;

        case IDENT_P:
            if ((charac >= 'a' and charac <= 'z') or (charac >= 'A' and charac <= 'Z') or (charac >= '0' and charac <= '9'))
            {
                state = IDENT_P;
                buffer[i++] = charac;
                continue;
            }

            state = STD;
            ungetc(charac, fp);
            buffer[i] = 0;

            return UL{IDENT, index(buffer, true)};

        case PF:
            if (charac == '>')
                return UL{PONCT, '->'};

            state = STD;
            ungetc(charac, fp);
            buffer[i] = 0;

        default:
            break;
        }
    }
}

int main()
{
    // Open the file
    FILE *fp;
    fp = fopen("data", "r");

    // Read the file
    parser(fp);

    // Close the file
    fclose(fp);

    return 0;
}