#include <mutantur.h>
#include <iostream>

int add(int x, int y)
{
    int z = x + y;
    return z;
}

int main()
{
    std::cout << "Original Function Code:\n";
    std::cout << mutantur::util::hex_dump((unsigned char*)&add, mutantur::util::calculate_function_size((unsigned char*)&add)) << "\n\n";

    std::cout << "Launch Clone Instance BEFORE Modification ...\n";
    system("demo_clone.exe");

    std::cout << "\n[*] Launching a mutant [*]\n\n";
    
    mutantur::mutant mutant("demo_clone.exe");
    mutant.mutate((unsigned char*)&add, mutantur::util::calculate_function_size((unsigned char*)&add));
    mutant.finalize_mutation("demo_clone.exe");

    std::cout << "Launch Clone Instance AFTER Modification ...\n";
    system("demo_clone.exe");

    return 0;
}
