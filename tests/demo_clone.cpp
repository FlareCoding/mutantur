#include <mutantur.h>
#include <iostream>

int add(int x, int y)
{
    int z = x + y;
    return z;
}

int main()
{
    const size_t fn_length = mutantur::util::calculate_function_size((unsigned char*)&add);

    std::cout << "Function Address:  0x" << std::hex << &add << std::dec << "\n";
    std::cout << "Function Length:   " << fn_length << " bytes\n";

    std::cout << mutantur::util::hex_dump((unsigned char*)&add, fn_length) << "\n";
    std::cout << "4 + 10 = " << add(4, 10) << "\n\n";

    system("pause");
    return 0;
}
