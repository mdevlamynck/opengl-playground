
#include <iostream>
#include <cstdlib>

#include "Engine.hpp"

int main(int argc, char* argv[])
{
    Engine eng;

    eng.init();
    eng.run();
    eng.release();

    return EXIT_SUCCESS;
}

