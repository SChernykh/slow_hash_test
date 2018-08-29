#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <chrono>

extern "C"
{
	#include "hash-ops.h"
}

enum
{
	NUM_VARIANTS = 3,
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Test file should be passed as first command line parameter" << std::endl;
		return 0;
	}

	const bool generate_hashes = (argc > 2) && (strcmp(argv[2], "generate") == 0);
	bool all_passed = true;

	std::ifstream f(argv[1]);

	auto t1 = std::chrono::high_resolution_clock::now();
	while (!f.eof())
	{
		std::string input;
		std::getline(f, input);
		if (input.empty())
		{
			continue;
		}

		if (generate_hashes)
		{
			std::cout << input << std::endl;
		}

		for (int i = 0; i < NUM_VARIANTS; ++i)
		{
			char hash[32];
			cn_slow_hash(input.c_str(), input.length(), hash, i, 0);

			if (generate_hashes)
			{
				for (int j = 0; j < HASH_SIZE; ++j)
				{
					std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(hash[j]));
				}
				std::cout << std::endl;
			}
			else
			{
				std::string output;
				std::getline(f, output);
				if (output.length() != HASH_SIZE * 2)
				{
					std::cerr << "Invalid test file" << std::endl;
					return 1;
				}

				char reference_hash[HASH_SIZE];
				for (int j = 0; j < HASH_SIZE; ++j)
				{
					reference_hash[j] = static_cast<char>(std::stoul(output.substr(j * 2, 2), 0, 16));
				}

				if (memcmp(hash, reference_hash, HASH_SIZE) != 0)
				{
					all_passed = false;
					std::cerr << "Hash test failed for string \"" << input << "\", variant " << i << std::endl;
					std::cerr << "Reference hash:  " << output << std::endl;
					std::cerr << "Calculated hash: ";
					for (int j = 0; j < HASH_SIZE; ++j)
					{
						std::cerr << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(hash[j]));
					}
					std::cerr << std::endl;
				}
			}
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	if (!generate_hashes && all_passed)
	{
		std::cout << "All tests passed in " << std::chrono::duration<double>(t2 - t1).count() << " seconds" << std::endl;
	}
	return 0;
}
