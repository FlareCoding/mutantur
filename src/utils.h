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
	namespace utils
	{
		std::string hex_dump(unsigned char* address, size_t size);

		size_t calculate_function_size(unsigned char* address);

		byte calculate_function_insertion_space(size_t fn_len);

		int random_in_range(int low, int high);

		void merge_clone_file(int exit_code, const char* clone_filename, const char* target_filename, const char* cloning_bat_filename = 0);
	}
}
