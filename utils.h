#pragma once

#include <string>

using std::string;

struct Response
{
	string text;
	int status{};
};

Response run_cmd(string cmd);
bool file_exists(const char *file_path);
