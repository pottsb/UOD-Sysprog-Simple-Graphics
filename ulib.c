#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*strcpy(char *s, const char *t) {
    char *os;

    os = s;
    while ((*s++ = *t++) != 0) {
        ;
    }
    return os;
}

int strcmp(const char *p, const char *q) {
    while (*p && *p == *q) {
        p++, q++;
    }
    return (uchar) * p - (uchar) * q;
}

uint strlen(const char *s)      {
    int n;

    for (n = 0; s[n]; n++) {
        ;
    }
    return n;
}

void* memset(void *dst, int c, uint n) {
    stosb(dst, c, n);
    return dst;
}

char* strchr(const char *s, char c) {
    for (; *s; s++) {
        if (*s == c) {
            return (char*)s;
        }
    }
    return 0;
}

char* gets(char *buf, int max) {
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1) {
            break;
        }
        buf[i++] = c;
        if (c == '\n' || c == '\r') {
            break;
        }
    }
    buf[i] = '\0';
    return buf;
}

int stat(const char *n, struct stat *st) {
    int fd;
    int r;

    fd = open(n, O_RDONLY);
    if (fd < 0) {
        return -1;
    }
    r = fstat(fd, st);
    close(fd);
    return r;
}

int atoi(const char *s) {
    int n;

    n = 0;
    while ('0' <= *s && *s <= '9') {
        n = n * 10 + *s++ - '0';
    }
    return n;
}

void* memmove(void *vdst, const void *vsrc, int n) {
    char *dst;
    const char *src;

    dst = vdst;
    src = vsrc;
    while (n-- > 0) {
        *dst++ = *src++;
    }
    return vdst;
}

// ====================================================================================
#define MAX_HDC 4
struct hdc hdcarray[MAX_HDC];
char  videobuffer[4][320 * 200];

int beginpaint(int hwnd){

    // loop through all hdcs checking for a free one
    ushort i;
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
            hdcarray[i].videomode = currentvideomode;
            if(currentvideomode == 0x13){
                hdcarray[i].screen.x = 320;
                hdcarray[i].screen.y = 200;
            }else if(currentvideomode == 0x12){
                hdcarray[i].screen.x = 640;
                hdcarray[i].screen.y = 400;
            }else{
                hdcarray[i].locked = false;
                printf(1,"ERROR: Unsupported video mode!\n");
                return -1;
            }
            return i;
        }
    }

    printf(1,"ERROR: No free HDC!\n");
    return -1;
}

int endpaint(int hdc){
    outputgraphicsbuffertoscreen(videobuffer);
    hdcarray[hdc].locked = false;
    return 1;
}

int redraw(int hdc){
    outputgraphicsbuffertoscreen(videobuffer);
    return 1;
}


void setpixelinbuffer(int hdcIndex, int x, int y) {
    ushort offset = hdcarray[hdcIndex].screen.x * y + x;
    if (hdcarray[hdcIndex].videomode == 0x13) {
        videobuffer[0][offset] = hdcarray[hdcIndex].pen;
    } else if (hdcarray[hdcIndex].videomode == 0x12) {
        ushort byteOffset = (hdcarray[hdcIndex].screen.x * y + x) / 8;
        ushort bitPosition = x % 8;

        for (short i = 0; i < 4; i++) {
            short color_bit = (hdcarray[hdcIndex].pen >> i) & 1; // Isolate the bit for the current plane
            if (color_bit) {
                videobuffer[i][byteOffset] |= (1 << (7 - bitPosition)); // Set the bit at the correct position
            } else {
                videobuffer[i][byteOffset] &= ~(1 << (7 - bitPosition)); // Clear the bit
            }
        }
    }
}

int setpixel(int hdcIndex, int x, int y){

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

int moveto(int hdcIndex, int x, int y){

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

    return 0;
}

int abs(int n) {
    return (n < 0) ? -n : n;
}

int lineto(int hdcIndex, int x2, int y2){

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

    return 0;
}



int selectpen(int hdcIndex, int index){

    int maxindex = 0;
    if(hdcarray[hdcIndex].videomode == 0x13){
        maxindex = 255;
    }else if(hdcarray[hdcIndex].videomode == 0x12){
        maxindex = 15;
    }

    if (index < 0 || index > maxindex) {
        printf(1,"ERROR: Pen index out of range!\n");
        return -1; // Return an error code to indicate out-of-bounds
    }

    hdcarray[hdcIndex].pen = index;
    return 0;
}

int fillrect(int hdcIndex, struct rect *rect){
    
    for (int y = rect->top; y <= rect->bottom; y++) {
            for (int x = rect->left; x <= rect->right; x++) {
                setpixelinbuffer(hdcIndex,x,y);
            }
        }

    return 0;
}