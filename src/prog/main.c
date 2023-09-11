#include "prog.h"

int main(int argc, char const* argv[])
{
    unused(argc);
    unused(argv);
    printf("%sCompilation%s de %s \n", COLOR_DEBUG_CATEGORY, COLOR_RESET, current_time);
    verification();


    memory_printf_final();
    return EXIT_SUCCESS;
}