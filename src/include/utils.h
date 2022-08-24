#ifndef SIMPLEFS_UTILS_H
#define SIMPLEFS_UTILS_H
#include <simplefs.h>

void output_head_info(simplefs_head_t head);
bool if_header_valid(simplefs_head_t head);
const unsigned char * sha256sum(const unsigned char * data, uint32_t length, unsigned char * buffer);

#endif //SIMPLEFS_UTILS_H
