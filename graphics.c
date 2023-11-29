#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"

int sys_setpixel(int hdc, int x, int y){

    // Check if the coordinates are within the screen boundaries
    if (x < 0 || x >= 320 || y < 0 || y >= 200) {
        return -1; // Return an error code to indicate out-of-bounds
    }
    ushort offset = 320 * y + x;

    //static ushort* crt = (ushort*)P2V(0xA0000); 
    //crt[offset] = 0x0f;
    //////////// the above code doesn't work. /////////////

    // Calculate the memory address for the pixel in video memory
    char* videoMemory = (char*)P2V(0xA0000);
    videoMemory[offset] = 0x0f;

    return 0; // Return 0 to indicate success
}
int sys_moveto(int hdc, int x, int y){
	return 0;
}
int sys_lineto(int hdc, int x, int y){
	return 0;
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
