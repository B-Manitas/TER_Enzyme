#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <map>

#include "enums.hpp"
#include "types.hpp"

/**
 * @brief The Parser class is used to parse the tokenized data.
 */
class Parser
{
public:
    /**
     * @brief Parse the tokenized data
     *
     * @param data_tokenized The tokenized data
     * @param reactions The vector of reactions to fill
     * @param instructions The vector of instructions to fill
     */
    void parse(std::vector<UL> data_tokenized, std::vector<react> &reactions, std::vector<instr> &instructions);

    /**
     * @brief Parse a reaction from the tokenized vector
     * For example: "e" : "s" -> "p" | 200uH - 100;
     *
     * @param data_tokenized The tokenized data
     * @return react The reaction
     */
    react reaction(std::vector<UL> &data_tokenized);
    /**
     * @brief Parse an instruction from the tokenized vector
     * For example: init(e) = 30;
     *
     * @param data_tokenized The tokenized data
     * @return instr The instruction
     */
    instr instruction(std::vector<UL> &data_tokenized);

    /**
     * @brief Parse a series of reactions from the tokenized vector
     * For example:
     * "e1" : "s1" -> "p1" | 200uH - 100;
     * "e2" : "s2" -> "p2" | 200uH - 100;
     *
     * @param data_tokenized The tokenized data
     * @return std::vector<react> The reactions
     */
    std::vector<react> reactions_series(std::vector<UL> data_tokenized);
    /**
     * @brief Parse a series of instructions from the tokenized vector
     * For example:
     * init(e1) = 30;
     * init(e2) = 40;
     *
     * @param data_tokenized The tokenized data
     * @return std::vector<instr> The instructions
     */
    std::vector<instr> instructions_series(std::vector<UL> data_tokenized);

    /**
     * @brief Parse a series of identifications from the tokenized vector
     * For example: "s1" "s2"
     *
     * @param data_tokenized The tokenized data
     * @return std::map<float, float> The identifications
     */
    std::map<float, float> idents_series(std::vector<UL> &data_tokenized);

    /**
     * @brief Parse a series of mM from the tokenized vector
     * For example: 200mM 200mM
     *
     * @param data_tokenized The tokenized data
     * @return std::map<float, float> The mM
     */
    std::map<float, float> mM_series(std::vector<UL> &data_tokenized);

    /**
     * @brief Get the value of the current token and move to the next one
     * Check if the token is of the right type and throw an exception if not
     *
     * @param data_tokenized The tokenized data
     * @param type The type of the token
     * @param exception The exception message if the token is not found
     * @return float The value of the current token
     */
    float next_token(std::vector<UL> &data_tokenized, State type, std::string exception);
    /**
     * @brief Check if the next token is the correct ponctuation and move to the next one
     *
     * @param data_tokenized The tokenized data
     * @param Ponct The ponctuation to find
     * @return bool True if the ponctuation is found and false otherwise
     */
    bool next_symbol(std::vector<UL> &data_tokenized, Ponct symbol);
    /**
     * @brief Check if the next token is the correct keyword and move to the next one
     *
     * @param data_tokenized The tokenized data
     * @param Keyword The keyword to find
     * @return bool True if the keyword is found and false otherwise
     */
    bool next_keyword(std::vector<UL> &data_tokenized, Keyword keyword);
    /**
     * @brief Check if the next token is the correct ponctuation and move to the next one
     * If the next token is not the correct ponctuation, throw an exception
     *
     * @param data_tokenized The tokenized data
     * @param Ponct The ponctuation to find
     * @param exception The exception message if the ponctuation is not found
     */
    void next_symbol_except(std::vector<UL> &data_tokenized, Ponct symbol, std::string exception);

    /**
     * @brief Print an instruction
     *
     * @param i The instruction
     */
    void print_instr(instr i);
    /**
     * @brief Print a series of instructions
     *
     * @param instructions The instructions
     */
    void print_instr(std::vector<instr> instructions);
    /**
     * @brief Print a reaction
     *
     * @param r The reaction
     */
    void print_react(react r);
    /**
     * @brief Print a series of reactions
     *
     * @param reactions The reactions
     */
    void print_react(std::vector<react> reactions);
};

#endif
