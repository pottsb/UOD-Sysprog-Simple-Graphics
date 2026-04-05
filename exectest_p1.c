#include "types.h"
#include "user.h"

int main(void){

    printf(1,"PROCESS 1 TRY GET HDC\n");    
    int hdc = beginpaint(0);
    printf(1,"PROCESS 1 START\n");
    for (int i = 0; i < 10; i++){
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 10);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20);
        lineto(hdc, i * 10, i * 5 + 20);
        lineto(hdc, i * 10, i * 5);
    }
    endpaint(hdc);
    printf(1,"PROCESS 1 END\n");

    exit();
}