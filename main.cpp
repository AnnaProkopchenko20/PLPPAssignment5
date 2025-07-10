#include "Interpreter.h"


int main()
{
    std::string input;
    Interpreter* i = new Interpreter();
    i->start();
    delete i;

    return 0;
}

