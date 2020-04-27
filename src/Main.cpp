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

struct ArgInfo
{
	std::string source_path;

	bool emit_ast = false;

	bool emit_bytecode = false;
};

ArgInfo ReadArg(int args, char* argv[])
{
	ArgInfo argInfo;

	for (int i = 1; i < args; i++)
	{
		auto arg = std::string(argv[i]);

		if (arg == "-emit-ast")
			argInfo.emit_ast = true;
		else if (arg == "-emit-bytecode")
			argInfo.emit_bytecode = true;
		else
			argInfo.source_path = arg;
	}

	return argInfo;
}

int main(int args, char* argv[])
{
	auto arg_info = ReadArg(args, argv);

	std::fstream ifs(arg_info.source_path);

	if (!ifs)
	{
		printf("failed to open file");
		return 0;
	}

	std::stringstream ss;
	ss << ifs.rdbuf();
	ifs.close();

	std::string source(ss.str());

	Xylo eval{ source };

	if (arg_info.emit_ast)
		eval.OutAST();

	if (arg_info.emit_bytecode)
		eval.OutCode();

	eval.Run();

	return 0;
}