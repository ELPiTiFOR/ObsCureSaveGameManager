#include <stdio.h>

#include "business.h"
#include "commands.h"
#include "data_access.h"

int main(int argc, char **argv)
{
    execute_commands(argc, argv);
    return 0;
}
