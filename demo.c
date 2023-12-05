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

    int maxdots = 250;
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

    int squareSize = 100; // Start with a 50x50 square
    int penindex = 20;
    while (squareSize >= 2) {
        int startX = (320 - squareSize) / 2;
        int startY = (200 - squareSize) / 2;
        setpencolour(penindex, 0, 0, 0);
        selectpen(hdc, penindex);
        

        moveto(hdc, startX, startY);
        lineto(hdc, startX + squareSize, startY);
        lineto(hdc, startX + squareSize, startY + squareSize);
        lineto(hdc, startX, startY + squareSize);
        lineto(hdc, startX, startY);

        squareSize -= 2;
        penindex++;
    }


    for(int i = 0; i < 1000; i+=3){
        setpencolour((i%69) + 20, i%63, 0, 0);
        setpencolour((i%69) + 21, 0, i%63, 0);
        setpencolour((i%69) + 22, 0, 0, i%63);
        redraw(hdc);
        sleep(4);
    }


    endpaint(hdc);
    setvideomode(0x12);
    hdc = beginpaint(0);

    for(int i = 0; i < maxdots; i++){
        pen = rand() %16;
        x = rand() % 640;
        y = rand() % 400;

        selectpen(hdc,pen);
        setpixel(hdc,x,y);
        redraw(hdc);
        sleep(1);

    }


    squareSize = 200;
    penindex = 0;
    while (squareSize >= 2) {
        int startX = (640 - squareSize) / 2;
        int startY = (400 - squareSize) / 2;
        selectpen(hdc, 1 +(penindex%13));
        

        moveto(hdc, startX, startY);
        lineto(hdc, startX + squareSize, startY);
        lineto(hdc, startX + squareSize, startY + squareSize);
        lineto(hdc, startX, startY + squareSize);
        lineto(hdc, startX, startY);

        squareSize -= 2;
        penindex++;
        redraw(hdc);
    }


    endpaint(hdc);
    getch();
    setvideomode(0x03);

}