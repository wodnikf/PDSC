#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define centerX (gfx_screenWidth() / 2)
#define centerY (gfx_screenHeight() / 2)

#define centerAngle(number_of_vertex) (2 * M_PI / (number_of_vertex))

#define MAX 5

#define INIT_RADIUS 20
#define MAX_RADIUS 350

#define RATE_ANGLE 0.012
#define RATE_GROW 0.6

struct VERTEX {
	int x;
	int y;
};

void generate_polygon(struct VERTEX* tip, int radius, double angle)
{
	for (int i = 0; i < MAX; i++) {
		tip[i].x = centerX + radius * cos(i * centerAngle(MAX) + angle);
		tip[i].y = centerY + radius * sin(i * centerAngle(MAX) + angle);
	}
}
void draw_polygon(struct VERTEX* tip)
{
	for (int i = 0; i < MAX; i++) {
		if (i == MAX - 1) {
			gfx_line(tip[i].x, tip[i].y, tip[0].x, tip[0].y, RED);
		}
		else {
			gfx_line(tip[i].x, tip[i].y, tip[i + 1].x, tip[i + 1].y, RED);
		}
	}
}

double radius_condition(double dir, double radius){
	if (radius >= MAX_RADIUS) {
			dir *= -1;
		}
		else if (radius <= INIT_RADIUS) {
			dir *= -1;
		}
		return dir;
}


int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	struct VERTEX tip[MAX];

	double dir = RATE_GROW;
	double radius = INIT_RADIUS;
	double angle = 0.0;

	while (1) {
		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
					   BLACK);

		generate_polygon(tip, radius, angle);

		draw_polygon(tip);

		radius += dir;
		dir = radius_condition(dir, radius);

		angle += RATE_ANGLE;
		angle = fmod(angle, 2 * M_PI);

		gfx_updateScreen();
		SDL_Delay(10);
		//printf("%.4lf\n", angle);

		if (gfx_pollkey() == SDLK_ESCAPE){
			break;
        }

	}
	
	
	return 0;
}