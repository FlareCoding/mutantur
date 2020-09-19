#include "utils.h"

namespace mutantur
{
    namespace util
    {
        std::string hex_dump(unsigned char* address, size_t size)
        {
            std::stringstream ss;
            ss.fill('0');
            for (size_t i = 0; i < size; i += 16)
            {
                ss << "0x" << std::setw(6) << std::hex << i << ": ";
                for (int j = 0; j < 16; ++j)
                {
                    if (i + j < size)
                        ss << std::hex << std::setw(2) << static_cast<int>(address[i + j]) << " ";
                    else
                        ss << "   ";
                }

                ss << " ";

                for (size_t j = 0; j < 16; ++j)
                {
                    if (i + j < size)
                    {
                        if (std::isprint(address[i + j]))
                            ss << static_cast<char>(address[i + j]);
                        else
                            ss << ".";
                    }
                }

                ss << "\n";
            }

            return ss.str();
        }

        size_t calculate_function_size(unsigned char* address)
        {
            size_t len = 0;
            bool end = false;
            while (!end)
            {
                if (*address == 0xC2 && *(address + 2) == 0x00)
                    return len + 3;

                if (*address == 0xC3 && *(address + 2) == 0xCC)
                    return len + 3;

                len++;
                address++;
            }

            return len;
        }

        byte calculate_function_insertion_space(size_t fn_len)
        {
            byte remainder = (byte)fn_len % 16;
            if (!remainder)
                return 0;

            return 16 - remainder;
        }

        int random_in_range(int low, int high)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(low, high);
            return distr(gen);
        }
    }
}
