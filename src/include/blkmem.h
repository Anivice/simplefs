#ifndef SIMPLEFS_BLKMEM_H
#define SIMPLEFS_BLKMEM_H

#include <cstdint>
#include <vector>
#include <sys/types.h>
#include <string>
#include <openssl/sha.h>

#define BLOCK_SIZE 4096

class block_t
{
private:
    unsigned char data [BLOCK_SIZE] { };
    off64_t block_num = 0;
    int fd = 0;
    uint64_t dirty:1;
    unsigned char hash [SHA256_DIGEST_LENGTH] { };

public:
    // assign block info
    explicit block_t(int _fd, off64_t _block_num)
        {
            fd = _fd;
            block_num = _block_num;
            dirty = 0;
            refresh();
        }

    uint64_t modify(uint64_t start, uint64_t length, const unsigned char * buffer);
    uint64_t copy(uint64_t start, uint64_t length, unsigned char * buffer);
    std::string sha512_to_string();
    [[nodiscard]] const unsigned char * get_sha256() const;

    void flush();
    void refresh();

    ~block_t() { flush(); }

    [[nodiscard]] bool if_dirty() const { return dirty; }
};

const unsigned char * sha256sum(const unsigned char * data, uint32_t length, unsigned char * buffer);

#endif //SIMPLEFS_BLKMEM_H
