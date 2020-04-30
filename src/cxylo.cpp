//
// Created by Katsu Matsuda on 2020/04/30.
//

#include "xylo.hpp"
#include "cxylo.h"

void* CreateXylo()
{
	auto* xylo = new Xylo{ "" };

	return xylo;
}
