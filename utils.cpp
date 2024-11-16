#include "utils.h"
#include <sys/stat.h>

Response run_cmd(string cmd)
{
	cmd.append(" 2>&1");
	auto *pipe = popen(cmd.c_str(), "r");

	if (!pipe) {
		return { "Error while running " + cmd, 1 };
	}

	const int bufer_size = 4096;
	char buffer[bufer_size]{};
	string out;
	while (fgets(buffer, bufer_size, pipe) != nullptr) {
		out.append(buffer);
	}

	const auto status = pclose(pipe);

	return { out, status };
}

bool file_exists(const char *file_path)
{
	struct stat buffer{};
	return stat(file_path, &buffer) == 0;
}
