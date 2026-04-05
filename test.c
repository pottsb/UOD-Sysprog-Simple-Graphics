#include "types.h"
#include "user.h"



int main(int argc, char* argv[])
{
    setvideomode(0x13);

    setpencolour(19,0,0,60);
    printf(1,"1\n");
    int hdc = beginpaint(0);
    printf(1,"2\n");
    struct rect recta = {.top = 90, .left = 10, .bottom = 190, .right = 110 };
    selectpen(hdc,5);
    fillrect(hdc,&recta);
    selectpen(hdc,19);
    moveto(hdc, 100, 50);
    lineto(hdc, 200, 50);
    lineto(hdc, 200, 150);
    lineto(hdc, 100, 150);
    lineto(hdc, 100, 50);

    printf(1,"3\n");
    selectpen(hdc,1);
    setpixel(hdc,10,10);
    setpixel(hdc,11,10);
    setpixel(hdc,10,11);
    setpixel(hdc,11,11);
    printf(1,"4\n");

    for (int i = 0; i < 20; i++)
    {
    moveto(hdc, i * 10, i * 5);
    setpencolour(i + 20, i * 3, i * 2, i);
    selectpen(hdc, i + 20);
    lineto(hdc, i * 10 + 20, i * 5);
    lineto(hdc, i * 10 + 20, i * 5 + 20);
    lineto(hdc, i * 10, i * 5 + 20);
    lineto(hdc, i * 10, i * 5);
    }
    printf(1,"5\n");
    
    endpaint(hdc);
    printf(1,"6\n");
    getch();
    setvideomode(0x12);
    printf(1,"7\n");

    hdc = beginpaint(0);
    printf(1,"8\n");
    selectpen(hdc,12);
    struct rect rectb = {.top = 190, .left = 10, .bottom = 400, .right = 600 };
    fillrect(hdc,&rectb);

    selectpen(hdc,1);
    moveto(hdc, 100, 50);
    lineto(hdc, 200, 50);
    lineto(hdc, 200, 150);
    lineto(hdc, 100, 150);
    lineto(hdc, 100, 50);

    selectpen(hdc,12);
    setpixel(hdc,10,10);
    setpixel(hdc,11,10);
    setpixel(hdc,10,11);
    setpixel(hdc,11,11);
    
    for (int i = 0; i < 40; i++)
    {
    moveto(hdc, i * 10, i * 5);
    selectpen(hdc, 10);
    lineto(hdc, i * 10 + 20, i * 5);
    lineto(hdc, i * 10 + 20, i * 5 + 20);
    lineto(hdc, i * 10, i * 5 + 20);
    lineto(hdc, i * 10, i * 5);
    }

    endpaint(hdc);
    getch();
    setvideomode(0x03);
    exit();
}