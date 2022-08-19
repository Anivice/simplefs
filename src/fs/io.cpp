#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <simple_err.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>

void io_on_dev::open(const char *pathname)
{
    fd = ::open(pathname, O_RDWR | O_SYNC);
    if (fd == -1)
    {
        throw simple_error_t(CANNOT_OPEN_FILE_CDX, "Failed to open " + std::string(pathname));
    }

    block_count = lseek(fd, 0, SEEK_END) / BLOCK_SIZE;
    max_buffer_size = block_count / 10;
    if (max_buffer_size == 0) {
        max_buffer_size = 1;
    }
}

io_on_dev::~io_on_dev()
{
    sync();
    close(fd);
}

block_t &io_on_dev::request(off64_t blk_num)
{
    if (blk_num > block_count)
    {
        throw simple_error_t(OUTOF_BOUNDARY_CDX);
    }

    auto it = blk_map.find(blk_num);
    // cannot find block number
    if (it == blk_map.end())
    {
        // assign new block
        if (blk_map.size() == max_buffer_size)
        {
            sync();
        }

        blk_map.emplace(blk_num, block_t(fd, blk_num));
        return blk_map.at(blk_num);
    }
    else
    {
        return it->second;
    }
}

void io_on_dev::sync()
{
    for (auto & i : blk_map)
    {
        i.second.flush();
    }

    blk_map.clear();
}
