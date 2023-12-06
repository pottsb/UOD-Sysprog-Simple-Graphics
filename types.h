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
};