#include <blkmem.h>
#include <cstring>
#include <unistd.h>
#include <simple_err.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

const unsigned char * sha256sum(const unsigned char * data, uint32_t length, unsigned char * buffer)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, length);
    SHA256_Final(buffer, &sha256);
    return buffer;
}

uint64_t block_t::modify(uint64_t start, uint64_t length, const unsigned char *buffer)
{
    uint64_t modify_len;
    if (start > BLOCK_SIZE)
    {
        return 0;
    }
    else if ((start + length) > BLOCK_SIZE)
    {
        modify_len = BLOCK_SIZE - start;
    }
    else
    {
        modify_len = length;
    }

    memcpy(data + start, buffer, modify_len);
    dirty = 1;
    return modify_len;
}

uint64_t block_t::copy(uint64_t start, uint64_t length, unsigned char *buffer)
{
    uint64_t copy_len;
    if (start > BLOCK_SIZE)
    {
        return 0;
    }
    else if ((start + length) > BLOCK_SIZE)
    {
        copy_len = BLOCK_SIZE - start;
    }
    else
    {
        copy_len = length;
    }

    memcpy(buffer, data + start, copy_len);
    return copy_len;
}

void block_t::flush()
{
    if (!dirty)
    {
        return;
    }

    lseek(fd, block_num * BLOCK_SIZE, SEEK_SET);
    auto len = write(fd, data, BLOCK_SIZE);
    if (len != BLOCK_SIZE)
    {
        throw simple_error_t(SHORT_WRITE_CDX,
                             std::string("Short write on block ") + std::to_string(block_num));
    }

    dirty = 0;
}

void block_t::refresh()
{
    lseek(fd, block_num * BLOCK_SIZE, SEEK_SET);
    if (read(fd, data, BLOCK_SIZE) != BLOCK_SIZE)
    {
        throw simple_error_t(SHORT_READ_CDX,
                             std::string("Short read on block ") + std::to_string(block_num));
    }

    dirty = 0;
}

std::string block_t::sha512_to_string()
{
    get_sha256();
    std::stringstream output;
    for(unsigned char i : hash)
    {
        output << std::hex << std::setw(2) << std::setfill('0') << (int)i;
    }

    return output.str();
}

const unsigned char *block_t::get_sha256() const
{
    return sha256sum(data, BLOCK_SIZE, (unsigned char*)hash);
}
