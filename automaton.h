class Pixel {
	private:
		int x;
		int y;
	public:
		Pixel(int _x, int _y, std::string _color){
			x      = _x;
			y      = _y;
		}
		void Draw(Color color){
			DrawRectangle(x, y, 1, 1, color);
		}

		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
};

class Simulation {
	private:
		bool pixelMap[1000][500] = {false};

	public:
		void setup(std::vector<Pixel>& pixels, int n){
			for(int i = 0; i < n; i ++ ){
				int x = rand()%200;
				int y = rand()%200;

				if(!pixelMap[x][y]){
					Pixel pixel(x, y, "RED");
					pixels.push_back(pixel);
					pixelMap[x][y] = true;
				}
			}
		}
						
						
		int countNeighbours(int x, int y){
			int cnt = 0;
			for(int i = -1; i < 2; i ++ ){
				for( int m = -1; m < 2; m ++ ){
					if( i == 0 && m == 0 ){
					}
					else{
						if( x + i < 1000 && y + m < 500 && x +i > 10 && y + m > 10){
							if(pixelMap[x+i][y+m]){
								cnt++;
							}
						}
					}
				}
			}
			return cnt;
		}
		void handleKeyPresses(Camera2D& camera, int x, int y, std::vector<Pixel>& pixels){
			//Spawn color pixel
			if(IsMouseButtonDown(0)){
				//color pixel
				int x = GetMouseX();
				int y = GetMouseY();

				int realX = GetMouseX()/camera.zoom - camera.offset.x/camera.zoom;
				int realY = GetMouseY()/camera.zoom - camera.offset.y/camera.zoom;
				if(pixelMap[realX][realY] == false){
					Pixel pixel(realX, realY, "DARKGRAY");
					pixels.push_back(pixel);
					pixelMap[realX][realY] = true;
				}
			}
			if (IsKeyDown(KEY_D)){
				camera.offset.x -= 10;
			}
			if (IsKeyDown(KEY_A)){
				camera.offset.x += 10;
			}
			if (IsKeyDown(KEY_W)){
				camera.offset.y += 10;
			}
			if (IsKeyDown(KEY_S)){
				camera.offset.y -= 10;
			}

			camera.zoom += ((float)GetMouseWheelMove()*.5f);
		}
		void handleSim(Camera2D& camera, std::vector<Pixel>& pixels){

			//elements to erase
			struct e2e {
				Pixel pixel;
			};
			//elements to create
			struct e2c {
				int x;
				int y;
			};
			//pixels to erase
			std::vector<e2e> p2e;
			//pixels to create
			std::vector<e2c> p2c;
			std::vector<int> indexes;

	//		std::cout << pixels.size() << std::endl;
			for( int i = 0; i < pixels.size(); i ++ ){
				int x = pixels[i].getX();
				int y = pixels[i].getY();

				int neigb;
				
				for(int i = -1; i < 2; i ++ ){
					for(int m = -1; m < 2; m ++ ){
						if(i == 0 && m == 0){
							//skip
						}
						else{
							if( x+i < 995 && y+m < 495 && x > 10 && y > 10){
								neigb = countNeighbours(x+i, y+m);
								if(neigb == 3 && pixelMap[x+i][y+m] == false){
									p2c.push_back(e2c{x+i, y+m});
								}
							}
						}
					}
				}

				if( x < 995 && y < 495 && x > 10 && y > 10){
					neigb = countNeighbours(x, y);
				}

				if(neigb < 2 || neigb > 3){
					p2e.push_back(e2e{pixels[i]});
					indexes.push_back(i);
				}
			}
			std::sort(indexes.begin(), indexes.end());
			for( int i = 0; i < p2e.size(); i ++ ){
				pixelMap[p2e[i].pixel.getX()][p2e[i].pixel.getY()] = false;
				pixels.erase(pixels.begin() + indexes[i] - i);
			}
			for( int i = 0; i < p2c.size(); i ++ ){
				int x = p2c[i].x;
				int y = p2c[i].y;
				Pixel pixel(x, y, "RED");

				if(!pixelMap[x][y])
					pixels.push_back(pixel);

				pixelMap[x][y] = true;
			}
			p2e.clear();
			p2c.clear();
		}
		void handleDraw(Camera2D& camera, std::vector<Pixel>& pixels){
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);
			for( int i = 0; i < pixels.size(); i ++ ){
				pixels[i].Draw(RED);
			}

			EndMode2D();
			EndDrawing();
		}

};
