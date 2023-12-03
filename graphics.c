#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"

#define MAX_HDC 4



struct point {
    int x;
    int y;
};
struct hdc {
    struct point mypoint;
    struct point screen;
    short pen;
    bool locked;
    char  videobuffer[4][320 * 200];
};

struct hdc hdcarray[MAX_HDC];
static ushort *plane1 = (ushort*)P2V(0xA0000);

void setpixelinbuffer(int hdcIndex, int x, int y) {
    int currentvideomode = getcurrentvideomode();
    ushort offset = hdcarray[hdcIndex].screen.x * y + x;
    if (currentvideomode == 0x13) {
        hdcarray[hdcIndex].videobuffer[0][offset] = hdcarray[hdcIndex].pen;
    } else if (currentvideomode == 0x12) {
        ushort byteOffset = (hdcarray[hdcIndex].screen.x * y + x) / 8;
        ushort bitPosition = x % 8;

        for (short i = 0; i < 4; i++) {
            short color_bit = (hdcarray[hdcIndex].pen >> i) & 1; // Isolate the bit for the current plane
            if (color_bit) {
                hdcarray[hdcIndex].videobuffer[i][byteOffset] |= (1 << (7 - bitPosition)); // Set the bit at the correct position
            } else {
                hdcarray[hdcIndex].videobuffer[i][byteOffset] &= ~(1 << (7 - bitPosition)); // Clear the bit
            }
        }
    }
}

int sys_setpixel(void){
    int hdcIndex;
    int x;
    int y;

    if (argint(0, &hdcIndex) < 0) {
        return -1;
    }

    if (argint(1, &x) < 0) {
        return -1;
    }

    if (argint(2, &y) < 0) {
        return -1;
    }

    // Check the pixels are within the bounds of display mode
    if (x < 0) {
        x = 0;
    } else if (x > hdcarray[hdcIndex].screen.x) {
        x = hdcarray[hdcIndex].screen.x;
    }
    if (y < 0) {
        y = 0;
    } else if (y > hdcarray[hdcIndex].screen.y) {
        y = hdcarray[hdcIndex].screen.y;
    }
    
    setpixelinbuffer(hdcIndex,x,y);
    return 0; // Return 0 to indicate success
}
int sys_moveto(void){
    int hdcIndex;
    int x;
    int y;

    if (argint(0, &hdcIndex) < 0) {
        return -1;
    }

    if (argint(1, &x) < 0) {
        return -1;
    }

    if (argint(2, &y) < 0) {
        return -1;
    }

    // Check the pixels are within the bounds of display mode
    if (x < 0) {
        x = 0;
    } else if (x > hdcarray[hdcIndex].screen.x) {
        x = hdcarray[hdcIndex].screen.x;
    }
    if (y < 0) {
        y = 0;
    } else if (y > hdcarray[hdcIndex].screen.y) {
        y = hdcarray[hdcIndex].screen.y;
    }


    hdcarray[hdcIndex].mypoint.x = x;
    hdcarray[hdcIndex].mypoint.y = y;

    return 1;
}

int abs(int n) {
    return (n < 0) ? -n : n;
}

int sys_lineto(void){
	int hdcIndex;
    int x2;
    int y2;

    if (argint(0, &hdcIndex) < 0) {
        return -1;
    }

    if (argint(1, &x2) < 0) {
        return -1;
    }

    if (argint(2, &y2) < 0) {
        return -1;
    }

    // Check the pixels are within the bounds of display mode
    if (x2 < 0) {
        x2 = 0;
    } else if (x2 > hdcarray[hdcIndex].screen.x) {
        x2 = hdcarray[hdcIndex].screen.x;
    }
    if (y2 < 0) {
        y2 = 0;
    } else if (y2 > hdcarray[hdcIndex].screen.y) {
        y2 = hdcarray[hdcIndex].screen.y;
    }

    int y1 = hdcarray[hdcIndex].mypoint.y;
    int x1 = hdcarray[hdcIndex].mypoint.x;


    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setpixelinbuffer(hdcIndex,x1,y1);

        if (x1 == x2 && y1 == y2) {
            break;
        }
        if (err > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err < dx) {
            err += dx;
            y1 += sy;
        }
    }

    hdcarray[hdcIndex].mypoint.x = x2;
    hdcarray[hdcIndex].mypoint.y = y2;

    return 1;
}

void clear320x200x256() {

    char* videoMemory = (char*)P2V(0xA0000);

    for(uint i = 0; i < 64320; i++){    
        videoMemory[i] = 0x0;
    }
}

void clear640x400x16(){

    for(uint i = 0; i<=4; i++){
        setplane(i);
        char* videoMemory = (char*)getframebufferbase();

        for(uint j = 0; j < 64320; j++){    // char is 4 bits 4 * 64320 = all the pixels
            videoMemory[j] = 0x0;
        }
    }
}

int sys_setpencolour(void){
    int index;
    int r;
    int g;
    int b;

    if(getcurrentvideomode() == 0x12){
        cprintf("ERROR: Unsupported video mode!\n");
        return -1;
    }

    if (argint(0, &index) < 0) {
        return -1;
    }

    if (argint(1, &r) < 0) {
        return -1;
    }

    if (argint(2, &g) < 0) {
        return -1;
    }

    if (argint(3, &b) < 0) {
        return -1;
    }


    outb(0x3C8, index);
    outb(0x3C9,r);
    outb(0x3C9,g);
    outb(0x3C9,b);
    return 1;
}

int sys_selectpen(void){
    int hdcIndex;
    int index;

    if (argint(0, &hdcIndex) < 0) {
        return -1;
    }

    if (argint(1, &index) < 0) {
        return -1;
    }

    int maxindex = 0;
    int currentvideomode = getcurrentvideomode();
    if(currentvideomode == 0x13){
        maxindex = 255;
    }else if(currentvideomode == 0x12){
        maxindex = 15;
    }

    if (index < 0 || index > maxindex) {
        cprintf("ERROR: Pen index out of range!\n");
        return -1; // Return an error code to indicate out-of-bounds
    }

    hdcarray[hdcIndex].pen = index;
    return 1;
}


int sys_fillrect(void){
    int hdcIndex;
    struct rect *rect;

    if (argint(0, &hdcIndex) < 0) {
        return -1;
    }

    if (argptr(1, (char**)&rect,16) < 0) {
        return -1;
    }

    for (int y = rect->top; y <= rect->bottom; y++) {
            for (int x = rect->left; x <= rect->right; x++) {
                setpixelinbuffer(hdcIndex,x,y);
            }
        }

    return 1;
}



int sys_beginpaint(void){
    int hwnd;

    if (argint(0, &hwnd) < 0) {
        return -1;
    }

    // loop through all hdcs checking for a free one
    int i;
    for (i = 0; i < MAX_HDC; i++) { 
        if(hdcarray[i].locked == false){
            // lock the hdc and reset vars
            hdcarray[i].locked = true;
            hdcarray[i].mypoint.x = 0;
            hdcarray[i].mypoint.y = 0;
            hdcarray[i].pen = 15;

            // store the screen res based on video mode when begin paint is called.
            // this is used for bounds checking and pixel position calculations.
            int currentvideomode = getcurrentvideomode();
            if(currentvideomode == 0x13){
                hdcarray[i].screen.x = 320;
                hdcarray[i].screen.y = 200;
                //hdcarray[i].videobuffer = (char *) malloc(320 * 320 * sizeof(char));
                memmove(hdcarray[i].videobuffer[0], plane1, sizeof(char) * 320 * 200);
            }else if(currentvideomode == 0x12){
                hdcarray[i].screen.x = 640;
                hdcarray[i].screen.y = 400;
                for (short j = 0; j < 4; j++){
                    setplane(j);
                    uchar* mem = getframebufferbase();
                    memmove(hdcarray[i].videobuffer[j], mem, sizeof(char) * 320 * 200);
                }
                cprintf("start paint\n");
            }else{
                hdcarray[i].locked = false;
                cprintf("ERROR: Unsupported video mode!\n");
                return -1;
            }
            return i;
        }
    }

    cprintf("ERROR: No free HDC!\n");
    return -1;
}

int sys_endpaint(void){
    int hdc;

    if (argint(0, &hdc) < 0) {
        return -1;
    }
    
    int currentvideomode = getcurrentvideomode();
    if(currentvideomode == 0x13){
        memmove(plane1, hdcarray[hdc].videobuffer[0], sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        for (short i = 0; i < 4; i++){
            setplane(i);
            uchar* mem = getframebufferbase();;
            memmove(mem, hdcarray[hdc].videobuffer[i], sizeof(char) * 320 * 200);
        }
    }

    //free(hdcarray[hdc].videobuffer);
    hdcarray[hdc].locked = false;
    return 1;
}

int sys_redraw(void){
    int hdc;

    if (argint(0, &hdc) < 0) {
        return -1;
    }

    int currentvideomode = getcurrentvideomode();
    if(currentvideomode == 0x13){
        memmove(plane1, hdcarray[hdc].videobuffer[0], sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        for (short i = 0; i < 4; i++){
            setplane(i);
            uchar* mem = getframebufferbase();;
            memmove(mem, hdcarray[hdc].videobuffer[i], sizeof(char) * 320 * 200);
        }
    }

    return 1;
}