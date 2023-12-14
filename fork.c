#include "types.h"
#include "user.h"



int main(int argc, char* argv[])
{
    setvideomode(0x12);

    int pid = fork();

    if(pid == 0){
    printf(1,"FORK 1 TRY GET HDC\n");    
    int hdc = beginpaint(0);
    printf(1,"FORK 1 START\n");
    for (int i = 0; i < 10; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 10);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);
    printf(1,"FORK 1 END\n");
    exit();
    }else if (pid > 0){
        sleep(1);
    printf(1,"FORK 2 TRY GET HDC\n");   
    int hdc = beginpaint(0);
    printf(1,"FORK 2 START\n");
    for (int i = 10; i < 20; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 5);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);
    printf(1,"FORK 2 END\n");

    }

    wait();
    getch();
    
    setvideomode(0x03);
    exit();
}