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

// Stuff I added
// ====================================================================================

struct hdc hdc;

int beginpaint(int hwnd){
    getHDC(&hdc); 
    return 0;
}

int endpaint(int hdcIndex){
    outputgraphicsbuffertoscreen(hdc.videobuffer);
    returnHDC();
    return 0;
}

int redraw(int hdcIndex){
    outputgraphicsbuffertoscreen(hdc.videobuffer);
    return 0;
}

// all pixel writes to the buffer go through this function
// this handles the difference in writing to the buffer in the different display modes
void setpixelinbuffer(int hdcIndex, int x, int y) {
    ushort offset = hdc.screen.x * y + x;
    if (hdc.videomode == 0x13) {
        hdc.videobuffer[0][offset] = hdc.pen;
    } else if (hdc.videomode == 0x12) {
        //pixel position in the buffer
        ushort byteoffset = (hdc.screen.x * y + x) / 8;
        //pixel data position in the byte
        ushort bitposition = x % 8;

        // set one pixels worth of data accross the four planes at a time
        for (ushort i = 0; i < 4; i++) {
            ushort colorbit = (hdc.pen >> i) & 1; // get the current bit for the plane
            if (colorbit) {
                hdc.videobuffer[i][byteoffset] |= (1 << (7 - bitposition)); // set the bit to 1
            } else {
                hdc.videobuffer[i][byteoffset] &= ~(1 << (7 - bitposition)); // set the bit to 0
            }
        }
    }
}

// clamp the pixel values to valid positions on the screen
int validatecoordinate(int *coordinate, int lowerlimit, int upperlimit) {
    if (*coordinate < lowerlimit) {
        *coordinate = lowerlimit;
    } else if (*coordinate > upperlimit) {
        *coordinate = upperlimit;
    }
    return *coordinate;
}

int setpixel(int hdcIndex, int x, int y){

    x = validatecoordinate(&x, 0, hdc.screen.x);
    y = validatecoordinate(&y, 0, hdc.screen.y);
    
    setpixelinbuffer(hdcIndex,x,y);
    return 0;
}

int moveto(int hdcIndex, int x, int y){

    x = validatecoordinate(&x, 0, hdc.screen.x);
    y = validatecoordinate(&y, 0, hdc.screen.y);

    hdc.lastpoint.x = x;
    hdc.lastpoint.y = y;

    return 0;
}

int abs(int n) {
    return (n < 0) ? -n : n;
}

int lineto(int hdcIndex, int x2, int y2){

    x2 = validatecoordinate(&x2, 0, hdc.screen.x);
    y2 = validatecoordinate(&y2, 0, hdc.screen.y);

    // set the start point to the last saved location.
    ushort y1 = hdc.lastpoint.y;
    ushort x1 = hdc.lastpoint.x;

    // Bresenham’s line algorithm
    ushort dx = abs(x2 - x1);
    ushort dy = abs(y2 - y1);
    ushort sx = (x1 < x2) ? 1 : -1;
    ushort sy = (y1 < y2) ? 1 : -1;
    short err = dx - dy;

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

    // set the saved location to the end of the line
    hdc.lastpoint.x = x2;
    hdc.lastpoint.y = y2;

    return 0;
}

int fillrect(int hdcIndex, struct rect *rect){

    rect->top = validatecoordinate(&rect->top, 0, hdc.screen.y);
    rect->bottom = validatecoordinate(&rect->bottom, 0, hdc.screen.y);
    rect->left = validatecoordinate(&rect->left, 0, hdc.screen.x);
    rect->right = validatecoordinate(&rect->right, 0, hdc.screen.x);
 
    
    for (ushort y = rect->top; y <= rect->bottom; y++) {
            for (ushort x = rect->left; x <= rect->right; x++) {
                setpixelinbuffer(hdcIndex,x,y);
            }
        }

    return 0;
}

int selectpen(int hdcIndex, int index){

    ushort maxindex = 0;
    if(hdc.videomode == 0x13){
        maxindex = 255;
    }else if(hdc.videomode == 0x12){
        maxindex = 15;
    }

    if (index < 0 || index > maxindex) {
        printf(1,"ERROR: Pen index out of range!\n");
        return -1;
    }

    hdc.pen = index;
    return 0;
}