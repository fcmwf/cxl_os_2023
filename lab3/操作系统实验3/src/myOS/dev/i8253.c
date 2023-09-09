#include "io.h"

void init8253(void){
	//finished
	outb(0x0043,0x34);//init port
	outb(0x0040,0x9C);//lower of 0x2E9C=11932
	outb(0X0040,0X2E);//higher of 0x2E9C
	return;
}
