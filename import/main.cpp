#include <cstdio>
#include "blender_stuff.h"
#include "BLO_readfile.h"

int main(int argc, const char **argv)
{
    bContext *C= CTX_create();
    startup_blender(argc, argv);
    BLO_read_from_file(argv[1], NULL);

    return 0;
}

