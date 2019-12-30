//
// main.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory>

int main()
{
    std::unique_ptr<int[]> p(new int[10]);

    p[0] = 1;
    p[1] = 2;
    p[2] = 8;

    printf("%d %d %d\n", p[0], p[1], p[2]);

    int *np = (int *)realloc(p.get(), 20 * sizeof(int));

    p.release();
    p.reset(np);

    p[19] = 3;

    printf("%d %d %d\n", p[0], p[1], p[2]);
    printf("%d\n", p[19]);

    return 0;
}