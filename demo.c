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

ushort c1 = rand() %69;
ushort c2 = rand() %69;
ushort c3 = rand() %69;

for (int i = 0; i < 500; i += 3) {
    setpencolour((i % 69) + 20, c1, c2, c3);
    setpencolour((i % 69) + 21, c1, c2, c3);
    setpencolour((i % 69) + 22, c1, c2, c3);
    
    ushort ct = c1;
    c1 = c2;
    c2 = c3;
    c3 = ct;
    
    redraw(hdc);
    sleep(4);
}

    struct rect recta;

  

    for(int i = 0; i <= 300; i+=21){
        for(int j = 0; j <= 180; j+=21){
                recta.top = j;
                recta.left = i;
                recta.bottom = j + 20;
                recta.right = i + 20;
                pen = rand() %16;
                selectpen(hdc,pen);
                fillrect(hdc,&recta);
                redraw(hdc);
                sleep(50);
        }

    }
    getch();

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