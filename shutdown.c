#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {

    if (argc >= 2 && strcmp(argv[1], "-r") == 0) {
        shutdown(1);
    }
    shutdown(0);

}