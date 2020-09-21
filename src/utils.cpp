#include "utils.h"
#include <Windows.h>

namespace mutantur
{
    namespace utils
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

        void merge_clone_file(int exit_code, const char* clone_filename, const char* target_filename, const char* cloning_bat_filename)
        {
            // create a .bat file in the temp folder
            char buf[MAX_PATH] = { 0 };
            GetTempPathA(MAX_PATH, buf);
            std::string script_path = std::string(buf) + "\\mtclonescrpt.bat";

            auto clone_path = std::string(clone_filename);
            auto target_path = std::string(target_filename);

            std::string script_src = std::string("@echo off\n") +
                "timeout 1 > nul\n" +
                "set clone=\"" + clone_path + "\"\n" +
                "set target=\"" + target_path + "\"\n" +
                "del %target% 2>nul\n" +
                "call :expand %target%\n" +
                ":expand\n" +
                "set target_filename=%~nx1\n" +
                "ren %clone% %target_filename% 2>nul\n" +
                "%target%\n" +
                "(goto) 2>nul & del \"%~f0\" 2>nul";

            std::ofstream script(script_path, std::ios::out);
            script << script_src;
            script.close();

            STARTUPINFOA info = { sizeof(info) };
            PROCESS_INFORMATION processInfo;
            CreateProcessA(script_path.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo);
            
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            exit(exit_code);
        }
    }
}
