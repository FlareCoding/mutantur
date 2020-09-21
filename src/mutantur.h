#pragma once
#include "utils.h"

namespace mutantur
{
	class mutant
	{
	public:
		/// If filename is passed in, it reads the executable into a byte array.
		mutant(const char* filename = 0);
		~mutant();

		/// Reads the executable into a byte array.
		/// @param filename Name of the program to read the contents of.
		/// @returns Success status.
		bool open(const char* filename);

		/// Creates a new file on disk and writes the mutated code to it.
		/// @param filename Name of the file to create and save the mutation to.
		void finalize_mutation(const char* filename);

		/// Mutates the function's code.
		/// @param address Known start address of the function.
		/// @param fn_len Known length of the function in bytes.
		void mutate(byte* address, size_t fn_len);

	private:
		std::ifstream m_stream;
		std::vector<byte> m_source;
		bool read_file_source(const char* filename);

	private:
		size_t find_function_start_index(byte* address, size_t fn_len);
		void insert_mutation_flag_and_body(size_t start_idx, size_t fn_len, byte insertion_space);
		void mutate_function(size_t fn_start_idx);
	};
}
