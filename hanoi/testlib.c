#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


#define EMPTY_STACK -1


#define NUMBER_PEGS 3
#define NUMBER_DISK 5


typedef struct 
{
	int x_left;
	int x_right;
	int y_top;
	int y_bot;
	int index;
}Disk;


//stack
typedef struct
{
	Disk disks[NUMBER_DISK];
	int top;
}Peg;

void push(Peg *s, Disk index){

	// is full
	if (s->top == NUMBER_DISK -1){
		return;
	}

	s->top++;
	s->disks[s->top] = index;

}

Disk pop(Peg *s){
	if (s->top == EMPTY_STACK){
		return;
	}

	Disk popped_disk = s->disks[s->top];
	s->top--;
	return popped_disk;
}



int get_peg_x(int index){
	int x = gfx_screenWidth() / (NUMBER_PEGS + 1)* index;
	return x;
}
/* it kinda work as generating pegs
for (int i = 0; i < NUMBER_PEGS; i++){
		x = get_peg_x(i + 1);

		gfx_filledRect(x, gfx_screenHeight() - 75, x+10, gfx_screenHeight() - 1, GREEN);
}
*/
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}
	int x;
	

	while(1){
		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);

		gfx_filledRect(0, gfx_screenHeight() - 25, gfx_screenWidth() -1, gfx_screenHeight() - 1, YELLOW);

		

		gfx_updateScreen();
		SDL_Delay(10);
	}

	

	return 0;
}
