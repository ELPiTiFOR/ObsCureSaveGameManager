#include <stdio.h>

#include "business.h"
#include "commands.h"
#include "data_access.h"
#include "path.h"

int main(int argc, char **argv)
{
    setup_paths();
    execute_commands(argc, argv);
    return 0;
}
