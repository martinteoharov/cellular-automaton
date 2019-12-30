#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "raylib.h"
#include "automaton.h"

#define WIDTH  1000
#define HEIGHT 500

int main(){
	InitWindow(WIDTH, HEIGHT, "cellular-automaton");
	Simulation Sim;

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 0.0f };
       	camera.zoom = 1.0f;
       	SetTargetFPS(60);

	std::vector<Pixel> pixels;

	int timestep = 0;
	bool startSim = false;
	Sim.setup(pixels, 500);
	while(!WindowShouldClose()){
		Sim.handleKeyPresses(camera, 100, 100, pixels);
		Sim.handleDraw(camera, pixels);
		IsKeyPressed(KEY_P) ? startSim = !startSim : NULL;

		if( timestep > 0 && startSim ){
			timestep = 0;
			Sim.handleSim(camera, pixels);
		}
		timestep++;
	}
	CloseWindow();
}
