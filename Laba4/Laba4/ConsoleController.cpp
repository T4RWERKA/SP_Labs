#include "Console.h"
#include "File.h"
#include <thread>

namespace Console
{
	// Get filename and read from it
	void OutputReadFromFile(std::vector<std::string> &stringsFromFile)
	{
		bool fileExists = false;

		while (!fileExists)
		{
			std::string filename;
			std::cout << "Enter the name of a file with strings: ";
			std::cin >> filename;
			std::cout << std::endl;

			if (!File::ReadFromFile(filename, stringsFromFile))
				std::cout << "File is not found" << std::endl;
			else
				fileExists = true;
		}
	}

	// Get filename and write in it
	void OutputWriteToFile(std::vector<std::string> &stringsFromFile)
	{
		std::string filename;
		std::cout << "Enter the name of a result file: ";
		std::cin >> filename;
		std::cout << std::endl;

		File::WriteToFile(filename, stringsFromFile);

		std::cout << "Strings has been written into file" << std::endl;
	}

	int GetThreadCount(size_t stringsCount)
	{
		// max number of concurrent threads	
		int maxThreadCount = std::thread::hardware_concurrency();
		int threadCount;
		bool success = false;

		if (maxThreadCount > stringsCount)
			maxThreadCount = stringsCount;

		while (!success)
		{

			std::cout << "Max number of concurrent threads: " << maxThreadCount << std::endl;
			std::cout << "Enter the number of threads: ";
			std::cin >> threadCount;
			std::cout << std::endl;
			if ((threadCount > maxThreadCount) || (threadCount <= 0))
			{
				std::cout << "Wrong number of threads." << std::endl;
			}
			else
			{
				success = true;
			}
		}

		return threadCount;
	}
}

