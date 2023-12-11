#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "spinlock.h"

static ushort *cgaframebuffer = (ushort*)P2V(0xA0000);
struct hdc hdc;

struct spinlock gfx;

// called from main.c
void graphicsinit(){
 initlock(&gfx, "graphics");
};

// clear video memory and buffer
// called when changing video modes
void clear320x200x256(void) {
    char* videoMemory = (char*)P2V(0xA0000);

    for(ushort i = 0; i < 320 * 200; i++){    
        videoMemory[i] = 0x0;
        hdc.videobuffer[0][i] = 0x0;
    }
}

// clear video memory and buffer
// called when changing video modes
void clear640x400x16(void){
    for(ushort i = 0; i<=4; i++){
        setplane(i);
        char* videoMemory = (char*)getframebufferbase();

        for(ushort j = 0; j < 320 * 200; j++){ 
            videoMemory[j] = 0x0;
        }
    }
    for(ushort i = 0; i<4; i++){
        for(ushort j = 0; j < 320 * 200; j++){  
            hdc.videobuffer[i][j] = 0x0;
        }
    }
}

// called from beginpain()
int sys_getHDC(void){
    acquire(&gfx);
    //acquireconslock();
    struct hdc (*userhdcpointer);
    if (argptr(0, (void*)&userhdcpointer,sizeof(struct hdc)) < 0) {
        return -1;
    }
        
    // reset hdc vars
    hdc.lastpoint.x = 0;
    hdc.lastpoint.y = 0;
    hdc.pen = 15;

    // store the screen res based on video mode when begin paint is called.
    // this is used for bounds checking and pixel position calculations.
    // copy the current contents of the screen into the buffer
    ushort currentvideomode = getcurrentvideomode();
    hdc.videomode = currentvideomode;
    if(currentvideomode == 0x13){
        hdc.screen.x = 320;
        hdc.screen.y = 200;
        memmove(hdc.videobuffer[0], cgaframebuffer, sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        hdc.screen.x = 640;
        hdc.screen.y = 400;
        for (short j = 0; j < 4; j++){
            setplane(j);
            uchar* framebuffer = getframebufferbase();
            memmove(hdc.videobuffer[j], framebuffer, sizeof(char) * 320 * 200);
        }
    }else{
        cprintf("ERROR: Unsupported video mode!\n");
        return -1;
    }

    memmove(userhdcpointer, &hdc, sizeof(struct hdc));
    
    return 0;
}

// called from endpaint() to release the HDC lock
void sys_returnHDC(void){
    release(&gfx);
    //releaseconslock();
}

// called from endpaint() and redraw()
// copies the buffer passed from user space into video memory
int sys_outputgraphicsbuffertoscreen(void){

    char (*videobuffer)[320 * 200];
    if (argptr(0, (void*)&videobuffer, 4 * 320 * 200) < 0) {
        return -1;
    }

    int currentvideomode = getcurrentvideomode();
    if(currentvideomode == 0x13){
        memmove(cgaframebuffer, videobuffer[0], sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        for (short i = 0; i < 4; i++){
            setplane(i);
            uchar* mem = getframebufferbase();;
            memmove(mem, videobuffer[i], sizeof(char) * 320 * 200);
        }
    }
    return 0;
}

int validatepenrgb(int *rgbvalue){
    if (*rgbvalue < 0) {
        *rgbvalue = 0;
    } else if (*rgbvalue > 63) {
        *rgbvalue = 63;
    }
    return *rgbvalue;
}

// called directly form user programs.
int sys_setpencolour(void){
    int index;
    int r,g,b;

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

    // clamp RGB paramiters to valid values
    r = validatepenrgb(&r);
    g = validatepenrgb(&g);
    b = validatepenrgb(&b);

    outb(0x3C8, index);
    outb(0x3C9,r);
    outb(0x3C9,g);
    outb(0x3C9,b);
    return 0;
}
