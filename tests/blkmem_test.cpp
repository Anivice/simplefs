#include <io.h>
#include <debug.h>
#include <simple_err.h>
#include <iostream>

int main(int, char ** argv)
{
    try
    {
        initialize_symbol_vector(argv[0]);
        io_on_dev dev;
        exec_cmd("dd if=/dev/urandom of=./disk.img bs=1M count=1 2>/dev/null");
        dev.open("disk.img");
        auto &block = dev.request(8);
        block.modify(0, 7, (unsigned char*)"ABCDEFG");
        block.flush();
        std::cout << block.sha512_to_string() << std::endl;
    }
    catch (simple_error_t & e)
    {
        std::cout << e.what() << std::endl;
    }
}
