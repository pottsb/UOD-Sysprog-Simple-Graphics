typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
struct point {
    int x;
    int y;
};
struct hdc {
    struct point lastpoint;
    struct point screen;
    short pen;
    bool locked;
};