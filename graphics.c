#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"


static ushort *plane1 = (ushort*)P2V(0xA0000);


void clear320x200x256() {
    char* videoMemory = (char*)P2V(0xA0000);

    for(ushort i = 0; i < 64320; i++){    
        videoMemory[i] = 0x0;
    }
}

void clear640x400x16(){
    for(ushort i = 0; i<=4; i++){
        setplane(i);
        char* videoMemory = (char*)getframebufferbase();

        for(uint j = 0; j < 64320; j++){    // char is 4 bits 4 * 64320 = all the pixels
            videoMemory[j] = 0x0;
        }
    }
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
