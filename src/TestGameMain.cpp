#include "Camera.hpp"
#include "Window.hpp"
#include "ShaderProgram.hpp"
#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"

#include "UVSG.hpp"


#include <SDL2/SDL.h>
#undef main //Not sure if this is needed

#include <stdlib.h>
#include <math.h>

#include "InputButton.hpp"

using namespace std;



int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	UVSG* game = new UVSG();

    InputButton input = InputButton();

    int num_joy = SDL_NumJoysticks();
    printf("%i joystick(s) were found.\n\n", num_joy);
    for(int i = 0; i < num_joy; i++)
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
		game->controller = SDL_GameControllerOpen(i);
        printf("%s\n", SDL_JoystickName(joystick));
		if (SDL_JoystickIsHaptic(joystick) == 1)
		{
			game->haptic = SDL_HapticOpenFromJoystick(joystick);
			game->joystick = joystick;
		}
    }

	int num_gameController = 0;

    float deltaTime = 0;

    Uint32 lastTime = 0;
    Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frames = 0;

    bool mouseCaptured = false;

	while(!UVSG::getInstance()->getShouldClose())
	{

        //FPS counter stuff
        currentTime = SDL_GetTicks();
        frames++;
        if(currentTime - lastTime > 1000)
        {
            cout << "FPS: " << frames << endl;
            lastTime = currentTime;
            frames = 0;
        }
        Uint32 delta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

		deltaTime = ((float)delta) / 1000.0f;
		//cout << "Update delta: " << deltaTime << endl;

		UVSG::getInstance()->update(deltaTime);
	}

	delete game;

	return 0;
}
