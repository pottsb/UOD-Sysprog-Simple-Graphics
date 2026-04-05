#include "types.h"
#include "user.h"



int main(int argc, char* argv[])
{
    setvideomode(0x13);

    setpencolour(19,0,0,60);
    printf(1,"1\n");
    int hdc = beginpaint(0);
    moveto(0, 50, 50);    
    lineto(0, 200, 150);
    
    endpaint(hdc);
    printf(1,"6\n");
    getch();
    setvideomode(0x12);
    printf(1,"7\n");

    hdc = beginpaint(0);
    printf(1,"8\n");
    selectpen(hdc,12);
        moveto(0, 50, 50);    
    lineto(0, 200, 150);

    endpaint(hdc);
    getch();
    setvideomode(0x03);
    exit();
}