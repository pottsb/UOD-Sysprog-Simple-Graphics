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

    int squareSize = 100;
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

    for (int i = 0; i < 250; i += 3) {
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

        struct rect rect;

  

    for(int i = 0; i <= 300; i+=20){
        for(int j = 0; j <= 180; j+=19){
                rect.top = j;
                rect.left = i;
                rect.bottom = j + 20;
                rect.right = i + 20;
                pen = rand() %16;
                selectpen(hdc,pen);
                fillrect(hdc,&rect);
                redraw(hdc);
                //printf(1,"t: %d l:%d, b: %d r: %d\n", recta.top,recta.left,recta.bottom,recta.right);
                sleep(50);

        }

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

        squareSize -= 3;
        penindex++;
        redraw(hdc);
    }

        for(int i = 0; i <= 640; i+=40){
        for(int j = 0; j <= 400; j+=40){
                rect.top = j;
                rect.left = i;
                rect.bottom = j + 40;
                rect.right = i + 40;
                pen = rand() %16;
                selectpen(hdc,pen);
                fillrect(hdc,&rect);
                redraw(hdc);
                //printf(1,"t: %d l:%d, b: %d r: %d\n", recta.top,recta.left,recta.bottom,recta.right);
                sleep(1);

        }

    }


    endpaint(hdc);
    setvideomode(0x03);

}