#include <iostream>
#include <string>

#include "speak.h"

namespace sarah_matrix
{

    void speak::run(std::string in)
    {
        std::cout << "speak::run() - " << in << std::endl;
    }

}
