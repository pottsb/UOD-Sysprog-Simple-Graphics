#include "types.h"
#include "user.h"



int main(int argc, char* argv[])
{
    setvideomode(0x13);

    int pid = fork();

    if(pid == 0){

    int hdc = beginpaint(0);
    for (int i = 0; i < 10; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 10);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);
    exit();
    }else if (pid > 0){
        //sleep(1);
    int hdc = beginpaint(0);
    for (int i = 10; i < 20; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 5);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);

    }

    
    getch();
    wait();
    setvideomode(0x03);
    exit();
}