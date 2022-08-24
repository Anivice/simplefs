#include <log.h>
log_end_t log_end;
log_t simple_log;

void log_t::open_file(const std::string &filename, bool append)
{
    int mode;
    if (append)
    {
        mode = O_APPEND;
    }
    else
    {
        mode = O_TRUNC;
    }

    pipe = open(filename.c_str(), O_CREAT | O_WRONLY | mode, S_IFREG | 0600);
    if (pipe == -1)
    {
        std::cerr << "Cannot open file for log device!" << std::endl;
        exit(EXIT_FAILURE);
    }

    pipe_is_file = true;
}

log_t &log_t::operator<<(const log_level &level)
{
    current_log_level = level.level;
    current_stream.clear();
    return *this;
}

log_t &log_t::operator<<(const log_end_t &)
{
    std::stringstream output;
    // prepare content
    auto time = std::chrono::system_clock::now();
    auto ctime = std::chrono::system_clock::to_time_t(time);
    auto my_time = std::ctime(&ctime);

    // delete '\n'
    auto str_len = strlen(my_time);
    my_time[str_len - 1] = 0;
    output << "[" << my_time << ", <"
           << current_log_level << ">] ";

    if (!pipe_is_file && colored_output)
    {

        switch (current_log_level)
        {
            case NORMAL:
                output << WHITE;
                break;
            case WARNING:
                output << GREEN;
                break;
            case ERROR:
                output << PURPLE;
                break;
            case FATAL:
                output << RED;
                break;
            default:
                output << WHITE;
        }

        output << current_stream << NO_COLOR << std::endl;
    }
    else
    {
        output << current_stream << std::endl;
    }
    std::string str = output.str();

    if (show_log_on_receival)
    {
        if (write(pipe, str.c_str(), str.size()) != str.size())
        {
            std::cerr << "[" << std::ctime(&ctime) << ", 3] "
                      << "Cannot write to log pipe!" << std::endl;
        }
    }

    // remove head of log_data if reaching max line
    if (log_data.size() == log_max_line)
    {
        log_data.erase(log_data.begin());
    }

    log_data.emplace_back(
            one_log_line_t { .level = current_log_level, .data = str}
    );
    current_stream.clear();

    return *this;
}

void log_t::show_all()
{
    for (const auto & i : log_data)
    {
        std::cout << i.data << std::flush;
    }
}
