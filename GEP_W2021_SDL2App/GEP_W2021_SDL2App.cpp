/****CREATION OF A PING PONG GAME*****/

#include <iostream>
#include <SDL.h>
#define SDL_MAIN_HANDLED
#include <SDL_ttf.h>
#include <string>

using namespace std;

bool isRunning = true;
SDL_Renderer* renderer;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Event event;  // to handle events
SDL_Rect playerPaddle;
SDL_Rect aiPaddle;
SDL_Rect ball;
SDL_Rect background = { 0,0, WINDOW_WIDTH, WINDOW_HEIGHT };

/* font related declaration */
TTF_Font* font; 
SDL_Texture* fontTexture;

SDL_Rect fontRectScore; // this defines the position of our 'score' text on the screen
void RenderFont(const char* text, int x, int y, bool isRefreshText);

int mouseX, mouseY;

bool InitGameEngine();
void InitGameWorld();
void Input();
void Update();
void Render();
void Quit();

int main(int argc, char * argv[])
{
	
	if (!InitGameEngine()) return -1;
	InitGameWorld();

	// setup the game loop
	while (isRunning)
	{
		Input();
		Update();
		Render();
	}

	Quit();
}

bool InitGameEngine()
{
	SDL_Window* window = SDL_CreateWindow("First SDL App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!window)
	{
		cout << "Window initialization failed.\n";
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		cout << "Renderer initialization failed.\n";
		return false;
	}

	// check if TTF engine can be used....if it can't be initialized, return false
	if (TTF_Init() != 0)
	{
		cout << "TTF Font Init failed\n";
		return false;
	}

	return true;
}

void InitGameWorld()
{
	font = TTF_OpenFont("Assets/Fonts/LTYPE.TTF", 30); // 30 is inital font size

	playerPaddle.x = 20;
	playerPaddle.y = WINDOW_HEIGHT * 0.5 - 50; // set it just above the half of the window's center
	playerPaddle.h = 100;
	playerPaddle.w = 20;

	aiPaddle.x = WINDOW_WIDTH - 40;	//sets the aiPaddle 40 pixels to the left of the right-most window corner
	aiPaddle.y = WINDOW_HEIGHT * 0.5 - 50;
	aiPaddle.h = 100;
	aiPaddle.w = 20;

	ball.x = (WINDOW_WIDTH * 0.5) - 30; //around 370 pixels if window is 800 in width
	ball.y = (WINDOW_HEIGHT * 0.5); // around the center, at above the 290 pixels
	ball.w = 20;
	ball.h = 20;
}

void RenderFont(const char* text, int x, int y, bool isRefreshText)
{
	if (isRefreshText)
	{
		SDL_Color textColor = { 255, 255, 255, 0 }; // white color
		
		SDL_Surface* fontSurface = TTF_RenderText_Solid(font, text, textColor);

		// need to first de-allocate any previously assigned font-texture
		SDL_DestroyTexture(fontTexture);

		fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
		fontRectScore = { x, y, fontSurface->w, fontSurface->h };
	}
	// draw the font
	SDL_RenderCopy(renderer, fontTexture, 0, &fontRectScore);
} 

void Render()
{
	SDL_RenderClear(renderer); // clear previously drawn frame

	// start drawing the current frame
	// render the backgroun
	// 5,30,67 represents a dark blue color. Check MS Paint's edit color to get your own rgb value
	//the fifth argument below, 255 stands for the alpha i.e: transparency of the color 255=opaque, 0=transparent
	SDL_SetRenderDrawColor(renderer, 5, 30, 67, 255);
	SDL_RenderFillRect(renderer, &background);	// fill the background rect with the above color

	// render the player & aiPaddle
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // draw paddles in white
	SDL_RenderFillRect(renderer, &playerPaddle); // fill rect with above white color
	SDL_RenderFillRect(renderer, &aiPaddle); // fill the aiPaddle rect with the above white color as well

	//render the ball
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &ball);

	int score = 45;
	string s = "score: " + to_string(score);
	RenderFont(s.c_str(), 30, 50, true);

	SDL_RenderPresent(renderer); // must call this to render the above defined
}

void Quit()
{
	// shudown SDL
	SDL_Quit();
}

void Input()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEMOTION) // if mouse movement is detected
		{
			SDL_GetMouseState(&mouseX, &mouseY);
		}

		//if we click 'X' button to close the app window, or when we press ESC
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}

		// check if key was pressed
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
			}
		}
	}
}

void Update()
{
	playerPaddle.y = mouseY;	// map the playerPaddle's position to the mouse y's position

	// our initial demo ball movement to be removed and replaced with something else later
	ball.x += 1;
	ball.y += 1;

	// slow down by using SDL Delay
	SDL_Delay(10);	// pause the game loop for 10 milliseconds to slow down the game to a decent speed
}



