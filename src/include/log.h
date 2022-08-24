#ifndef SIMPLEFS_LOG_H
#define SIMPLEFS_LOG_H

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <chrono>
#include <cstring>

#define WHITE       "\033[01;37m"
#define GREEN       "\033[01;32m"
#define PURPLE      "\033[01;35m"
#define RED         "\033[01;31m"
#define NO_COLOR    "\033[0m"

#define NORMAL  0
#define WARNING 1
#define ERROR   2
#define FATAL   3

struct log_end_t { };
extern log_end_t log_end;

// clear stream, set log level
class log_level
{ public:
    unsigned level;
    explicit log_level(unsigned _level) : level(_level) { }
};

extern class log_t
{
private:
    std::string current_stream;
    struct one_log_line_t
            {
                uint32_t level;
                std::string data;
            };
    std::vector < one_log_line_t > log_data;
    uint32_t current_log_level = 0; // 0: normal verbose 1: warning 2: error 3: fatal

    int pipe = STDOUT_FILENO;
    bool pipe_is_file = false;

public:
    log_t() noexcept = default;

    // public access to log_data
    const std::vector < one_log_line_t > & _log_data = log_data;

    // enable print on receival
    bool show_log_on_receival = true;
    // max log line
    uint32_t log_max_line = 100;
    // enable color on stdout
    bool colored_output = false;

    // open a file instead of redirecting to stdout
    void open_file(const std::string & filename, bool append = false);

    ~log_t() { if (pipe_is_file) { close(pipe); } }

    template < typename Type > log_t & operator << (const Type &);
    log_t & operator << (const log_level & level);
    log_t & operator << (const log_end_t &);

    // show all the logs
    void show_all();
} simple_log;

template<typename Type>
log_t &log_t::operator << (const Type & content)
{
    current_stream += std::string(content);

    return *this;
}

#endif //SIMPLEFS_LOG_H
