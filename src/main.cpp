//
// main.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

#include "xylo.hpp"

int main(int args, char *argv[])
{
    std::fstream ifs(argv[1]);

    if (!ifs)
    {
        printf("failed to open file");
        return 0;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();

    std::string code_str(ss.str());

    return 0;
}
