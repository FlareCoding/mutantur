#include "mutantur.h"

#define MUTANTUR_MUTATION_FL	0xB4
#define MUTANTUR_NOP_FL			0x90

namespace mutantur
{
	static std::vector<std::vector<byte>> mutation_instructions_2op =
	{
		{ 0x89, 0xDB },
	};

	static std::vector<std::vector<byte>> mutation_instructions_3op =
	{
		{ 0x4D, 0x89, 0xF6 },
		{ 0x4D, 0x89, 0xFF },
		{ 0x4D, 0x89, 0xED },
		{ 0x48, 0x89, 0xC0 },
		{ 0x48, 0x89, 0xDB },
		{ 0x48, 0x89, 0xE4 },
		{ 0x48, 0x89, 0xED },
	};

	mutant::mutant(const char* filename)
	{
		if (filename)
			read_file_source(filename);
	}

	mutant::~mutant()
	{
		m_stream.close();
	}

	bool mutant::open(const char* filename)
	{
		if (!m_stream.is_open())
		{
			return read_file_source(filename);
		}

		return false;
	}

	bool mutant::read_file_source(const char* filename)
	{
		m_stream.open(filename, std::ios::in | std::ios::binary);
		if (m_stream.fail())
			return false;

		m_stream.unsetf(std::ios::skipws);

		m_stream.seekg(0, std::ios::end);
		std::streampos size = m_stream.tellg();
		m_stream.seekg(0, std::ios::beg);

		m_source.reserve(size);
		m_source.insert(m_source.begin(),
			std::istream_iterator<byte>(m_stream),
			std::istream_iterator<byte>());

		return true;
	}

	void mutant::finalize_mutation(const char* filename)
	{
		m_stream.close();
		
		std::ofstream file(filename, std::ios::out | std::ios::binary);
		file.write((const char*)&m_source[0], m_source.size());
		file.close();
	}

	size_t mutant::find_function_start_index(byte* address, size_t fn_len)
	{
		size_t function_start_index = 0;
		for (size_t i = 0; i < m_source.size(); i++)
		{
			if (m_source[i] == *address &&
				m_source[i + 1] == *(address + 1) &&
				m_source[i + 2] == *(address + 2) &&
				m_source[i + 3] == *(address + 3)
				)
			{
				if (memcmp(&m_source[i], address, fn_len) == 0)
				{
					function_start_index = i;

					// checking for mutated code before the actual function chunk
					for (size_t j = 0; j < 14; j++)
					{
						if (m_source[i - j] == 0xB4 &&
							(int)m_source[i - j + 1] < 16
							)
						{
							function_start_index = i - j;
						}
					}
					break;
				}
			}
		}
		return function_start_index;
	}

	void mutant::insert_mutation_flag_and_body(size_t start_idx, size_t fn_len, byte insertion_space)
	{
		byte mutation_length = insertion_space - 2;

		// making the space for mutation bytes by erasing the dead bytes from the end of the function
		m_source.erase(m_source.begin() + start_idx + fn_len, m_source.begin() + start_idx + fn_len + insertion_space);

		//  mov ah, mutation_length
		m_source.insert(m_source.begin() + start_idx, { MUTANTUR_MUTATION_FL, mutation_length });

		// filling the mutation body with nop instructions
		for (byte i = 0; i < mutation_length; i++)
			m_source.insert(m_source.begin() + start_idx + 2, MUTANTUR_NOP_FL);
	}

	void mutant::mutate_function(size_t fn_start_idx)
	{
		byte mutation_length = m_source[fn_start_idx + 1];
		size_t mutation_start_index = fn_start_idx + 2;

		size_t i = 0;
		while ((byte)i <= mutation_length)
		{
			if (i + 3 <= mutation_length)
			{
				auto& instructions = mutation_instructions_3op[util::random_in_range(0, (int)mutation_instructions_3op.size() - 1)];
				memcpy_s(&m_source[mutation_start_index + i], 3, &instructions[0], 3);
				i += 3;
			}
			else if (i + 2 <= mutation_length)
			{
				auto& instructions = mutation_instructions_2op[util::random_in_range(0, (int)mutation_instructions_2op.size() - 1)];
				memcpy_s(&m_source[mutation_start_index + i], 2, &instructions[0], 2);
				i += 2;
			}
			else
				i++;
		}
	}

	void mutant::mutate(byte* address, size_t fn_len)
	{
		size_t function_start_index = find_function_start_index(address, fn_len);
		if (!function_start_index)
			return;

		fn_len = util::calculate_function_size(&m_source[function_start_index]);
		byte insertion_space = util::calculate_function_insertion_space(fn_len);

		if (insertion_space >= 2)
		{
			insert_mutation_flag_and_body(function_start_index, fn_len, insertion_space);
		}
		else if (m_source[function_start_index] == MUTANTUR_MUTATION_FL)
		{
			mutate_function(function_start_index);
		}
	}
}
