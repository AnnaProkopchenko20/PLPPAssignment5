#include <iostream> 
#include <string> 
#include <stdio.h>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>


//how to control user do using whitespace between something correct 
//like inta and int a


namespace parsing {

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

    const std::string recognizedOperators = "-+/*()";
    const std::string recognizedSymbols = recognizedOperators + "abcdefghijklmnopqrstuvwxyz" + " " + "1234567890" + ",";
    const std::string recognized_non_number_tokens[12] = {"min","max","abs","pow","(",")","-","+","/","*",",","Negative"};
    const int recognized_non_number_tokens_count = 12;

    bool isprelastnonblanksymbolacommaordoesntexist(int curindex, std::string input) {
        for (int i = curindex - 1; i >= 0; --i) {

            if (input[i] != ',' && input[i] != ' ') {
                return false;
            }

            if (input[i] == ',') {
                return true;
            }

        }
        return true;
    }


    bool isprelastnonblanksymbolaopeningbracket(int curindex, std::string input) {
        for (int i = curindex - 1; i >= 0; --i) {

            if (input[i] != '(' && input[i] != ' ') {
                return false;
            }

            if (input[i] == '(') {
                return true;
            }

        }
        return false;
    }
    bool is_string_in_recognized_non_number_tokens(std::string token) {

        for (int i = 0; i < recognized_non_number_tokens_count;++i) {
            if (token == recognized_non_number_tokens[i]) {
                return true;
            }
        }
        return false;
    }


    std::list < std::string > BasicParsing(std::string input) {//split into numbers functions and operators ignore blankspace and comma
        
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

    

    bool is_operand1_higher_or_same_precedence_as_operand2(std::string operand1, std::string operand2) {

        //operations priority
        //+- 
        //*/
        //functions
        int operand1Priority = -1;
        int operand2Priority = -1;

        std::string precedenceByIndex[3];
        precedenceByIndex[0] = "+-";
        precedenceByIndex[1] = "*/";
        precedenceByIndex[2] = "minmaxpowabsNegative";

        for (int i = 0;i < 3;++i) {
            if (precedenceByIndex[i].contains(operand1)) {
                operand1Priority = i;
            }
        }

        for (int i = 0;i < 3; ++i) {
            if (precedenceByIndex[i].contains(operand2)) {
                operand2Priority = i;
            }
        }

        if (operand1Priority >= operand2Priority) {
            return true;
        }

        return false;
    }



    std::queue<std::string> ConvertToReversePolishNotation(std::list < std::string > tokens) {

        std::queue<std::string> q;
        std::stack<std::string> s;
        std::string curtoken;

        for (std::list < std::string >::iterator i = tokens.begin(); i != tokens.end(); ++i) {

            curtoken = *i;

            if (IsStringNumber(curtoken)) {
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
                    if (is_operand1_higher_or_same_precedence_as_operand2(out, curtoken) && out != "(") {
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


    int executeBinaryOperation(std::string operation, int leftnum, int rightnum) {

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
            return std::pow(leftnum, rightnum);
        }
        else {
            std::cout << "operation not found";
            exit(-1);
        }

    };

    int executeUnaryOperation(std::string operation, int num) {
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

    int Calculate(std::queue<std::string> expressionInRpn, std::unordered_map<std::string, int> context) { //ReversePolishNotation = rpn

        std::string curtoken;
        std::stack<int> numbersStack;

        while (expressionInRpn.size() > 0) {

            curtoken = expressionInRpn.front();
            expressionInRpn.pop();

            if (IsStringNumber(curtoken)) {
                numbersStack.push(std::stoi(curtoken));
            }
            else if (context.find(curtoken) != context.end()) {
                numbersStack.push(context[curtoken]);
            }
            else {
                if (curtoken == "abs" || curtoken == "Negative") {
                   
                    int num = numbersStack.top();
                    numbersStack.pop();
                    int result = executeUnaryOperation(curtoken, num);
                    numbersStack.push(result);

                }
                else {
                    
                    int rightnum = numbersStack.top();
                    numbersStack.pop();

                    int leftnum = numbersStack.top();
                    numbersStack.pop();

                    int result = executeBinaryOperation(curtoken, leftnum, rightnum);
                    numbersStack.push(result);
                }

            }


        }
        return numbersStack.top();
    }




};



int main()
{
    std::string input;
    std::getline(std::cin, input);

    std::list < std::string > output = parsing::BasicParsing(input);

    for (std::list < std::string >::iterator i = output.begin(); i != output.end(); ++i) {
        std::cout << " " << *i;
    }
    std::cout << "\n";

    output = parsing::detect_and_change_all_unary_minuses(output);

    for (std::list < std::string >::iterator i = output.begin(); i != output.end(); ++i) {
        std::cout << " " << *i;
    }
    std::cout << "\n";

    output = parsing::delete_commas(output);

    for (std::list < std::string >::iterator i = output.begin(); i != output.end(); ++i) {
        std::cout << " " << *i;
    }
    std::cout << "\n";

    std::unordered_map<std::string, int> context;

    std::queue<std::string> q = parsing::ConvertToReversePolishNotation(output);
    
    std::queue<std::string> newq;
    while (q.size() > 0) {
        std::cout << q.front() << " ";
        newq.push(q.front());
        q.pop();
    }

    std::cout << "\n";

    std::cout << parsing::Calculate(newq, context);

    return 0;
}

