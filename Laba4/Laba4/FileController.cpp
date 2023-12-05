#include "File.h"
#include <fstream>

namespace File
{
	bool ReadFromFile(std::string const &filename, std::vector<std::string> &strings)
	{
		std::ifstream reader(filename);
		std::string str;

		if (!reader.is_open())
		{
			reader.close();
			return false;
		}

		while (!reader.eof())
		{
			getline(reader, str, '\n');
			strings.push_back(str);
		}
		reader.close();
		return true;
	}

	void WriteToFile(std::string const &filename, std::vector<std::string> &strings)
	{
		std::ofstream writer(filename);

		for (std::string str : strings)
		{
			const char *char_str = str.c_str();
			writer.write(char_str, strlen(char_str));
			writer.write("\n", 1);
		}
		writer.close();
	}
}

