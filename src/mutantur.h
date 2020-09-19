#pragma once
#include "utils.h"

namespace mutantur
{
	class mutant
	{
	public:

		mutant(const char* filename = 0);
		~mutant();

		bool open(const char* filename);
		void finalize_mutation(const char* filename);

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
