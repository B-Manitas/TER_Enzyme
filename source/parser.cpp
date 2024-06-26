#include <stdexcept>
#include <tuple>
#include <string>

#include "../include/parser.hpp"
#include "../include/enums.hpp"
#include "../include/types.hpp"

// 'PARSE' METHODS
void Parser::parse(std::vector<UL> data_tokenized, std::vector<react> &reactions, std::vector<instr> &instructions)
{
    while (data_tokenized.size() > 0)
    {
        switch (data_tokenized.at(0).type)
        {
        case KEYWORD:
            instructions.push_back(instruction(data_tokenized));
            continue;

        case IDENT:
            reactions.push_back(reaction(data_tokenized));
            continue;

        case END_OF_FILE:
            return;

        default:
            data_tokenized.erase(data_tokenized.begin());
            continue;
        }
    }
}

react Parser::reaction(std::vector<UL> &data_tokenized)
{
    react r;

    // Get enzyma
    r.ident = next_token(data_tokenized, IDENT, "enzyma error");

    // Next symbol is colon
    next_symbol_except(data_tokenized, COLON, "syntax_error 0");

    // Get substrate
    r.substrate = next_token(data_tokenized, IDENT, "substrate error");

    // Next symbol is arrow
    next_symbol_except(data_tokenized, ARROW, "syntax_error 1");

    // Get product
    r.product = next_token(data_tokenized, IDENT, "product error");

    // Next symbol is arrow
    next_symbol_except(data_tokenized, VBAR, "syntax_error 2");

    // Get mM
    r.mM = std::get<0>(mM_series(data_tokenized));

    // Next symbol is minus
    next_symbol_except(data_tokenized, MINUS, "syntax_error 3");

    // Get kcat
    r.kcat = next_token(data_tokenized, NUM, "kcat error");

    next_symbol_except(data_tokenized, SEMICOLON, "syntax_error 4");

    if (data_tokenized.at(0).type == END)
        data_tokenized.erase(data_tokenized.begin());

    return r;
}

instr Parser::instruction(std::vector<UL> &data_tokenized)
{
    instr i;

    // Get the type of instruction
    i.type = Keyword(next_token(data_tokenized, KEYWORD, "keyword error"));

    // Next symbol is parenthesis open
    next_symbol_except(data_tokenized, PARENTHESIS_OPEN, "syntax_error");

    // Get the ident
    i.ident = next_token(data_tokenized, IDENT, "ident error");

    // Next symbol is parenthesis close
    next_symbol_except(data_tokenized, PARENTHESIS_CLOSE, "syntax_error");

    // Next symbol is equal
    next_symbol_except(data_tokenized, EQUAL, "syntax_error");

    // Get the value
    i.value = next_token(data_tokenized, NUM, "value error");

    // If the instruction is diameter, multiply the value by 10
    if (i.type == Keyword::DIAMETER)
        i.value *= 10;

    // If the instruction is speed, multiply the value by 5
    if (i.type == Keyword::SPEED)
        i.value *= 5;

    // Next symbol is semicolon
    next_symbol_except(data_tokenized, SEMICOLON, "syntax_error");

    // Erase the end token if it exists
    if (data_tokenized.at(0).type == END)
        data_tokenized.erase(data_tokenized.begin());

    return i;
}

// 'SERIES' METHODS
std::vector<react> Parser::reactions_series(std::vector<UL> data_tokenized)
{
    std::vector<react> reactions;

    // While there are still tokens, parse the reactions
    while (data_tokenized.size() > 0 and data_tokenized.at(0).type != END_OF_FILE)
    {
        try
        {
            reactions.push_back(reaction(data_tokenized));
        }
        catch (const std::exception &e)
        {
            printf("Error: %s\n", e.what());
            data_tokenized.erase(data_tokenized.begin());
            continue;
        }
    }

    return reactions;
}

std::vector<instr> Parser::instructions_series(std::vector<UL> data_tokenized)
{
    std::vector<instr> instructions;

    // While there are still tokens, parse the instructions
    while (data_tokenized.size() > 0)
    {
        try
        {
            instructions.push_back(instruction(data_tokenized));
        }
        catch (const std::exception &e)
        {
            data_tokenized.erase(data_tokenized.begin());
            continue;
        }
    }

    return instructions;
}

std::tuple<float, float> Parser::idents_series(std::vector<UL> &data_tokenized)
{
    std::tuple<float, float> ident = {0, 0};

    // Extract first ident
    std::get<0>(ident) = next_token(data_tokenized, IDENT, "ident");

    // If there is a plus, extract the second ident
    if (next_symbol(data_tokenized, Ponct::PLUS))
        std::get<1>(ident) = next_token(data_tokenized, IDENT, "ident");

    return ident;
}

std::tuple<float, float> Parser::mM_series(std::vector<UL> &data_tokenized)
{
    std::tuple<float, float> mM = {0, 0};
    std::tuple<float, float> unit = {Unit::mM, Unit::mM};

    std::get<0>(mM) = next_token(data_tokenized, NUM, "mM number");
    std::get<0>(unit) = next_token(data_tokenized, UNIT, "mM unit");

    if (std::get<0>(unit) == Unit::uM)
        std::get<0>(mM) *= 1e-3;

    if (next_symbol(data_tokenized, COMMA))
    {
        std::get<1>(mM) = next_token(data_tokenized, NUM, "mM number");
        std::get<1>(unit) = next_token(data_tokenized, UNIT, "mM unit");

        if (std::get<1>(unit) == Unit::uM)
            std::get<1>(mM) *= 1e-3;
    }

    return mM;
}

// ========================
// 'NEXT' METHODS

float Parser::next_token(std::vector<UL> &data_tokenized, State type, std::string exception)
{
    // Check if the token is of the right type
    if (data_tokenized.at(0).type == type)
    {
        float tmp = data_tokenized.at(0).valeur;
        data_tokenized.erase(data_tokenized.begin());
        return tmp;
    }

    else
        throw std::runtime_error(exception);
}

bool Parser::next_symbol(std::vector<UL> &data_tokenized, Ponct symbol)
{
    // Check if the next token is the correct ponctuation
    if (data_tokenized.at(0).type == PONCT and data_tokenized.at(0).valeur == symbol)
    {
        // Move to the next token
        data_tokenized.erase(data_tokenized.begin());
        return true;
    }

    else
        return false;
}

bool Parser::next_keyword(std::vector<UL> &data_tokenized, Keyword keyword)
{
    // Check if the next token is the correct keyword
    if (data_tokenized.at(0).type == KEYWORD and data_tokenized.at(0).valeur == keyword)
    {
        // Move to the next token
        data_tokenized.erase(data_tokenized.begin());
        return true;
    }

    else
        return false;
}

void Parser::next_symbol_except(std::vector<UL> &data_tokenized, Ponct symbol, std::string exception)
{
    // Check if the next token is the correct ponctuation
    if (not next_symbol(data_tokenized, symbol))
        throw std::runtime_error(exception);
}

// ========================
// PRINT METHODS

void Parser::print_instr(instr i)
{
    std::string keyword = "";

    switch (i.type)
    {
    case Keyword::INIT:
        keyword = "INIT";
        break;

    case Keyword::DIAMETER:
        keyword = "DIAMETER";
        break;

    case Keyword::SPEED:
        keyword = "SPEED";
        break;

    default:
        keyword = "UNKNOWN";
        break;
    }

    printf("Instruction: %s\n", keyword.c_str());
    printf("Ident: %d\n", int(i.ident));
    printf("Value: %f\n\n", i.value);
}

void Parser::print_instr(std::vector<instr> instructions)
{
    for (auto &&i : instructions)
    {
        print_instr(i);
    }
}

void Parser::print_react(react r)
{
    printf("Enzyma: %d\n", int(r.ident));
    printf("Substrates: %d\n", int(r.substrate));
    printf("Products: %d\n", int(r.product));
    printf("mM: %f\n", r.mM);
    printf("kcat: %f\n\n", r.kcat);
}

void Parser::print_react(std::vector<react> reactions)
{
    for (auto &&r : reactions)
    {
        print_react(r);
    }
}