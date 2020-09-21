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
		/// Displays the contents of the address in hex as well as showing the addresses.
		/// @param address Pointer to the address you want to start displaying the bytes from.
		/// @param size How many bytes to read and display.
		/// @returns The string representation of the contents of the address.
		std::string hex_dump(unsigned char* address, size_t size);

		/// Calculates the length of a function in bytes.
		/// @param address Function start address.
		/// @returns Length of the function in bytes.
		size_t calculate_function_size(unsigned char* address);

		/// Calculates the number of bytes that can fit into the function's padding space.
		byte calculate_function_insertion_space(size_t fn_len);

		/// Returns a random number in range (exclusive).
		int random_in_range(int low, int high);

		/// Creates a process that will exit the current application and rename the clone program to the current application's name.
		/// @param exit_code Code to be used in the exit() call. Usually should be 0.
		/// @param clone_filename Name of the mutated clone executable.
		/// @param target_filename Name that mutated clone should be renamed to, usually is the name of the current executable.
		/// @param cloning_bat_filename (Optional) name of the batch file that will be temporarily created.
		/// @returns Will exit the application by calling exit(exit_code);
		void merge_clone_file(int exit_code, const char* clone_filename, const char* target_filename, const char* cloning_bat_filename = 0);
	}
}
