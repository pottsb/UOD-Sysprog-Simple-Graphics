#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "Hello World from user space!\n");
    greeting();
    exit();
}