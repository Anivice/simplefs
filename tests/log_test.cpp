#include <log.h>

int main()
{
    simple_log.colored_output = true;
    simple_log << log_level(NORMAL) << "Hello, world!" << log_end;
    simple_log << log_level(ERROR) << "Error!" << log_end;
    simple_log.open_file("log.log", false);
    simple_log << log_level(1) << "Hello, world2!" << log_end;
    simple_log << log_level(3) << "Hello, world6!" << log_end;

    simple_log.show_all();
    return EXIT_SUCCESS;
}
