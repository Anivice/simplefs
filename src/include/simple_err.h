#ifndef SIMPLEFS_SIMPLE_ERR_H
#define SIMPLEFS_SIMPLE_ERR_H

#include <debug.h>
#include <exception>

/// _ADD_ERROR_INFORMATION_(error_code_name, error_code_value, error_code_text)
#define _ADD_ERROR_INFORMATION_(code, val, info)    \
static const char * code##_ERR_TEXT = info;         \
const unsigned long int code##_CDX = val;

// add error information
_ADD_ERROR_INFORMATION_(SUCCESS, 0, "Success")
_ADD_ERROR_INFORMATION_(CANNOT_OPEN_FILE, 1, "Cannot open file")
_ADD_ERROR_INFORMATION_(SHORT_WRITE, 2, "Short write")
_ADD_ERROR_INFORMATION_(SHORT_READ, 3, "Short read")
_ADD_ERROR_INFORMATION_(OUTOF_BOUNDARY, 4, "Requested parameter out of boundary")
_ADD_ERROR_INFORMATION_(NOT_SIMPLEFS, 5, "Filesystem on device is not simplefs")

#undef _ADD_ERROR_INFORMATION_


class simple_error_t : public std::exception
{
    trace_info_t _trace_info;
    uint64_t error_code;
    std::string what_str;

public:
    const trace_info_t & trace_info = _trace_info;

    explicit simple_error_t(uint64_t _error_code, const std::string& extra = "");
    const char * what();
    [[nodiscard]] uint64_t get_err_code() const { return error_code; }
};

#endif //SIMPLEFS_SIMPLE_ERR_H
