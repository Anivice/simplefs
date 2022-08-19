#include <simplefs.h>
#include <iostream>
#include <simple_err.h>
#include <sstream>

void output_head_info(simplefs_head_t head)
{
    try
    {
        std::cout << "Device available block count       " << head.content.device_block_count << std::endl
                  << "Inode Bitmap starting at block     " << head.content.inode_bitmap_blk_start << std::endl
                  << "Inode Bitmap occupied block count  " << head.content.inode_bitmap_blk_count << std::endl
                  << "Zone Bitmap starting at block      " << head.content.zone_bitmap_blk_start << std::endl
                  << "Zone Bitmap occupied block count   " << head.content.zone_bitmap_blk_count << std::endl
                  << "Inode block starting at " << head.content.inode_blk_start << std::endl
                  << "Inode block count       " << head.content.inode_blk_count << std::endl
                  << "Zone block starting at  " << head.content.zone_blk_start << std::endl
                  << "Zone block count        " << head.content.zone_blk_count << std::endl
                  << "Filesystem block efficiency "
                  <<  (double)(1 /* head */ + head.content.inode_bitmap_blk_count + head.content.zone_bitmap_blk_count
                               + head.content.inode_blk_count + head.content.zone_blk_count)
                      / (double)head.content.device_block_count * 100 << "%" << std::endl;

        std::stringstream inode_bitmap_range;
        inode_bitmap_range << head.content.inode_bitmap_blk_start;
        if ((head.content.inode_bitmap_blk_start + head.content.inode_bitmap_blk_count - 1)
            != head.content.inode_bitmap_blk_start)
        {
            inode_bitmap_range << " - "
                               << head.content.inode_bitmap_blk_start + head.content.inode_bitmap_blk_count - 1;
        }

        std::stringstream zone_bitmap_range;
        zone_bitmap_range << head.content.zone_bitmap_blk_start;
        if ((head.content.zone_bitmap_blk_start + head.content.zone_bitmap_blk_count - 1)
            != head.content.zone_bitmap_blk_start)
        {
            zone_bitmap_range << " - "
                              << head.content.zone_bitmap_blk_start + head.content.zone_bitmap_blk_count - 1;
        }

        std::stringstream inode_blk_range;
        inode_blk_range << head.content.inode_blk_start;
        if ((head.content.inode_blk_start + head.content.inode_blk_count - 1)
            != head.content.inode_blk_start)
        {
            inode_blk_range << " - " << head.content.inode_blk_start + head.content.inode_blk_count - 1;
        }

        std::stringstream zone_blk_range;
        zone_blk_range << head.content.zone_blk_start;
        if ((head.content.zone_blk_start + head.content.zone_blk_count - 1)
            != head.content.zone_blk_start)
        {
            zone_blk_range << " - " << head.content.zone_blk_start + head.content.zone_blk_count - 1;
        }

        std::cout << "========================"   << std::endl
                  << "|         HEAD         | 0" << std::endl
                  << "========================"   << std::endl
                  << "|     INODE BITMAP     | "  << inode_bitmap_range.str() << std::endl
                  << "========================"   << std::endl
                  << "|      ZONE BITMAP     | "  << zone_bitmap_range.str() << std::endl
                  << "========================"   << std::endl
                  << "|   INODE BLOCK FILED  | "  << inode_blk_range.str() << std::endl
                  << "========================"   << std::endl
                  << "|    ZONE BLOCK FILED  | "  << zone_blk_range.str() << std::endl
                  << "========================" << std::endl;
    }
    catch (simple_error_t & err)
    {
        std::cerr << err.what() << std::endl;
    }
}
