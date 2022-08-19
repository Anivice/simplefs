#ifndef SIMPLEFS_IO_H
#define SIMPLEFS_IO_H

#include <sys/types.h>
#include <blkmem.h>
#include <map>
#include <string>

class io_on_dev
{
private:
    int fd = 0;
    off64_t block_count = 0;
    off64_t max_buffer_size = 0;

    std::map < off64_t /* block */, block_t /* data */ > blk_map;

public:
    // open device
    void open(const char * pathname);

    // get current device block count
    [[nodiscard]] off64_t get_block_count() const { return block_count; }

    // request a block
    block_t & request(off64_t blk_num);

    // flush dirty buffer
    void sync();

    ~io_on_dev();
};

#endif //SIMPLEFS_IO_H
