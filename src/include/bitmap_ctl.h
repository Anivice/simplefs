#ifndef SIMPLEFS_BITMAP_CTL_H
#define SIMPLEFS_BITMAP_CTL_H

#include <blkmem.h>
#include <io.h>

class bitmap_ctl_t
{
    uint32_t bitmap_starting_blk;
    uint32_t bitmap_bit_count;
    io_on_dev & device;

public:

    bitmap_ctl_t(uint32_t _bitmap_starting_blk, uint32_t _bitmap_bit_count, io_on_dev & _device)
        : bitmap_starting_blk(_bitmap_starting_blk),
          bitmap_bit_count(_bitmap_bit_count),
          device(_device) { }

    unsigned int get_bit(uint32_t location);
    void set_bit(uint32_t bit, uint32_t location);
};

#endif //SIMPLEFS_BITMAP_CTL_H
