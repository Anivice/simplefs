#include <simplefs.h>
#include <io.h>
#include <simple_err.h>
#include <iostream>
#include <openssl/sha.h>
#include <blkmem.h>
#include <cstring>

bool if_header_valid(simplefs_head_t head)
{
    if (head.content.magic != SIMPLEFS_MAGIC)
    {
        return false;
    }

    unsigned char hash [SHA256_DIGEST_LENGTH] { };
    sha256sum((unsigned char*)&head.content, sizeof(head.content), hash);

    if (!!memcmp(hash, head._hash_check_, SHA256_DIGEST_LENGTH))
    {
        return false;
    }

    return true;
}

int main(int argc, char ** argv)
{
    try
    {
        simplefs_head_t head;
        io_on_dev device;
        device.open(argv[1]);
        auto & block0 = device.request(0);
        block0.copy(0, BLOCK_SIZE, (unsigned char*)&head);

        if (!if_header_valid(head))
        {
            std::cerr << "Filesystem sanity check failed!" << std::endl;
            return EXIT_FAILURE;
        }

        output_head_info(head);
    }
    catch (simple_error_t & err)
    {
        std::cerr << err.what() << std::endl;
    }
}
