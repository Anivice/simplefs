#ifndef SIMPLEFS_SIMPLEFS_H
#define SIMPLEFS_SIMPLEFS_H

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <blkmem.h>
#include <cstdint>
#include <openssl/sha.h>
#include <io.h>
#include <string>

#define SIMPLEFS_MAGIC 0xAABBAABB

/*
 * ==================================
 *         first block: head
 * ==================================
 *         inode bitmap
 * ==================================
 *         zone bitmap
 * ==================================
 *         inode block
 * ==================================
 *         zone block
 * ==================================
 * */

struct simplefs_head_t
{
    struct {
        uint32_t magic;
        uint32_t device_block_count;
        uint32_t inode_bitmap_blk_start;
        uint32_t inode_bitmap_blk_count;
        uint32_t zone_bitmap_blk_start;
        uint32_t zone_bitmap_blk_count;
        uint32_t inode_blk_start;
        uint32_t inode_blk_count;
        uint32_t zone_blk_start;
        uint32_t zone_blk_count;
    } content { };

    unsigned char _hash_check_ [SHA256_DIGEST_LENGTH] { }; // sha256sum of `content`
    char _fill_ [ BLOCK_SIZE - sizeof(content) - SHA256_DIGEST_LENGTH] { }; // empty filed
};
static_assert(sizeof(simplefs_head_t) == BLOCK_SIZE, "Incorrect head size");


struct simplefs_inode_t
{
    struct {
        char name [ 128 ];
        uint8_t zone_map_used_index_count;
        uint32_t zone_map [
                2 + /* 2nd level block */
                2   /* 3rd level block */
                ]; /* 2 * 4096 / 4 * 4096 +
                    * 2 * 4096 / 4 * 4096 / 4 * 4096
                    * = 8598323200 ~= 8Gb */
        unsigned char _hash_check_ [SHA256_DIGEST_LENGTH] [4]; // sha256sum of each block
        struct stat inode_stat;
    }
    content __attribute__ ((packed)) { };

    unsigned char _hash_check_ [SHA256_DIGEST_LENGTH] { }; // sha256sum of `content`
    char _fill_ [ BLOCK_SIZE - sizeof(content) - SHA256_DIGEST_LENGTH] { }; // empty filed
};
static_assert(sizeof(simplefs_inode_t) == BLOCK_SIZE, "Incorrect head size");

typedef unsigned char simplefs_zone_t [ BLOCK_SIZE ];

class simplefs_t
{
private:
    io_on_dev device;

    simplefs_head_t get_head();

    // save head on block 0 (automatically refresh hash check)
    void set_head(const simplefs_head_t & head);

public:
    explicit simplefs_t(const std::string & pathname);
};

#endif //SIMPLEFS_SIMPLEFS_H
