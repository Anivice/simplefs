#ifndef SIMPLEFS_SIMPLEFS_H
#define SIMPLEFS_SIMPLEFS_H

#include <blkmem.h>
#include <cstdint>
#include <openssl/sha.h>

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

void output_head_info(simplefs_head_t head);

#endif //SIMPLEFS_SIMPLEFS_H
