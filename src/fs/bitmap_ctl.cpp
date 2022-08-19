#include <simple_err.h>
#include <bitmap_ctl.h>

unsigned int bitmap_ctl_t::get_bit(uint32_t location)
{
    if (location > bitmap_bit_count)
    {
        throw simple_error_t(OUTOF_BOUNDARY_CDX);
    }

    uint32_t bit_offset = location % 8;
    uint32_t byte_offset = location / 8;
    uint32_t block_offset = byte_offset / BLOCK_SIZE;

    auto block = device.request(block_offset);
    unsigned char data;
    unsigned int cmp = 0x01 << bit_offset;
    block.copy(byte_offset, 1, &data);
    auto ret = data & cmp;
    return ret;
}

void bitmap_ctl_t::set_bit(uint32_t bit, uint32_t location)
{
    if (location > bitmap_bit_count)
    {
        throw simple_error_t(OUTOF_BOUNDARY_CDX);
    }

    uint32_t bit_offset = location % 8;
    uint32_t byte_offset = location / 8;
    uint32_t block_offset = byte_offset / BLOCK_SIZE;

    auto &block = device.request(block_offset);
    unsigned char data;
    unsigned int cmp = 0x01 << bit_offset;
    block.copy(byte_offset, 1, &data);

    cmp = ~cmp;
    bit = bit != 0 ? 1 : 0;
    bit <<= bit_offset;

    if (bit)
    {
        data |= bit;
    }
    else
    {
        data &= cmp;
    }

    block.modify(byte_offset, 1, &data);
}
