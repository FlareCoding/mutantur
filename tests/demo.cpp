#include <mutantur.h>
#include <iostream>
#include <filesystem>

int add(int x, int y)
{
    int z = x + y;
    return z;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        mutantur::mutant mutant("demo.exe");
        mutant.mutate((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add));
        mutant.finalize_mutation("demo_clone.exe");

        auto current_dir = std::filesystem::current_path();
        auto clone = current_dir.string() + "\\demo_clone.exe";
        auto target = current_dir.string() + "\\demo.exe";
        mutantur::utils::merge_clone_file(0, clone.c_str(), target.c_str());
    }

    std::cout << "Function Code:\n";
    std::cout << mutantur::utils::hex_dump((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add)) << "\n";
        
    std::cout << "Proof of Work:\n4 + 10 = " << add(4, 10) << "\n\n";
    system("pause");

    return 0;
}
