#pragma once

#include <string>
#include <vector>

namespace File
{
	bool ReadFromFile(std::string const& filename, std::vector<std::string>& strings);

	void WriteToFile(std::string const& filename, std::vector<std::string>& strings);
};
