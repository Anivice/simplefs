#include <bitmap_ctl.h>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <debug.h>

std::vector < uint64_t > generate_unique_num(uint64_t start, uint64_t end, uint64_t count)
{
    std::vector < uint64_t > numbers;
    std::vector < uint64_t > ret;
    for (uint64_t i = start; i < end; i++) {
        numbers.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));

    for (int i = 0; i < count; i++)
    {
        ret.push_back(numbers[i]);
    }

    return ret;
}

uint64_t generate_random_num (uint64_t start, uint64_t end)
{
    std::random_device dev;
    std::string ret;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist255(start, end);
    return dist255(rng);
};

int main(int argc, char ** argv)
{
    initialize_symbol_vector(argv[0]);

    exec_cmd("dd if=/dev/zero of=disk.img bs=1024 count=1440");
    io_on_dev device;
    device.open("disk.img");
    bitmap_ctl_t bitmap(0, 4096 * 8, device);

    uint64_t init_count = generate_random_num(1024, 4096);
    uint64_t ops_count = 0;
    auto ops_map =
            generate_unique_num(0,
                                4096 * 8,
                                init_count);

    // randomly set bits in bitmap
    for (auto &i: ops_map)
//    for (uint64_t i = 0; i < 32; i++)
    {
        bitmap.set_bit(1, i);
    }

    // count how many bits are set in bitmap
    for (uint64_t i = 0; i < 4096 * 8; i++)
    {
        if (bitmap.get_bit(i))
        {
            ops_count++;
        }
    }

    if (ops_count != init_count)
    {
        std::cout << "Test failed" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
