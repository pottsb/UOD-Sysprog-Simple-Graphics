#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"


struct point {
    int x;
    int y;
};
struct point mypoint= { .x = 1, .y = 0 };


int sys_setpixel(void){

    int hdc;
    int x;
    int y;

    if (argint(0, &hdc) < 0) {
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
    videoMemory[offset] = 0x0f;

    return 0; // Return 0 to indicate success
}
int sys_moveto(void){

    int hdc;
    int x;
    int y;

    if (argint(0, &hdc) < 0) {
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


	mypoint.x = x;
    mypoint.y = y;

    return 1;
}

int abs(int n) {
    return (n < 0) ? -n : n;
}

int sys_lineto(void){
	int hdc;
    int x2;
    int y2;

    if (argint(0, &hdc) < 0) {
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

    int y1 = mypoint.y;
    int x1 = mypoint.x;


    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        ushort offset = 320 * y1 + x1;
        char* videoMemory = (char*)P2V(0xA0000);
        videoMemory[offset] = 0x0f;

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

    mypoint.x = x2;
    mypoint.y = y2;

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
