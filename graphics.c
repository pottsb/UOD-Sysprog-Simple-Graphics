#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"

#define MAX_HDC 32
#define VGA_MEMORY_ADDRESS 0xA0000
#define VGA_MEMORY_SIZE 0x10000

struct point {
    int x;
    int y;
};
struct hdc {
    struct point mypoint;
    int pen;
    bool locked;
};
struct hdc hdcarray[MAX_HDC];

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

    // Check if the coordinates are within the screen boundaries
    if (x < 0 || x >= 320 || y < 0 || y >= 200) {
        return -1; // Return an error code to indicate out-of-bounds
    }
    ushort offset = 320 * y + x;
    char* videoMemory = (char*)P2V(0xA0000);
    videoMemory[offset] = hdcarray[hdcIndex].pen;

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

    // Check if the x-coordinate is out of bounds
    if (x < 0) {
        x = 0;
    } else if (x > 320) {
        x = 320;
    }

    // Check if the y-coordinate is out of bounds
    if (y < 0) {
        y = 0;
    } else if (y > 200) {
        y = 200;
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

    // Check if the coordinates are within the screen boundaries
    if (x2 < 0 || x2 >= 320 || y2 < 0 || y2 >= 200) {
        return -1; // Return an error code to indicate out-of-bounds
    }

    int y1 = hdcarray[hdcIndex].mypoint.y;
    int x1 = hdcarray[hdcIndex].mypoint.x;


    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        ushort offset = 320 * y1 + x1;
        char* videoMemory = (char*)P2V(0xA0000);
        videoMemory[offset] = hdcarray[hdcIndex].pen;

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
	// You need to put code to clear the video buffer here.  Initially, 
	// you might just set each pixel to black in a nested loop, but think
	// about faster ways to do it. 
	//
	// This function is called from videosetmode.

    // Loop through all pixels and set them to black

    char* videoMemory = (char*)P2V(0xA0000);

    for(uint i = 0; i < 64320; i++){    
        videoMemory[i] = 0x0;
    }
}


int sys_setpencolour(void){
    int index;
    int r;
    int g;
    int b;

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

    // Check if the coordinates are within the screen boundaries
    if (index < 0 || index > 255) {
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

    char* videoMemory = (char*)P2V(0xA0000);
    ushort offset;
    for (int y = rect->top; y <= rect->bottom; y++) {
            for (int x = rect->left; x <= rect->right; x++) {
                offset = 320 * y + x;
                //cprintf("X:%d Y:%d\n", x, y);
                videoMemory[offset] = hdcarray[hdcIndex].pen;
            }
        }

    return 1;
}



int sys_beginpaint(void){
    int hwnd;

    if (argint(0, &hwnd) < 0) {
        return -1;
    }

    int i;
    for (i = 0; i < MAX_HDC; i++) {

    if(hdcarray[i].locked == false)

    hdcarray[i].locked = true;
    hdcarray[i].mypoint.x = 0;
    hdcarray[i].mypoint.y = 0;
    hdcarray[i].pen = 15;

    return i;

    }

    cprintf("ERROR: No free HDC!");
    return -1;
}
int sys_endpaint(void){
    int hdc;

    if (argint(0, &hdc) < 0) {
        return -1;
    }

    hdcarray[hdc].locked = false;


    return 1;
}