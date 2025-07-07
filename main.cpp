#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <string> 
#include <stdio.h>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>


namespace parsing {

    struct func_info {
        std::queue<std::string> rpn;
        std::list<std::string> param_names;

    } func;

    class Interpreter {
    public:
        void start() {
            std::string input;
            do {
                std::getline(std::cin, input);
                std::cout << interpret_line(input) << "\n";
            } while (input != "\n");
        };

        ~Interpreter() {
            for (auto i = functions.begin(); i != functions.end(); i++) {
                free(functions[i->first]);
            }
        };
    private:
        std::unordered_map<std::string, int> variables;
        std::unordered_map<std::string, func_info* > functions;


        bool IsStringNumber(std::string str) {

            if (str.size() == 0) {
                return false;
            }

            for (size_t i = 0; i < str.size(); ++i) {
                if (!isdigit(str[i])) {
                    return false;
                }
            }
            return true;
        }

        const std::string unary_operations[2] = { "abs","Negative" };
        const std::string binary_operations[7] = { "min","max","pow", "/", "*" ,"+","-" };

        const std::string recognized_non_number_tokens[12] = { "min","max","abs","pow","(",")","-","+","/","*",",","Negative" };
        const int recognized_non_number_tokens_count = 12;

        void printstringlist(std::list < std::string > list) {
            for (std::list < std::string >::iterator i = list.begin(); i != list.end(); ++i) {
                std::cout << " " << *i;
            }
            std::cout << "\n";

        }

        //operations priority
        //+- 
        //*/
        //functions
        const std::string priority0_operatr[2] = {"+","-"};
        const std::string priority1_operatr[2] = { "/","*" };
        const std::string priority2_operatr[5] = { "min","max","abs","pow","Negative" };
        

        bool is_string_in_recognized_non_number_tokens(std::string token) {
            if (variables.contains(token) || functions.contains(token)) {
                return true;
            }

            for (int i = 0; i < recognized_non_number_tokens_count;++i) {
                if (token == recognized_non_number_tokens[i]) {
                    return true;
                }
            }
            return false;
        }


        std::list < std::string > basic_parsing(std::string input) {//split into numbers functions and operators ignore blankspace and comma

            char cursymbol;
            std::string curtoken;
            std::list < std::string > tokens;
            for (size_t i = 0; i < input.size(); ++i) {
                cursymbol = input[i];

                if (cursymbol == ' ' || cursymbol == ',') {
                    if (curtoken.size() > 0) {
                        tokens.push_back(curtoken);
                        curtoken.clear();
                    }

                    if (cursymbol == ',') {
                        tokens.push_back(",");
                    }
                    continue;
                }

                if (isdigit(cursymbol)) {
                    curtoken.push_back(cursymbol);
                }
                else {
                    if (IsStringNumber(curtoken)) {
                        tokens.push_back(curtoken);
                        curtoken.clear();
                    }
                    curtoken.push_back(cursymbol);

                    if (is_string_in_recognized_non_number_tokens(curtoken)) {
                        tokens.push_back(curtoken);
                        curtoken.clear();
                    }
                }
            }

            if (curtoken.size() > 0) {
                tokens.push_back(curtoken);
                curtoken.clear();
            }
            return tokens;
        }

        std::list < std::string > detect_and_change_all_unary_minuses(std::list < std::string > tokens) {

            for (std::list < std::string >::iterator i = tokens.begin(), previous = tokens.end(); i != tokens.end(); previous = i, ++i) {
                std::string curtoken = *i;

                if (curtoken != "-") {
                    continue;
                }

                if (i == tokens.begin()) {
                    *i = "Negative";
                    continue;
                }

                std::string prevvalue = *previous;
                if (prevvalue == "," || prevvalue == "(") {
                    *i = "Negative";
                }

            }
            return tokens;
        }

        std::list < std::string > delete_commas(std::list < std::string > tokens) {
            tokens.remove(",");
            return tokens;
        }


        bool check_if_all_tokens_are_recognized(std::list < std::string > tokens) {
            for (std::list < std::string >::iterator i = tokens.begin(); i != tokens.end(); ++i) {
                std::string curtoken = *i;
                if (IsStringNumber(curtoken) || is_string_in_recognized_non_number_tokens(curtoken)) {
                    continue;
                }
                return false;
            }
            return true;
        }

        int operator_priority(std::string operatr) {
            int operand_priority = -1;
            if (priority0_operatr->contains(operatr)) {
                operand_priority = 0;
            }
            else if (priority1_operatr->contains(operatr)) {
                operand_priority = 1;
            }
            else if(priority2_operatr->contains(operatr) || functions.contains(operatr)){
                operand_priority = 2;
            }
            return operand_priority;
        }

        bool is_operatr1_higher_or_same_precedence_as_operatr2(std::string operatr1, std::string operatr2) {

            int operand1Priority = operator_priority(operatr1);
            int operand2Priority = operator_priority(operatr2);

            if (operand1Priority >= operand2Priority) {
                return true;
            }

            return false;
        }



        std::queue<std::string> convert_to_reverse_polish_notation(std::list < std::string > tokens) {

            std::queue<std::string> q;
            std::stack<std::string> s;
            std::string curtoken;

            for (std::list < std::string >::iterator i = tokens.begin(); i != tokens.end(); ++i) {

                curtoken = *i;

                if (IsStringNumber(curtoken) || variables.contains(curtoken)) {
                    q.push(curtoken);
                    continue;
                }

                if (curtoken == "(") {
                    s.push(curtoken);
                }
                else if (curtoken == ")") {

                    while (s.size() > 0) {
                        std::string out = s.top();
                        s.pop();

                        if (out == "(") {
                            break;
                        }
                        else {
                            q.push(out);
                        }
                    }
                }
                else {
                    while (s.size() > 0) {
                        std::string out = s.top();
                        if (is_operatr1_higher_or_same_precedence_as_operatr2(out, curtoken) && out != "(") {
                            s.pop();
                            q.push(out);

                        }
                        else {
                            break;
                        }
                    }
                    s.push(curtoken);
                }
            }

            while (s.size() > 0) {
                q.push(s.top());
                s.pop();
            }

            return q;

        }


        int execute_standard_binary_operation(std::string operation, int leftnum, int rightnum) {

            if (operation == "min") {
                return std::min(leftnum, rightnum);
            }
            else if (operation == "max") {
                return std::max(leftnum, rightnum);
            }
            else if (operation == "-") {
                return leftnum - rightnum;
            }
            else if (operation == "+") {
                return leftnum + rightnum;
            }
            else if (operation == "*") {
                return leftnum * rightnum;
            }
            else if (operation == "/") {
                return leftnum / rightnum;
            }
            else if (operation == "pow") {
                return (int)std::pow(leftnum, rightnum);
            }
            else {
                std::cout << "operation not found";
                exit(-1);
            }

        };

        int execute_standard_unary_operation(std::string operation, int num) {
            if (operation == "abs") {
                return std::abs(num);
            }
            else if (operation == "Negative") {
                return -num;
            }
            else {
                std::cout << "operation not found";
                exit(-1);
            }
        }

        int сalculate_rpn_expression(std::queue<std::string> expressionInRpn, std::unordered_map<std::string, int> curvariables) { //ReversePolishNotation = rpn

            std::string curtoken;
            std::stack<int> numbersStack;

            while (expressionInRpn.size() > 0) {

                curtoken = expressionInRpn.front();
                expressionInRpn.pop();

                if (IsStringNumber(curtoken)) {
                    numbersStack.push(std::stoi(curtoken));
                }
                else if (curvariables.contains(curtoken)) {
                    numbersStack.push(variables[curtoken]);
                }
                else {
                    if (unary_operations->contains(curtoken)) {

                        int num = numbersStack.top();
                        numbersStack.pop();
                        int result = execute_standard_unary_operation(curtoken, num);
                        numbersStack.push(result);

                    }
                    else if (binary_operations->contains(curtoken)) {

                        int rightnum = numbersStack.top();
                        numbersStack.pop();

                        int leftnum = numbersStack.top();
                        numbersStack.pop();

                        int result = execute_standard_binary_operation(curtoken, leftnum, rightnum);
                        numbersStack.push(result);
                    }
                    else if (functions.contains(curtoken)){

                        std::unordered_map<std::string, int> newcurvariables(variables);
                        
                        for (std::list<std::string>::iterator i = functions[curtoken]->param_names.begin();i != functions[curtoken]->param_names.end();++i) {
                            newcurvariables[*i] = numbersStack.top();
                            numbersStack.pop();
                        }
                       
                        int result = сalculate_rpn_expression(functions[curtoken]->rpn,newcurvariables);
                        numbersStack.push(result);
                    }
                }
            }
            return numbersStack.top();
        }

        std::list < std::string > parse_expression(std::string input) {
            std::list < std::string > output = basic_parsing(input);
            // printstringlist(output);

            output = detect_and_change_all_unary_minuses(output);

            // printstringlist(output);

            output = delete_commas(output);

            // printstringlist(output);
            return output;
        }

        std::queue<std::string> turn_parsed_expression_into_computable(std::list < std::string > parsed) {

            if (!check_if_all_tokens_are_recognized(parsed)) {
                std::cout << "unrecognized tokens";
                exit(-1);
            }
            std::queue<std::string> q = convert_to_reverse_polish_notation(parsed);
            /*std::queue<std::string> newq;
            while (q.size() > 0) {
                std::cout << q.front() << " ";
                newq.push(q.front());
                q.pop();
            }

            std::cout << "\n";*/

        }


        ///func definition f has def keyword
        ///variable definition v has var keyword
        ///expressions e
        char identify_user_input(std::string input) {

            std::list < std::string > list;

            const char* del = " ";

            char* t = strtok((char*)input.c_str(), " ");

            while (t != nullptr) {
                list.push_back(t);
                t = strtok(nullptr, del);
            }

            if (list.size() < 2) {
                return 'e';
            }

            std::list < std::string >::iterator i1 = list.begin();
            if (*i1 == "var") {
                return 'v';
            }
            else if (*i1 == "def") {
                return 'f';
            }
            else {
                return 'e';
            }
        }

        int interpret_line(std::string input) {

            char result = identify_user_input(input);

            if (result == 'e') {
                std::list < std::string > parsed = parse_expression(input);
                std::queue<std::string> q = convert_to_reverse_polish_notation(parsed);
                return сalculate_rpn_expression(q, variables);
            }

            if (result == 'v') {

                std::string variable_name;
                char cursymbol;
                int assigning_index = -1;

                for (int i = 0; i < input.size(); ++i) {
                    cursymbol = input[i];

                    if (cursymbol == '=') {
                        break;
                    }
                    variable_name.push_back(cursymbol);
                }

                for (int i = 0; i < input.size(); ++i) {
                    cursymbol = input[i];

                    if (cursymbol == ' ') {
                        continue;
                    }

                    if (cursymbol == '=') {
                        assigning_index = i;
                        break;
                    }

                    if (variable_name == "var") {
                        variable_name.clear();
                    }
                    variable_name.push_back(cursymbol);
                }

                if (assigning_index == -1) {
                    std::cout << "syntax error";
                    exit(-1);
                }

                std::string expression = input.substr(assigning_index + 1, input.size() - assigning_index - 1); 
                std::list < std::string > parsed = parse_expression(expression);
                std::queue<std::string> q = convert_to_reverse_polish_notation(parsed);
                variables[variable_name]  = сalculate_rpn_expression(q, variables);
                return 0;
            }

            if(result == 'f') {

                std::string curtoken;
                char cursymbol;
                int func_name_start_index = -1;
                int func_name_end_index = -1;

                for (int i = 0; i < input.size(); ++i) {
                    cursymbol = input[i];

                    if (cursymbol == ' ') {
                        func_name_start_index = i + 1;
                        continue;
                    }

                    if (cursymbol == '(') {
                        func_name_end_index = i - 1;
                        break;
                    }
                     
                    if (curtoken == "def") {
                        curtoken.clear();
                    }
                    curtoken.push_back(cursymbol);
                }

                if (func_name_end_index == -1 || func_name_start_index == -1 || func_name_start_index > input.size()) {
                    std::cout << "syntax error";
                    exit(-1);
                }

                
                int closing_brace_index = -1;
                
                for (int i = 0; i < input.size(); ++i) {
                    cursymbol = input[i];
                    if (cursymbol == ')') {
                        closing_brace_index = i;
                        break;
                    }
                }
               
                std::string expresion_with_curly_braces = input.substr(closing_brace_index + 1, input.size() - closing_brace_index - 1);

                for (int i = 0; i < expresion_with_curly_braces.size(); ++i) {
                    if (expresion_with_curly_braces[i] == '{' || expresion_with_curly_braces[i] == '}') {
                        expresion_with_curly_braces.erase(i);
                    }
                }

                std::list < std::string > parsed = parse_expression(expresion_with_curly_braces);

                std::queue<std::string> q = convert_to_reverse_polish_notation(parsed);

                func_info* info = new func_info;
                info->rpn = q;
                std::string parameters = input.substr(func_name_end_index + 1, closing_brace_index - func_name_end_index);

                const char* del = ",";

                char* t = strtok((char*)parameters.c_str(), del);

                while (t != nullptr) {
                    info->param_names.push_back(t);
                    t = strtok(nullptr, del);
                }
                
                std::string func_name = input.substr(func_name_start_index, func_name_end_index - func_name_start_index + 1);
                functions[func_name] = info;
                return 0;
            }
            return 0;
        }

   

    };

};





int main()
{
    std::string input;
    parsing::Interpreter* i = new parsing::Interpreter();
    i->start();
    delete i;

    return 0;
}

