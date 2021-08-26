/*
 * Copyright 2020~2021 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: SYSON, MICHAEL B.
 * @date created: 20200926
 * @date updated: 20210826
 * @website address: http://www.usbong.ph
 *
 * References:
 * 1) Dev-C++ 5.11 auto-generated OpenGL example project
 *
 * 2) https://www.libsdl.org/download-2.0.php;
 * last accessed: 20200423
 *
 * 3) https://docs.microsoft.com/en-us/windows/win32/inputdev/user-input3
 * last accessed: 20200424
 *
 * 4) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL Game Programming". USA: Premier Press.
 *
 * 5) https://en.wikibooks.org/wiki/OpenGL_Programming/Basics/Structure
 * last accessed: 20200926
 *
 * 6) https://dos.gamebub.com/cpp.php
 * last accessed: 20200921
 *
 * 7) https://chortle.ccsu.edu/Bloodshed/howToGL.html
 * last accessed: 20200927
 * --> instructions to add GLUT library in Dev-C++ Editor 5.11
 *
 * 8) https://stackoverflow.com/questions/5289284/compiling-and-runnin-opengl-glut-program-in-ubuntu-10-10
 * --> last accessed: 20200928
 *
 * 9) https://askubuntu.com/questions/96087/how-to-install-opengl-glut-libraries
 * --> last accessed: 20200928
 *
 * 10) https://www3.ntu.edu.sg/home/ehchua/programming/opengl/HowTo_OpenGL_C.html
 * --> last accessed: 20200928
 *
 * 11) https://stackoverflow.com/questions/2571402/how-to-use-glortho-in-opengl/36046924#36046924;
 * --> last accessed: 20200928
 * --> answers by: Mikepote, 20100408T1912
 * --> answers by: Santilli, Ciro, 20160316T2106
 *
 * 12) https://www.parallelrealities.co.uk/tutorials/;
 * --> last accessed: 20210818
 * --> 12.1) https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php;
 * --> last accessed: 20210818
 * --> 12.2) https://www.parallelrealities.co.uk/tutorials/shooter/shooter3.php;
 * --> last accessed: 2021819
 *
 * TO-DO: -reverify: execution speed in 32-bit OS
 *
 * 13) https://wiki.libsdl.org/SDL_SetRenderDrawColor;
 * --> last accessed: 20210818
 *
 * Notes:
 * 1) We can use this software tool to extract the compressed (zipped) folder.
 *   https://www.7-zip.org/download.html; last accessed: 20200423
 *
 * 2) OpenGL (Open Graphics Library) Utility Toolkit Library
 *
 * 3) Linux Machine
 * 3.1) Compile Command
 *   g++ main.cpp -o mainOutput -lGL -lGLU -lglut
 *
 * 3.2) Execute Command
 *   ./mainOutput
 *
 * 3.3) Install OpenGL Libraries
 *   sudo apt-get install mesa-utils
 *
 * 3.4) Install GLUT Library
 *   sudo apt-get install freeglut3-dev
 */


//TO-DO: -add: installation of SDL and SDL Image


/**************************
 * Includes
 *
 **************************/

//added by Mike, 20201209
//Reference: https://stackoverflow.com/questions/34152424/autodetection-of-os-in-c-c;
//answer by: Jeegar Patel, 20151208T0940
//auto-identify if Windows Machine
#ifdef _WIN32
#include <windows.h> //Windows Machine
#endif
/*
 #ifdef linux
 printf("In Linux");
 #endif
 */


//added by Mike, 20201209
#if defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#endif

//added by Mike, 20210818
#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
//added by Mike, 20210825
#elif defined(__APPLE__)
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

#include "OpenGLCanvas.h"

//added by Mike, 20201210
#include <time.h>
#include <unistd.h>

//added by Mike, 20201002
#include <iostream>

//added by Mike, 20201226
#define TRUE 1
#define FALSE 0

static int myWindowWidthAsPixel=640; //2048;//320
static int myWindowHeightAsPixel=640; //2048;//320

//added by Mike, 20210825
OpenGLCanvas *myOpenGLCanvas = NULL;

//added by Mike, 20210818
SDL_Window *mySDLWindow = NULL;
SDL_Renderer *mySDLRenderer = NULL;

//added by Mike, 20210819
int iPilotX;
int iPilotY;
int iCountTaoAnimationFrame;

int myKeysDown[10]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,

bool bMyPause;
int myMouseActionState;


//added by Mike, 20210510
//note: keys and mouseActionIds equal with that in OpenGLCanvas.cpp
enum Keys
{
    KEY_W = 0,
    KEY_S,
    KEY_D,
    KEY_A,
    KEY_J,
    KEY_L,
    KEY_I,
    KEY_K,
    //added by Mike, 20210123
    KEY_H,
    KEY_U,
    //added by Mike, 20210513
    KEY_Z,
    
    iNumOfKeyTypes
};

//added by Mike, 20210510
enum mouseActionIds
{
    MOUSE_LEFT_BUTTON = 0,
    iNumOfMouseActionIdTypes
};

//note: special functions, e.g. shift, control, arrow keys
//UP: 101
//DOWN: 103
//LEFT: 100
//RIGHT: 102
//added by Mike, 20201002
void specialKeyDown (int specialKey, int x, int y)
{
    //noted by Mike, 20201121
    //Note: space bar and escape key not detected
    //    std::cout << "keydown " << specialKey << "\n";
    
    switch (specialKey)
    {
            /* //edited by Mike, 20201121
             //added by Mike, 20201001
             case VK_LEFT:
             myOpenGLCanvas->keyDown(KEY_LEFT);
             return;
             case VK_RIGHT:
             myOpenGLCanvas->keyDown(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyDown(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyDown(KEY_DOWN);
             return;
             //removed by Mike, 20201002
             //   	       case VK_SPACE:
             //                myOpenGLCanvas->keyDown(KEY_SPACE);
             //                return;
             */
            //TO-DO: -add: this
            //removed by Mike, 20201121
            /*
             case VK_ESCAPE:
	            PostQuitMessage(0);
	            return;// 0;
             */
            //removed by Mike, 20210130
            /*
             //added by Mike, 20201001
             case VK_LEFT:
             myOpenGLCanvas->keyDown(KEY_LEFT);
             return;// 0;
             case VK_RIGHT:
             myOpenGLCanvas->keyDown(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyDown(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyDown(KEY_DOWN);
             return;
             */
    }
    return;
    
}
//added by Mike, 20201002
void specialKeyUp (int specialKey, int x, int y)
{
    //    std::cout << "keyup " << specialKey << "\n";
    
    switch (specialKey)
    {
            //removed by Mike, 20201121
            /*
             case VK_LEFT:
             myOpenGLCanvas->keyUp(KEY_LEFT);
             return;
             case VK_RIGHT:
             myOpenGLCanvas->keyUp(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyUp(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyUp(KEY_DOWN);
             return;
             //removed by Mike, 20201002
             //   	       case VK_SPACE:
             //                myOpenGLCanvas->keyUp(KEY_SPACE);
             //                return;
             */
            //removed by Mike,  20210130
            /*
             case VK_LEFT:
             myOpenGLCanvas->keyUp(KEY_LEFT);
             return;// 0;
             case VK_RIGHT:
             myOpenGLCanvas->keyUp(KEY_RIGHT);
             return;
             case VK_UP:
             myOpenGLCanvas->keyUp(KEY_UP);
             return;
             case VK_DOWN:
             myOpenGLCanvas->keyUp(KEY_DOWN);
             return;
             */
    }
    return;
}

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	//includes window title bar	
	SDL_DisplayMode mySDLDisplayMode;
	SDL_GetCurrentDisplayMode(0, &mySDLDisplayMode);
	
	myWindowWidthAsPixel=mySDLDisplayMode.w;
	myWindowHeightAsPixel=mySDLDisplayMode.h;
	
	
//	mySDLWindow = SDL_CreateWindow("Pagong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags);
	mySDLWindow = SDL_CreateWindow("Pagong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags | SDL_WINDOW_BORDERLESS);
	

	if (!mySDLWindow )
	{
		printf("Failed to open %d x %d window: %s\n", myWindowWidthAsPixel, myWindowHeightAsPixel, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	mySDLRenderer = SDL_CreateRenderer(mySDLWindow, -1, rendererFlags);

	if (!mySDLRenderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
	
	//removed by Mike, 20210819
	//SDL_GetRendererOutputSize(mySDLRenderer, &myWindowWidthAsPixel, &myWindowHeightAsPixel);
}

void keyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
//		if (event->keysym.scancode == SDL_SCANCODE_UP)
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
//			iPilotY-=4;
			myKeysDown[KEY_W] = TRUE;		
			myOpenGLCanvas->keyDown(KEY_W);			
		}

//		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
//			iPilotY+=4;
			myKeysDown[KEY_S] = TRUE;					
			myOpenGLCanvas->keyDown(KEY_S);			
		}

//		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
//			iPilotX-=4;
			myKeysDown[KEY_A] = TRUE;					
			myOpenGLCanvas->keyDown(KEY_A);			
		}

//		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
//			iPilotX+=4;
			myKeysDown[KEY_D] = TRUE;					
			myOpenGLCanvas->keyDown(KEY_D);			
		}
	}
}

void keyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
//		if (event->keysym.scancode == SDL_SCANCODE_UP)
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
			myKeysDown[KEY_W] = FALSE;		
			myOpenGLCanvas->keyUp(KEY_W);									
		}

//		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
			myKeysDown[KEY_S] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_S);									
		}

//		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
			myKeysDown[KEY_A] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_A);									
		}

//		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
			myKeysDown[KEY_D] = FALSE;					
			myOpenGLCanvas->keyUp(KEY_D);									
		}
	}
}

void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				keyDown(&event.key);
				break;

			case SDL_KEYUP:
				keyUp(&event.key);
				break;

			default:
				break;
		}
	}
}

void prepareScene(void)
{
	//edited by Mike, 20210818
//	SDL_SetRenderDrawColor(mySDLRenderer, 96, 128, 255, 255);
//	SDL_SetRenderDrawColor(mySDLRenderer, 255, 0, 0, 255); //red
	//note: SDL color max 255; GIMP color max 100
	SDL_SetRenderDrawColor(mySDLRenderer, 0, 255*0.667, 255*0.494, 255); //blue green
	
	SDL_RenderClear(mySDLRenderer);
}

void presentScene(void)
{
	SDL_RenderPresent(mySDLRenderer);
}

//added by Mike, 20210826
void enable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
	
		//glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
		glOrtho(0, myWindowWidthAsPixel, myWindowHeightAsPixel, 0, -1, 1);
	
	
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
}

//added by Mike, 20210826
void disable2D()
{
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

//added by Mike, 20210826
//TO-DO: -reverify: refresh rate to cause displayed parts of pixel texture to be incorrect
//note: did NOT observe this with openGL + GLUT without SDL
void displayOpenGLCanvas() {	
	//TO-DO: -update: this
	enable2D();
		myOpenGLCanvas->render();
	disable2D();
		
/*	//added by Mike, 20210821; edited by Mike, 20210821
  SDL_GL_SwapBuffers();
  SDL_UpdateWindowSurface(mySDLWindow);
*/  
  SDL_GL_SwapWindow(mySDLWindow);
}


/*	//removed by Mike, 20210825
//added by Mike, 20210818
//TO-DO: -put: this in Pilot.cpp, et cetera
SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(mySDLRenderer, filename);

	return texture;
}
*/

/* //removed by Mike, 20210825
//TO-DO: -reverify: draw refresh rate
//Reference: http://wiki.libsdl.org/SDL_RenderCopy;
//last accessed: 20210818
//TO-DO: -reverify: this
void draw(SDL_Texture *texture, int x, int y)
{
	//added by Mike, 20210818
	int iPilotWidth=64;
	int iPilotHeight=64;

	
  // Rectangles for drawing which will specify source (inside the texture)
  and target (on the screen) for rendering our textures.
  SDL_Rect SrcR;
  SDL_Rect DestR;
  
	iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;                    																				    

  SrcR.x = 0+ iCountTaoAnimationFrame*iPilotWidth;
  SrcR.y = 0;
  SrcR.w = iPilotWidth;
  SrcR.h = iPilotHeight;

  DestR.x = x; //myWindowWidthAsPixel / 2 - iPilotWidth / 2;
  DestR.y = y; //myWindowHeightAsPixel / 2 - iPilotHeight / 2;
  DestR.w = iPilotWidth;
  DestR.h = iPilotHeight;
  	
  int iCount;
  for (iCount=0; iCount<iNumOfKeyTypes; iCount++) {
		if (myKeysDown[iCount]==TRUE) {
 			iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;																				
 			break;
		}
  }
  if (iCount==iNumOfKeyTypes) {
 			iCountTaoAnimationFrame=0;																				
  }

	SDL_RenderClear(mySDLRenderer);
	
	////scaled down image
//	dest.w = iPilotWidth;
//	dest.h = iPilotHeight;

	SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
	SDL_RenderPresent(mySDLRenderer);
}
*/

void update() {
		if (myKeysDown[KEY_W])
		{
			iPilotY-=4;
		}

		if (myKeysDown[KEY_S])
		{
			iPilotY+=4;
		}

		if (myKeysDown[KEY_A])
		{
			iPilotX-=4;
		}

		if (myKeysDown[KEY_D])
		{
			iPilotX+=4;
		}
}

int main(int argc, char *argv[])
{
 	//removed by Mike, 20210818
//	memset(&App, 0, sizeof(App));

	initSDL();
	
	myOpenGLCanvas = new OpenGLCanvas();  
	//edited by Mike, 20210825
//  myOpenGLCanvas->init(myWindowWidthAsPixel,myWindowHeightAsPixel);
  myOpenGLCanvas->init(myWindowWidthAsPixel,myWindowHeightAsPixel,mySDLRenderer);

/* //removed by Mike, 20210825	
	//solution to problem: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
	SDL_Texture *texture = loadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png");

	iPilotX=myWindowWidthAsPixel/2;
	iPilotY=myWindowHeightAsPixel/2;
	
	//added by Mike, 20210819
	iCountTaoAnimationFrame=0;

 	//removed by Mike, 20210818
	//atexit(cleanup);

	while (1)
	{
		prepareScene();

		doInput();
		
		update();
				
		//added by Mike, 20210818
//		draw(player.texture, player.x, player.y);
		//edited by Mike, 20210819
//		draw(texture, myWindowWidthAsPixel/2, myWindowHeightAsPixel/2);
		draw(texture, iPilotX, iPilotY);

		presentScene();

		SDL_Delay(16);
	}
*/
	while (1) {
		prepareScene();
		doInput();
			
		myOpenGLCanvas->update();
		
/*	//edited by Mike, 20210826
		myOpenGLCanvas->render();
*/
		displayOpenGLCanvas();
		
		presentScene();
        
    //edited by Mike, 20210826
    //TO-DO: -add: auto-identify delay input based on computer processor speed
		SDL_Delay(16);
			//SDL_Delay(1);
    }

	return 0;
}

