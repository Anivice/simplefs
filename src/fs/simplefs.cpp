#include <simplefs.h>
#include <utils.h>
#include <simple_err.h>

simplefs_t::simplefs_t(const std::string & pathname)
{
    device.open(pathname.c_str());
    auto head = get_head();
    if (!if_header_valid(head))
    {
        throw simple_error_t(NOT_SIMPLEFS_CDX);
    }

    output_head_info(head);
}

simplefs_head_t simplefs_t::get_head()
{
    simplefs_head_t head;
    auto & block0 = device.request(0);
    block0.copy(0, BLOCK_SIZE, (unsigned char*)&head);
    return head;
}

void simplefs_t::set_head(const simplefs_head_t &head)
{
    sha256sum((unsigned char*)&head.content, sizeof(head.content),
              (unsigned char*)head._hash_check_);
    auto & block0 = device.request(0);
    block0.modify(0, BLOCK_SIZE, (unsigned char*)&head);
}
