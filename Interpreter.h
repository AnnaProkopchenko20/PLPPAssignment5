#pragma once
#ifndef INTEPRETER_H
#define INTEPRETER_H

#include <iostream> 
#include <string> 
#include <stdio.h>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>

struct func_info {
    std::queue<std::string> rpn;
    std::list<std::string> param_names;

};

class Interpreter
{public:
    void start();
    ~Interpreter();

private:
    std::unordered_map<std::string, int> variables;
    std::unordered_map<std::string, func_info* > functions;
    std::unordered_map<std::string, int> extra_temp_variables;
    std::string cur_input_line;

    bool isStringInStringList(std::string str, std::list<std::string> list);
    bool IsStringNumber(std::string str);

    const std::list<std::string> unary_operations = { "abs","Negative" };
    const std::list<std::string> binary_operations = { "min","max","pow", "/", "*" ,"+","-" };

    const std::string recognized_non_number_tokens[12] = { "min","max","abs","pow","(",")","-","+","/","*",",","Negative" };
    const int recognized_non_number_tokens_count = 12;

    const std::list<std::string> priority0_operatr = { "+","-","Negative" };
    const std::list<std::string> priority1_operatr = { "/","*" };
    const std::list<std::string> priority2_operatr = { "min","max","abs","pow" };

    void printstringlist(std::list < std::string > list);

    bool is_string_in_recognized_non_number_tokens(std::string token);

    std::list < std::string > basic_parsing(std::string input);
    
    std::list < std::string > detect_and_change_all_unary_minuses(std::list < std::string > tokens);
    
    std::list < std::string > delete_commas(std::list < std::string > tokens);
    
    bool check_if_all_tokens_are_recognized(std::list < std::string > tokens);
    
    int operator_priority(std::string operatr);
    
    bool is_operatr1_higher_or_same_precedence_as_operatr2(std::string operatr1, std::string operatr2);
    
    std::queue<std::string> convert_to_reverse_polish_notation(std::list < std::string > tokens);
    
    int execute_standard_binary_operation(std::string operation, int leftnum, int rightnum);
    
    int execute_standard_unary_operation(std::string operation, int num);
    
    int calculate_rpn_expression(std::queue<std::string> expressionInRpn, std::unordered_map<std::string, int> curvariables);
    
    std::list < std::string > parse_expression(std::string input);
    
    void erase_white_space_from_string(std::string& str);
    
    std::queue<std::string> turn_parsed_expression_into_computable(std::list < std::string > parsed);
    
    void parse_function_definition_input(std::string input);
    
    void parse_variable_definition_input(std::string input);
    
    char identify_user_input(std::string input);
    
    int interpret_line(std::string input);
};

#endif 



