#pragma once
#include <cinttypes>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>

namespace mutantur
{
	typedef unsigned char byte;
	namespace util
	{
		std::string hex_dump(unsigned char* address, size_t size);

		size_t calculate_function_size(unsigned char* address);

		byte calculate_function_insertion_space(size_t fn_len);

		int random_in_range(int low, int high);
	}
}
