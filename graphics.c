#include "types.h"
#include "defs.h"
#include "x86.h"


int sys_setpixel(int hdc, int x, int y){

    // Check if the coordinates are within the screen boundaries
    if (x < 0 || x >= 320 || y < 0 || y >= 200) {
        return -1; // Return an error code to indicate out-of-bounds
    }

    // Calculate the memory address for the pixel in video memory
    char* videoMemory = (char*)0xA0000;
    int offset = 320 * y + x;

    // Set the pixel color at the calculated address
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
    //for (int y = 0; y < 200; y++) {
        //for (int x = 0; x < 320; x++) {
            sys_setpixel(1, 0, 0);
        //}
    //}

	
}
