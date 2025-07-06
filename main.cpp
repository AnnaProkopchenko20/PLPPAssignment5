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
//how to unary minus
//how to control i fuser enters something in recognized tokens


namespace parsing {

    const std::string recognizedOperators = "-+/*()";
    const std::string recognizedSymbols = recognizedOperators + "abcdefghijklmnopqrstuvwxyz" + " " + "1234567890" + ",";

    std::list < std::string > Parse(std::string input) {

        char cursymbol;
        bool iscursymbolblankspaceOrcomma = false;
        std::string curNum;
        std::string funcOrVar;
        std::string operand;
        std::list < std::string > tokens;

        for (size_t i = 0; i < input.size(); ++i) {
            iscursymbolblankspaceOrcomma = false;

            cursymbol = input[i];

            if (isdigit(cursymbol)) {
                curNum.push_back(cursymbol);
            }
            else if (isalpha(cursymbol)) {//what if variable name has number in it
                funcOrVar.push_back(cursymbol);
            }
            else if (recognizedOperators.contains(cursymbol)) {
                operand.push_back(cursymbol);
            }
            else if (cursymbol == ' ' || cursymbol == ',') {
                iscursymbolblankspaceOrcomma = true;
            }
            else {
                std::cout << "unrecognized input: " << cursymbol;
                exit(-1);
            }

            if (iscursymbolblankspaceOrcomma) {
                if ((curNum.length() > 0) + (funcOrVar.length() > 0) + (operand.length() > 0) > 1) {
                    std::cout << "incorrect input";
                    exit(-1);
                }

                if (curNum.length() > 0) {
                    tokens.push_back(curNum);
                    curNum.clear();
                }
                else if (funcOrVar.length() > 0) {
                    tokens.push_back(funcOrVar);
                    funcOrVar.clear();
                }
                else if (operand.length() > 0) {
                    tokens.push_back(operand);
                    operand.clear();
                }

            }
            else {
                if (isdigit(cursymbol)) {
                    if (funcOrVar.length() > 0 && operand.length() > 0) {
                        std::cout << "incorrect input";
                        exit(-1);
                    }

                    if (funcOrVar.length() > 0) {
                        tokens.push_back(funcOrVar);
                        funcOrVar.clear();
                    }
                    else if (operand.length() > 0) {
                        tokens.push_back(operand);
                        operand.clear();
                    }

                }
                else if (isalpha(cursymbol)) {
                    if (curNum.length() > 0 && operand.length() > 0) {
                        std::cout << "incorrect input";
                        exit(-1);
                    }

                    if (curNum.length() > 0) {
                        tokens.push_back(curNum);
                        curNum.clear();
                    }
                    else if (operand.length() > 0) {
                        tokens.push_back(operand);
                        operand.clear();
                    }
                }
                else if (recognizedOperators.contains(cursymbol)) {
                    if (curNum.length() > 0 && funcOrVar.length() > 0) {
                        std::cout << "incorrect input";
                        exit(-1);
                    }

                    if (curNum.length() > 0) {
                        tokens.push_back(curNum);
                        curNum.clear();
                    }
                    else if (funcOrVar.length() > 0) {
                        tokens.push_back(funcOrVar);
                        funcOrVar.clear();
                    }
                }
            }
        }

        if (curNum.length() > 0) {
            tokens.push_back(curNum);
        }
        else if (funcOrVar.length() > 0) {
            tokens.push_back(funcOrVar);
        }
        else if (operand.length() > 0) {
            tokens.push_back(operand);
        }

        return tokens;
    }

    bool IsStringNumber(std::string str) {
        for (size_t i = 0; i < str.size(); ++i) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }


    bool is_operand1_higher_or_same_precedence_as_operand2(std::string operand1, std::string operand2) {

        //operations priority
        //+- 
        //*/
        //^
        //functions
        int operand1Priority = -1;
        int operand2Priority = -1;

        std::string precedenceByIndex[3];
        precedenceByIndex[0] = "+-";
        precedenceByIndex[1] = "*/";
        precedenceByIndex[2] = "minmax";

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
        else {
            std::cout << "operation not found";
            exit(-1);
        }

    };

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
                //unary or binary operation? i think i dont have unary operations for know
                int rightnum = numbersStack.top();
                numbersStack.pop();
                int leftnum = numbersStack.top();
                numbersStack.pop();

                int result = executeBinaryOperation(curtoken, leftnum, rightnum);
                numbersStack.push(result);

            }


        }
        return numbersStack.top();
    }




};



int main()
{
    std::string input;
    std::getline(std::cin, input);

    std::list < std::string > output = parsing::Parse(input);

    for (std::list < std::string >::iterator i = output.begin(); i != output.end(); ++i) {
        std::cout << " " << *i;
    }
    std::cout << "\n";

    std::unordered_map<std::string, int> context;

    std::queue<std::string> q = parsing::ConvertToReversePolishNotation(output);

    std::cout << "\n";

    std::cout << parsing::Calculate(q, context);

    return 0;
}


//python script is for testing
//for now I should create the logic
//first milestone
//Basic arithmetic operations: +, -, *, /
//6 points:
//> 5 + 3 * 2
//> 11 #  < ----here and below it is an answer

//reverse polish notation 
//get input
//tokenize
//convert to reverse polish notation
//calculate
