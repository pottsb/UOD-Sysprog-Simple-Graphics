#include "types.h"
#include "user.h"



int main(void)
{
    setvideomode(0x13);
    char *argv[] = {"exectest_p1",0};
    int pid = fork();

    if(pid == 0){
        printf(1,"EXEC CALL TO PROCESS 1\n"); 
        exec("exectest_p1",argv);
        exit(); // Not expecting this to be hit.
    }


    sleep(1);
    printf(1,"PROCESS 2 TRY GET HDC\n");   
    int hdc = beginpaint(0);
    printf(1,"PROCESS 2 START\n");
    for (int i = 10; i < 20; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 5);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);
    printf(1,"PROCESS 2 END\n");

    wait();
    getch();
    
    setvideomode(0x03);
    exit();
}