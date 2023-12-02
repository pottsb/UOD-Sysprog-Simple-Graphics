#include "types.h"
#include "user.h"

unsigned long randstate = 1;
unsigned int rand()              {
    randstate = randstate * 1664525 + 1013904223;
    return randstate;
}

int main(int argc, char* argv[])
{

    setvideomode(0x13);
    int hdc = beginpaint(0);

    int maxdots = 3000;
    int pen;
    int x;
    int y;

    for(int i = 0; i < maxdots; i++){
        pen = rand() %16;
        x = rand() % 320;
        y = rand() % 200;

        selectpen(hdc,pen);
        setpixel(hdc,x,y);
        redraw(hdc);
        sleep(1);

    }


    endpaint(hdc);
    getch();
    setvideomode(0x03);

}