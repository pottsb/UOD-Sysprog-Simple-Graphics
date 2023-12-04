typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef enum {false, true} bool;
struct point {
    int x;
    int y;
};
struct hdc {
    struct point mypoint;
    struct point screen;
    short pen;
    bool locked;
    short videomode;
};
struct rect {
int top; // y co-ordinate of top of rectangle
int left; // x co-ordinate of left of rectangle
int bottom; // y co-ordinate of bottom of rectangle
int right; // x co-ordinate of right of rectangle
};