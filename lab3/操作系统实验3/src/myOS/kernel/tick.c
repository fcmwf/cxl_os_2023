#include "tick.h"
extern void oneTickUpdateWallClock(int HH, int MM, int SS);

void tick(void){
	//finished
	static int millisecond=0;//100 per second

	//when turns to 0, a new second
	if(millisecond==0||system_ticks==1){
		oneTickUpdateWallClock(HH, MM, SS);//display time
	}

	//new millisecond
	millisecond++;
	system_ticks++;//called times
	//new second
	if(millisecond>=100){
		millisecond-=100;
		SS++;
		if(SS>=60){
			SS=SS-60;
			MM++;
		}
		if(MM>=60){
			MM=MM-60;
			HH++;
		}
	}
	return;
}