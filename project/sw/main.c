#include <stdlib.h>

#include "uos/uos.h"

int main(int argc, char *argv[]) {
    exit(uos_exit(uos_main(argc, argv)));
    return 0;
}

