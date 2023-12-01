#include "types.h"
#include "user.h"

int main(int argc, char* argv[])
{
    setvideomode(0x13);

    setpixel(1,10,10);
    setpixel(1,11,10);
    setpixel(1,10,11);
    setpixel(1,11,11);

    moveto(0, 100, 50);
    lineto(0, 200, 50);
    lineto(0, 200, 150);
    lineto(0, 100, 150);
    lineto(0, 100, 50);


    getch();
    setvideomode(0x03);
    exit();
}