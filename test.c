#include "types.h"
#include "user.h"



int main(int argc, char* argv[])
{
    setvideomode(0x13);

    setpencolour(20,0,0,60);
    selectpen(1,20);




    moveto(0, 100, 50);
    lineto(0, 200, 50);
    lineto(0, 200, 150);
    lineto(0, 100, 150);
    lineto(0, 100, 50);


    struct rect recta = {.top = 10, .left = 10, .bottom = 100, .right = 100 };

    selectpen(1,5);
    fillrect(1,&recta);
    selectpen(1,1);
    setpixel(1,10,10);
    setpixel(1,11,10);
    setpixel(1,10,11);
    setpixel(1,11,11);


    getch();
    setvideomode(0x03);
    exit();
}