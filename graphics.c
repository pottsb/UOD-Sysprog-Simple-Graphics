#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "spinlock.h"

static ushort *plane1 = (ushort*)P2V(0xA0000);
struct hdc hdc;
static struct {
    struct spinlock lock;
    int locking;
} gfx;

void clear320x200x256() {
    char* videoMemory = (char*)P2V(0xA0000);

    for(ushort i = 0; i < 320 * 200; i++){    
        videoMemory[i] = 0x0;
        hdc.videobuffer[0][i] = 0x0;
    }

}

void clear640x400x16(){
    for(ushort i = 0; i<=4; i++){
        setplane(i);
        char* videoMemory = (char*)getframebufferbase();

        for(uint j = 0; j < 320 * 200; j++){ 
            videoMemory[j] = 0x0;
        }
    }
    for(ushort i = 0; i<4; i++){
        for(uint j = 0; j < 320 * 200; j++){  
            hdc.videobuffer[i][j] = 0x0;
        }
    }
}

void dontcallthis(){
acquire(&gfx.lock);
release(&gfx.lock);
}

int sys_getHDC(void){

    

    struct hdc (*user_space_ptr);
    if (argptr(0, (void*)&user_space_ptr,sizeof(struct hdc)) < 0) {
        return -1;
    }
        
    // lock the hdc and reset vars
    hdc.mypoint.x = 0;
    hdc.mypoint.y = 0;
    hdc.pen = 15;

    // store the screen res based on video mode when begin paint is called.
    // this is used for bounds checking and pixel position calculations.
    int currentvideomode = getcurrentvideomode();
    hdc.videomode = currentvideomode;
    if(currentvideomode == 0x13){
        hdc.screen.x = 320;
        hdc.screen.y = 200;
        memmove(hdc.videobuffer[0], plane1, sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        hdc.screen.x = 640;
        hdc.screen.y = 400;
        for (short j = 0; j < 4; j++){
            setplane(j);
            uchar* mem = getframebufferbase();
            memmove(hdc.videobuffer[j], mem, sizeof(char) * 320 * 200);
        }
    }else{
        cprintf("ERROR: Unsupported video mode!\n");
        return -1;
    }

    memmove(user_space_ptr, &hdc, sizeof(struct hdc));
    return 1;
}


void sys_returnHDC(void){
    
}


int sys_outputgraphicsbuffertoscreen(void){

    char (*videobuffer)[320 * 200];
    if (argptr(0, (void*)&videobuffer, 4 * 320 * 200) < 0) {
        return -1;
    }

    int currentvideomode = getcurrentvideomode();
    if(currentvideomode == 0x13){
        memmove(plane1, videobuffer[0], sizeof(char) * 320 * 200);
    }else if(currentvideomode == 0x12){
        for (short i = 0; i < 4; i++){
            setplane(i);
            uchar* mem = getframebufferbase();;
            memmove(mem, videobuffer[i], sizeof(char) * 320 * 200);
        }
    }
    return 0;
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
