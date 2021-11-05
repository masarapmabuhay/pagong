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
 * @date created: 20200930
 * @date updated: 20211106
 * @website address: http://www.usbong.ph
 *
 * Reference: 
 * 1) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL game programming". USA: Thomson Course Technology
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 * 
 */
 
//added by Mike, 20210728
//TO-DO: -update: Pilot does not enter gap of tile grid x1
//add: offset in tile grid to make inner rectangle for collision detection
 
//added by Mike, 20210130
//reverify: Robotshipship does not execute correctly
//when down and left buttons are pressed while firing beam down
//AND when up and left buttons are pressed while firing beam up		  
//in Windows Machine
//problem did not occur on Linux Machine (with external USB keyboard)
//added by Mike, 20210131
//note: add use of external USB keyboard solves the problem		  

//TO-DO: -update: this

//added by Mike, 20210201
//TO-DO: -delete: excess instructions
//TO-DO: increase lower right arm angle to go up y-axis

//added by Mike, 20210202
//TO-DO: -update: instructions when diagonal movement is combined with attack and defend keys


#include <stdio.h>
#include <math.h>

/*	//removed by Mike, 20201121
#include <gl/gl.h>
#include <gl/glu.h>
*/

//added by Mike, 20201121
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

//added by Mike, 20210821; edited by Mike, 20210826
//note: reverifying: use of SDL Image + OpenGL, without GLUT
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

//added by Mike, 20210820
//TO-DO: -reverify: http://www.nongnu.org/pngpp/doc/0.2.9/;
//last accessed: 20210820
//#include "png.hpp"
//#include <pngpp/png.hpp>
//removed by Mike, 20210821
//note: -reverify: using SDL and SDL Image + OpenGL; without GLUT
//to execute system development speed-up 
//#include <png.hpp>

#include "Robotship.h"
/* //TO-DO: -add: these
#include "PolygonUtils.h"
#include "ModelPool.h"
*/

//added by Mike, 20210523
#include "UsbongUtils.h"

/*	//removed by Mike, 20210925; TO-DO: -add: this
//added by Mike, 20210725
#include "Level2D.h"
*/

#include "Level3D.h"


#include <string.h>

//added by Mike, 20201217
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

//#include "ModelPool.h"

//added by Mike, 20210130
//TO-DO: -reverify: fire beam down, move left up not OK
//TO-DO: -reverify: fire beam up, move left down not OK

//edited by Mike, 20201014
//added by Mike, 20201001
enum Keys
{
/*	//edited by Mike, 20210128
	KEY_UP = 0,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
	//edited by Mike, 20201013
	KEY_ENTER,
	//TO-DO: -verify: gamepad
	KEY_A,
	KEY_D,
	KEY_W,
	KEY_S,
*/
	//TO-DO: -verify: gamepad
/*	//edited by Mike, 20210129
	KEY_A = 0,
	KEY_D,
	KEY_W,
	KEY_S,
*/
	KEY_W = 0,
	KEY_S,
	KEY_D,
	KEY_A,
/* //removed by Mike, 20210130
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_SPACE,
	KEY_ENTER,
*/
	KEY_J,
	KEY_L,
	KEY_I,
	KEY_K,
	KEY_H, //added by Mike, 20210111
	KEY_U, //added by Mike, 20210121

    //added by Mike, 20210603
    KEY_Z,
    
	//added by Mike, 20201226
	iNumOfKeyTypes
};

//object: Cube.005_0
//body
typedef struct
{
    GLubyte id_field_length;
    GLubyte color_map_type;
    GLubyte image_type_code;
    GLubyte ignore[9];
    GLushort width;
    GLushort height;
    GLubyte image_pixel_size;
    GLubyte image_descriptor;
} TARGA_HEADER;

//TO-DO: -put: in MyDynamicObject
GLboolean Robotship::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

/* //removed by Mike, 20211025
//added by Mike, 20210823
//Reference: https://discourse.libsdl.org/t/sdlsurface-to-opengl-texture/15597/2;
//last accessed: 20210823
//answer by: Holmes_Futrell, 2008-08
int pot(int x) {
int val = 1;
while (val < x) {
val *= 2;
}
return val;
}
*/

//added by Mike, 20210826
//TO-DO: -add: CAD tool to assist in identify excess markings in image file
//-add: CAD tool to verify animating sequence
//edited by Mike, 20210830
//void Robotship::openGLDrawTexture(int x, int y, GLuint textureId, int textw, int texth)
void Robotship::openGLDrawTexture(float x, float y, float textw, float texth)
{
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
	
/*	//removed by Mike, 20210901	
	float fTaoAnimationFrameOffset=0.0f;
	float fTaoAnimationFrameOffsetYAxis=0.0f;
*/
	
	//TO-DO: -update: Pilot shoes; animation sequence

	//added by Mike, 20210927
	//note: updated: texture and vertex positions to be clock-wise 1st quadrant
    //edited by Mike, 20211007; Pilot height divided by 2
	glTranslatef(-textw,-texth,0.0f);
//    glTranslatef(-textw,-texth-(texth/2),0.0f);

	//added by Mike, 20210918
	glColor3f(1.0f, 1.0f, 1.0f); // white

/* //removed by Mike, 20211012; TO-DO: -fix: in Level3D.cpp, overlap between tiles as cubes
    //added by Mike, 20211011
    //TO-DO: -reverify: スーパーマリオ３Dランド
    //TO-DO: -reverify: Pocky & Rocky
    glRotatef(45,1.0f,1.0f,0.0f);
*/
		//edited by Mike, 20210901
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
  
  			//removed by Mike, 20210927      
//        glTranslatef(textw,0.0f,0.0f);
  
/*
        //added by Mike, 20210925
        glRotatef(45,1.0f,0.0f,0.0f);
*/
			//set vertex counter-clock-wise
/* //edited by Mike, 20210927			
			glBegin(GL_QUADS);
				glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
				glVertex3f(x, y, 0);
				
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
//				glVertex3f(x + textw, y, 0);
				glVertex3f(x - textw, y, 0);
				
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
//				glVertex3f(x + textw, y + texth, 0);
				glVertex3f(x - textw, y + texth, 0);
				
				glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
//				glVertex3f(x, y + texth, 0);
				glVertex3f(x, y + texth, 0);
			glEnd();
*/
        

			//note: texture positions inverted
			//set vertex clock-wise
			//texture positions U shape, counter clock-wise
			glBegin(GL_QUADS);
				glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);				
				glVertex3f(x, y, 0);
				
				glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);				
				glVertex3f(x, y + texth, 0);

				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
				glVertex3f(x + textw, y + texth, 0);

				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glVertex3f(x + textw, y, 0);
			glEnd();			

/* //divide by 2 instructions causes turn from right to left to make Cartoon Pilot become big
        //note: texture positions inverted
        //set vertex clock-wise
        //texture positions U shape, counter clock-wise
        glBegin(GL_QUADS);
        glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
        glVertex3f(x, y, 0);
        
        glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
        glVertex3f(x, y + texth/2, 0);
        
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
        glVertex3f(x + textw/2, y + texth/2, 0);
        
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
        glVertex3f(x + textw/2, y, 0);
        glEnd();			
*/
    }
		else {		
/* //edited by Mike, 20210927		
			//added by Mike, 20210827
			//set vertex counter-clock-wise
			glBegin(GL_QUADS);
				glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
				glVertex3f(x, y, 0);
				
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
				glVertex3f(x + textw, y, 0);
				
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glVertex3f(x + textw, y + texth, 0);
				
				glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glVertex3f(x, y + texth, 0);
			glEnd();
*/
			//note: texture positions inverted
			//set vertex clock-wise
			//texture positions U shape, clock-wise			
			glBegin(GL_QUADS);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glVertex3f(x, y, 0);
				
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
				glVertex3f(x, y + texth, 0);

				glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);				
				glVertex3f(x + textw, y + texth, 0);

				glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);				
				glVertex3f(x + textw, y, 0);
			glEnd();
			
	}	
			
	glDisable(GL_TEXTURE_2D);
}

//added by Mike, 20210423
void Robotship::setup()
{
//    openGLLoadTexture(char *filename, int *textw, int *texth)((char*)"textures/imageSpriteExampleMikeWithoutBG.png", MIKE_TEXTURE_A);	
	//edited by Mike, 20210827
//	openGLITexture = openGLLoadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png", &iTextureWidth, &iTextureHeight);	
	//TO-DO: -update: myWidth and myHeight to quickly identify as float
    
    //edited by Mike, 20211014; TO-DO: -update: Robotship.cpp, et cetera; set width and height to be x2 of Pilot's...
//    openGLITexture = openGLLoadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png", myWidth, myHeight);
  
      openGLITexture = openGLLoadTexture((char*)"textures/imageSpriteExampleRoboWithoutBG.png", myWidth, myHeight);
}

//added by Mike, 20210522
//TO-DO: -identify cause of incorrect position in grid for player2

//edited by Mike, 20201001
//Robotship::RobotshipShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Robotship::RobotshipShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210815
//Robotship::Pilot(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Robotship::Robotship(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{ 
    //edited by Mike, 20201001
	//currentState=IN_TITLE_STATE;//MOVING_STATE;
	currentState=MOVING_STATE;
	//added by Mike, 20201201
	currentMovingState=IDLE_MOVING_STATE;
	
	//added by Mike, 20210611
	fTaoAnimationFrameOffsetYAxis=0.0f;
	
	fCountTotalFrames=4.0f; //added by Mike, 20210903
	
	//added by Mike, 20210810
	iShieldEffectCount=0;
	iGlowEffectCount=0;
	iGlowFadeEffectCount=0;
    
  invincibleCounter=0;
  currentDeathFrame=0;
  
	//added by Mike, 20210523
	//edited by Mike, 202105028
	myXPos=xPos;
	//edited by Mike, 20210827
  myYPos=yPos;//0.0f;
 
  myZPos=zPos;
  
	myXPosAsPixel=xPos;
	myYPosAsPixel=yPos;
	myZPosAsPixel=zPos;

	fMyWindowWidth=fWindowWidth;
	fMyWindowHeight=fWindowHeight;

/*	
	printf(">>Pilot.cpp: myWindowWidth: %i\n",myWindowWidth);
	printf(">>Pilot.cpp: myWindowHeight: %i\n",myWindowHeight);
*/	
  //added by Mike, 20210903
  fCountTotalFrames=4.0f;

  //TO-DO: -add: receive as input the values for the following to execute cascading values
  iRowCountMax=10;
  iColumnCountMax=18;
  iHeightCountMax=10;

/* //edited by Mike, 20210815    	
  fGridSquareWidth = myWindowWidth/iColumnCountMax; //example: 136.60
  fGridSquareHeight = myWindowHeight/iRowCountMax; //example: 76.80
*/	
  fGridSquareWidth = fMyWindowWidth/iColumnCountMax; //example: 136.60
  fGridSquareHeight = fMyWindowHeight/iRowCountMax; //example: 76.80
	
/*  //edited by Mike, 20210807; edited by Mike, 20210901
  	stepX=fGridSquareWidth/10;
    stepY=fGridSquareHeight/10;
    stepZ=fGridSquareWidth/10;
*/    
/*
    stepX=fGridSquareWidth/10/4;
    stepY=fGridSquareHeight/10/4;    
    stepZ=fGridSquareWidth/10/4;   
*/
/*
    //edited by Mike, 20210913
    stepX=fGridSquareWidth/10/2;
    stepY=fGridSquareHeight/10/2;    
    stepZ=fGridSquareWidth/10/2;   
*/

   //edited by Mike, 20210919
   //TO-DO: -reverify: cause of acceleration until half of window width
   //when moving forward to the RIGHT
    //note: excess +1 tile drawing and collision detection
    //after going to the half of window width, and then back to myCanvasPosX=0;
    //stepX/2.0 causes increase in correct output, albeit still re-occurs
    /*
     //edited by Mike, 20210919
     stepX=fGridSquareWidth/10/2;
     stepY=fGridSquareHeight/10/2;
     stepZ=fGridSquareWidth/10/2;
     */
    stepX=fGridSquareWidth/10;
    stepY=fGridSquareHeight/10;
    stepZ=fGridSquareWidth/10;

    
    //TO-DO: -reverify: gravity AND push down due to tile collision;
    //Pilot sinks in ground, e.g. 2-0 tile
    
    myWidthAsPixel=fGridSquareWidth;
    myHeightAsPixel=fGridSquareHeight;
		
		//added by Mike, 20210831
		//we NOW use floating-point numbers, instead of integers, i.e. whole numbers
 		//edited by Mike, 20211004
//edited by Mike, 20211024
    myWidth=fGridSquareWidth;
    myHeight=fGridSquareHeight;
/*
    myWidth=fGridSquareWidth*2;
    myHeight=fGridSquareHeight*2;
*/
    
    //edited by Mike 20211007
/*
    //note: shall need to update collision detection computation; TO-DO: -reverify: cause
    myWidth=fGridSquareWidth/2.0f;
    myHeight=fGridSquareHeight/2.0f;
*/
/*
    myWidth=myWidth*0.20f;
    myHeight=myHeight*0.35f;
    
    myWidth=myWidth/2.0f;
    myHeight=myHeight/2.0f;
*/

/*
    float fGridSquareWidthOffset=20.0f;
    float fGridSquareHeightOffset=20.0f;
    
    myWidth=fGridSquareWidth-fGridSquareWidthOffset;
    myHeight=fGridSquareHeight-fGridSquareHeightOffset;
*/
    
  //edited by Mike, 20210916; used with collision detection; edited by Mike, 20211007
/*
    iOffsetXPosAsPixel=fGridSquareWidth*0.28;
    iOffsetYPosAsPixel=fGridSquareHeight*0.15;
*/
    iOffsetXPosAsPixel=fGridSquareWidth*0.28*2.0f;
    iOffsetYPosAsPixel=fGridSquareHeight*0.15*4.0f; //note: divide Pilot height by 2.0f
    

/*  //note: if NOT set, CAN climb diagonal triangle tile; 
    //however, empty tile x1, Pilot does not fall through...
 
		//TO-DO: -reverify: offset due to integer, NOT float classification
    iOffsetXPosAsPixel=fGridSquareWidth*0.28*2.5;
    //edited by Mike, 20210917; //TO-DO: -reverify: this
    iOffsetYPosAsPixel=fGridSquareHeight*0.15;
//    iOffsetYPosAsPixel=fGridSquareHeight*0.05;
 //   iOffsetYPosAsPixel=0;
*/
    
    //added by Mike, 20211106
    //note: set this in OpenGLCanvas.cpp
    bIsPlayer1=false;
    bIsPlayer1Partner=false;
    bIsPlayer2=false;
    bIsPlayer2Partner=false;
    
	//added by Mike, 20210728
	bHasHitWall=false;
	
	//added by Mike, 20210523
	myUsbongUtils = new UsbongUtils();
	//added by Mike, 20210726; edited by Mike, 20210727
//  myUsbongUtils->setWindowWidthHeight(windowWidth, windowHeight);
	//edited by Mike, 20210815
//  	myUsbongUtils->setWindowWidthHeight(myWindowWidth, myWindowHeight);
	  	myUsbongUtils->setWindowWidthHeight(fMyWindowWidth, fMyWindowHeight);
    
//    myWidthX=0.5;

    rotationAngle=0.0f;//360.0f;//90.0;
    rotationStep=10.0;//1.0f;    
    thrust=0.0f;
    thrustMax=0.8f;
/* //removed by Mike, 20210826
    xVel;
    yVel;
*/
    //edited by Mike, 20201001
    maxXVel=0.04f;//1.0f;
    maxYVel=0.04f;//1.0f;

    //boundary = 8.0f;

    tricount = 120;
    isMovingForward = 0;

	//added by Mike, 20210122
	iPunchAnimationCountDelay=0;
	//added by Mike, 20210123
	iPunchAnimationCount=0;

    //added by Mike, 20210613
    bIsExecutingKick=false;
    iKickAnimationCount=0;
    iKickAnimationCountDelay=0;

    //added by Mike, 20210805
    bIsExecutingPunch=false;
    bIsExecutingDefend=false;

    
    //init default values
    //previousFacingState=FACING_UP;
    //currentFacingState=FACING_UP;

	//added by Mike, 20201225
	bIsFiringBeam=false;
	
	//added by Mike, 20210126
    bIsExecutingDash=false, //removed by Mike, 20210128; added again by Mike, 20210805

    bIsDashReady=false;
	//edited by Mike, 20210128
//	iInputWaitCount=0;
	
	//added by Mike, 20210202
	for (int iCount=0; iCount<iNumOfKeyTypes; iCount++) {
		myKeysDown[iCount]=false;
	}	
	
	//added by Mike, 20210128
	for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		bIsExecutingDashArray[iCount]=false;
		
		//added by Mike, 20210128
		iInputWaitCountArray[iCount]=0;
	}
	
    //edited by Mike, 20201201; edited by Mike, 20210502
//	currentFacingState=FACING_UP;
	currentFacingState=FACING_RIGHT;
    //added by Mike, 20210729
    prevFacingState=FACING_RIGHT;

	armAngles[LEFT] = 0.0;
	armAngles[RIGHT] = 0.0;
	legAngles[LEFT] = 0.0;
	legAngles[RIGHT] = 0.0;

	armStates[LEFT] = FORWARD_STATE;
	armStates[RIGHT] = BACKWARD_STATE;

	legStates[LEFT] = FORWARD_STATE;
	legStates[RIGHT] = BACKWARD_STATE;

	//removed by Mike, 20210423
/*	loadTexture(myBodyTexture, "bodyTexture.tga", &myBodyTextureObject);
	loadTexture(myHeadTexture, "headTexture.tga", &myHeadTextureObject);	
*/
	
	//added by Mike, 20210502; edited by Mike, 20210601
	//note: set this in OpenGLCanvas.cpp	
	bIsPlayer1=false;
	bIsPlayer1Partner=false;
	bIsPlayer2=false;
	bIsPlayer2Partner=false;	
	
	
	//removed by Mike, 20201001; added by Mike, 20210423
	setup();
	
  setCollidable(true);    
}

Robotship::~Robotship()
{
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Robotship::getXYZPos()
{
      //float myXYZ[3];
      float* myXYZ;
      myXYZ = (float*) malloc(3*sizeof(float));

      myXYZ[0]=myXPos;
      myXYZ[1]=myYPos;
      myXYZ[2]=myZPos;
      
      return myXYZ;
}

/* //removed by Mike, 20201217
//added by Mike, 20201213
void Robotship::draw()
{
	drawRobotshipShip();
}
*/


//added by Mike, 20210423
void Robotship::drawPilotAsQuadWithTexture()
{
    //removed by Mike, 20210912
//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), 0);
	
    switch (currentState)
    {
/*
           case MOVING_STATE://DYING_STATE:
                glColor3f(1.0f, 1.0f, 1.0f); //white
                if (currentDeathFrame<5) {
                    switch(currentDeathFrame)
                    {
                      case 0:
                        drawMyPlane(myDeathAnimationImg1);
                        break;
                      case 1:
                        drawMyPlane(myDeathAnimationImg2);
                        break;
                      case 2:
                        drawMyPlane(myDeathAnimationImg3);
                        break;
                      case 3:
                        drawMyPlane(myDeathAnimationImg4);
                        break;
                      case 4:
                        drawMyPlane(myDeathAnimationImg5);
                        //changeState(MOVING_STATE);
                        break;
                    }
                }
                currentDeathFrame=(currentDeathFrame+1)%4;
                break;
*/
/* //removed by Mike, 20201014
           case INITIALIZING_STATE:
                if (invincibleCounter==10) {
                  changeState(MOVING_STATE);
                  setCollidable(true);
                }
                else invincibleCounter++;
*/
				
            case MOVING_STATE:
							switch(currentMovingState) {
		            		 case IDLE_MOVING_STATE:
							//added by Mike, 20210420
							//TO-DO: -add: this in key movement
							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
							iCountTaoAnimationFrame=3;
                        						//edited by Mike, 20210609
						//                        fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.125; //0.25;
                        						fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
                        							//removed by Mike, 20210610
						//                        iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;
												
												//added by Mike, 20210611
												//TO-DO: -add: identify which Unit Member hit the Ball
												//TO-DO: -add: image with kick
												//edited by Mike, 20210901
//              					fTaoAnimationFrameOffsetYAxis=0.75f; //added by Mike, 20210611
              					fTaoAnimationFrameOffsetYAxis=0.0f;
	
													//added by Mike, 20210809
//												drawPilotObjectGlow();
																								
												//added by Mike, 20210810; //removed by Mike, 20210815
//												drawPilotObjectGlowFade();
												//with GlowFadePart1(), SFIII's GIL?
//												drawPilotObjectGlowFadePart1(); //added by Mike, 20210811
												
												//FACING_UP...
												drawPilotObject();
												
												//added by Mike, 20210809
//		     								drawShieldEffectAsQuadWithTexture();

												//added by Mike, 20210811
												//star power?; after drawPilotObject()
//												drawPilotObjectGlow();

												break;
											case WALKING_MOVING_STATE:
							//added by Mike, 20210420
							//TO-DO: -add: this in key movement
							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
							iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;
							
						//	fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.5;
                        						//edited by Mike, 20210609
						//                        fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.125; //0.25;
                        						fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
                       						
							
						//	printf("iCountTaoAnimationFrame: %i",iCountTaoAnimationFrame);
							iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;		
							//printf("iTaoAnimationFrameOffset: %i",iTaoAnimationFrameOffset);

												//edited by Mike, 20210901																		
//              					fTaoAnimationFrameOffsetYAxis=0.75f; //added by Mike, 20210611
              					fTaoAnimationFrameOffsetYAxis=0.0f; //added by Mike, 20210611
																	                                    
                                    //added by Mike, 20210809
//                                    drawPilotObjectGlow();

												//added by Mike, 20210810; removed by Mike, 20210815
//												drawPilotObjectGlowFade();
												//with GlowFadePart1(), SFIII's GIL?												
												//drawPilotObjectGlowFadePart1(); //added by Mike, 20210811
                                    
												drawPilotObject();
												
												//added by Mike, 20210811
												//star power?; after drawPilotObject()												
//												drawPilotObjectGlow();
												
												
												//added by Mike, 20210805; edited by Mike, 20210805
												//drawAccelerationEffectAsQuadWithTexture();
                                    if (prevFacingState==currentFacingState) {
//                                        if (bIsExecutingDash) {
/*                                        if (bIsExecutingDashArray[KEY_D] ||
                                            bIsExecutingDashArray[KEY_A]) {
*/
                                        if (bIsExecutingDash) {
                                        }
                                        else {
                                            if (bIsExecutingDashArray[KEY_D] ||
                                                bIsExecutingDashArray[KEY_A]) {
                                                bIsExecutingDash=true;
                                            }

                                            if (bIsExecutingDash) {
                                                drawAccelerationEffectAsQuadWithTexture();
                                            }
                                        }
                                    }
                                    else {
                                        drawAccelerationEffectAsQuadWithTexture();
                                    }                                    
												break;
												
												case ATTACKING_MOVING_STATE:
												
printf(">>>>>>>>>>>>>>> ATTACK DITO");												
              							//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
              							iCountTaoAnimationFrame=0;
              							fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;
              							
														//edited by Mike, 20210901																		              							
//              							fTaoAnimationFrameOffsetYAxis=0.25f; //added by Mike, 20210611
              							fTaoAnimationFrameOffsetYAxis=0.0f; //added by Mike, 20210611
              							
							/*											//removed by Mike, 20210611
              							iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;
							*/                       
              							drawPilotObject(); //added by Mike, 20210603
              							break;
													}
                							break;
            						case IN_TITLE_STATE:
               						break;						
    						}    

	//removed by Mike, 20201001
//    glPopMatrix();	// pop back to original coordinate system
}

//added by Mike, 20210827
void Robotship::drawPilotObject()
{
	//added by Mike, 20210826
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //removed by Mike, 20210829
		glLoadIdentity(); //removed by Mike, 20210925
    
    glDisable(GL_CULL_FACE);

		//added by Mike, 20210927
//		glTranslatef(1.0f, -1.0f, 0.0f);

//    glRotatef(180, 0.0f, 0.0f, 1.0f); //flip vertically
    
    //edited by Mike, 20210830
//	openGLDrawTexture(myXPosAsPixel, myYPosAsPixel, openGLITexture, myWidth, myHeight);
    
/* //TO-DO: -reverify: this    
    glTranslatef(1.0f, 
							 	 1.0f, 
							   0.0f);
*/

/*
		//TO-DO: -add: in openGLDrawTexture(...)
		glTranslatef(-2.0f, 
							 	 2.0f, 
							   0.0f);

    glRotatef(180, 0.0f, 0.0f, 1.0f); //flip vertically
    glRotatef(180, 1.0f, 0.0f, 0.0f);		
//    glRotatef(180, 0.0f, 1.0f, 0.0f); //note: update: axis movement
*/  
  
  	//edited by Mike, 20210927  
//	  glScalef(0.5f, 0.5f, 0.5f);
//    glScalef(0.75f, 0.75f, 0.75f);
//    glScalef(1.0f, 1.0f, 1.0f); 
		 //use correct width x height ratio; window 10x18; row x column
	   //glScalef(0.25f, 0.25f, 0.25f);
	   //removed by Mike, 20210927
//	   glScalef(0.20f, 0.35f, 0.0f);
	   
	   //added by Mike, 20210927
	  //TO-DO: -reverify: pixel to vertex position computation
	  
//	  printf(">>myXPos: %f; vertexPosX: %f\n",myXPos,myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos));
	  

    //TO-DO: -reuse: in OpenGLCanvas.cpp, et cetera, 
    //myUsbongUtils->autoConvertFromPixelToVertexPointX(...) with position computations    
/*	//edited by Mike, 20210927    
    openGLDrawTexture(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos), 
    									myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos), 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth), 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight));
*/

    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos), 0);
		
		//added by Mike, 20210927
		//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
		//use correct width x height ratio; window 10x18; row x column
		glScalef(0.20f, 0.35f, 0.0f);

		//added by Mike, 20211007
		//note: shall need to update collision detection computation; TO-DO: -reverify: cause		
//		glScalef(0.50f, 0.50f, 0.0f);
/*
    //edited by Mike, 20211007
    openGLDrawTexture(0.0f, 
    									0.0f, 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth), 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight));
*/
    //edited by Mike, 20211024
    //note: if myWidth/2, instead of myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth)/2
    //no change; TO-DO: -reverify: cause
    openGLDrawTexture(0.0f,
                      0.0f,
                      myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth)/2,
                      myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight)/2);

/*
    //added by Mike, 20211024
    //TO-DO: -add: town, e.g. Marikina City
    //TO-DO: -add: island ruins, e.g. Crash Bandicoot?
    //TO-DO: -add: Robotship techniques, e.g. night vision, thermal vision, speed, strength, armor against cold, heat (natural disasters), wireless wave...
    //TO-DO: -add: shields as defense, 合気道（aikidou）?
    //TO-DO: -add: Robotship has no gun weapon
    //TO-DO: -add: kalaban, e.g. teroristang komunista (who misuse wireless broadcasting technology to cause physical injury)
    //TO-DO: -add: multiplayer network; Final Fantasy Adventure Gameboy version PLUS?

    //removed by Mike, 20211024; increase width and height due to increased strength? Crysis?
    //note: use instructions with Robotship.cpp; Robotship with talking Kaluluwa... Magic Knight Rayearth?
    openGLDrawTexture(0.0f,
                      0.0f,
                      myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth),
                      myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight));
*/
}

//added by Mike, 20210727
//TO-DO: -reverify: this
void Robotship::drawPilotObjectPrev()
{
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    //added by Mike, 20210809;
    //edited by Mike, 20210811
    //note: remove: this to cause blank output in color white, et cetera
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    
//    glColor3f(1.0f, 1.0f, 0.0f); //yellow
//    glColor3f(0.0f, 0.0f, 0.0f); //black; removes additional color
//    glColor3f(1.0f, 0.0f, 0.0f); //red

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); ///home/unit_member/Documents/USBONG/story-mainexample: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720    
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
/*  //edited by Mike, 20210824; 
		//due to rotation?    
//from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;
*/
    float fTileSideYAxis = 0.0625f;

		//added by Mike, 20210724
		//background color of tile
//-----
/*
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/

/*	//removed by Mike, 20210727
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	
//-----		

		//added by Mike, 20210727
		//due flipped vertically
		//removed by Mike, 20210824
//		glRotatef(180, 0.0f, 0.0f, 1.0f);		
				
    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
/* //edited by Mike, 20210824
			//note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
*/    	

//TO-DO: -update: this
fTaoAnimationFrameOffset=0;

float fTextureWidth = 256.0f/4.0f; //surface->w/4;
float fTextureHeight = 256.0f/4.0f; //surface->h/4;

float fTaoAnimationFrameOffset=0.0f;
float fTaoAnimationFrameOffsetYAxis=0.0f;

/*
 glBegin(GL_QUADS);
	glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
//	glVertex3f(x, y, 0);
	glVertex3f(0, 0, 0);
	
	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
//	glVertex3f(x + textw, y, 0);
	glVertex3f(0 + ftextureWidth, 0, 0);
	
	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
//	glVertex3f(x + textw, y + texth, 0);
	glVertex3f(0 + ftextureWidth, 0 + ftextureHeight, 0);

	
	glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
//	glVertex3f(x, y + texth, 0);
	glVertex3f(0, 0 + ftextureHeight, 0);	
glEnd();
*/
 glBegin(GL_QUADS);
	glTexCoord2f(0+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
//	glVertex3f(x, y, 0);
//	glVertex3f(0, 0, 0);
	glVertex3f(myXPosAsPixel, myYPosAsPixel, 0);
	
	
//	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, 0+fTaoAnimationFrameOffsetYAxis);
	glTexCoord2f(1.0f, 0+fTaoAnimationFrameOffsetYAxis);
//	glVertex3f(x + textw, y, 0);
//	glVertex3f(0 + fTextureWidth, 0, 0);
	glVertex3f(myXPosAsPixel + fTextureWidth, myYPosAsPixel, 0);
	
	//glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
	glTexCoord2f(1.0f, 1.0f);

//	glVertex3f(x + textw, y + texth, 0);
//	glVertex3f(0 + fTextureWidth, 0 + fTextureHeight, 0);
	glVertex3f(myXPosAsPixel + fTextureWidth, myYPosAsPixel + fTextureHeight, 0);	
	
//	glTexCoord2f(0+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
	glTexCoord2f(0.0f, 1.0f);
	
//	glVertex3f(x, y + texth, 0);
//	glVertex3f(0, 0 + fTextureHeight, 0);	
	glVertex3f(myXPosAsPixel, myYPosAsPixel + fTextureHeight, 0);	

glEnd();

   	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();    
}

//added by Mike, 20210809; edited by Mike, 20210810
void Robotship::drawPilotObjectGlow()
{
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
//    glColor3f(1.0f, 0.67f, 0.0f); //gold
//    glColor3f(0.0f, 0.0f, 0.0f); //black
		//edited by Mike, 20210810
//    glColor3f(1.0f, 0.0f, 0.0f); //red

		iGlowEffectCount=(iGlowEffectCount+1)%2;
		if (iGlowEffectCount==0) {
    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //set to default, i.e. red
		}
		else {
    	glColor4f(1.0f, 0.33f, 0.0f, 0.60f); //orange; 60% transparent
		}


    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//background color of tile
//-----
/*
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/

/*	//removed by Mike, 20210727
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	
//-----		
		
		//added by Mike, 20210809
        glScalef(1.08f, 1.08f, 1.0f);

		//added by Mike, 20210727
		//due flipped vertically
		glRotatef(180, 0.0f, 0.0f, 1.0f);		

        //added by Mike, 20210809
        //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        glTranslatef(0.004f, 0.008f, 0.0f);

    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {

        
        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

            //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210810; edited by Mike, 20210811
void Robotship::drawPilotObjectGlowFadePart1()
{
    //added by Mike, 20210810
    // Disable AutoTexture Coordinates
/*    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
*/
    
//    glEnable(GL_TEXTURE_2D);                    // Enable 2D Texture Mapping
//    glDisable(GL_DEPTH_TEST);                   // Disable Depth Testing
/*
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);               // Set Blending Mode
    glEnable(GL_BLEND);                     // Enable Blending
*/
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
		
		int iGlowFadeEffectCountMax=10;

//      glScalef(1.08f, 1.08f, 1.0f);
		
        //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
        //last accessed: 20210810
        //TO-DO: -reverify: this
		for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {		
			//from red to orange to yellow
//    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
//    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
            //edited by Mike, 20210810
            glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
            //from yellow to orange to red
//            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
            
			if (iGlowFadeEffectCount==0) {
    		glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);

      	glScalef(1.08f, 1.08f, 1.0f);
    	}
    					
			//edited by Mike, 20210810
//      glScalef(1.08f, 1.08f, 1.0f);
//      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
//      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
      glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
		
			if (iGlowFadeEffectCount==0) {
				//due flipped vertically
				glRotatef(180, 0.0f, 0.0f, 1.0f);			
			}

      //added by Mike, 20210809
      //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
//TO-DO: -reverify: this
      glTranslatef(0.004f, 0.008f, 0.0f);
      glTranslatef(-0.0032f, -0.0072f, 0.0f);

/*
      glTranslatef(0.004f*iGlowFadeEffectCount, 0.008f*iGlowFadeEffectCount, 0.0f);
      glTranslatef(-0.0032f*iGlowFadeEffectCount, -0.0072f*iGlowFadeEffectCount, 0.0f);
*/

//removed by Mike, 20210810
//		}

    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

    //note: vertex position sequence to be auto-drawn
    //vertex positions sequence: counter-clockwise sequence to auto-draw front face		
    //edited by Mike, 20210729
//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

        //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();
    }
    //edited by Mike, 20210729
//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {

        
        //added by Mike, 20210809
//        glTranslatef(0.004f, 0.008f, 0.0f);

            //note: vertex positions sequence: counter-clock-wise
			//note:texture positions sequence: counter-clock-wise
    	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    		//glTexCoord2f(fTx, fTy);
//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);               	            
    		glVertex3f(0.0f, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);   
        glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);	
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    		
    		//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
    		glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    		
    		//glTexCoord2f(fTx, fTy + fTileSideYAxis);
      	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
				glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
    		glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    	glEnd();    	
		}
		//TO-DO: -add: facing_up, facing_down
		else {
		}

    //added by Mike, 20210810
    }
            
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210811
void Robotship::drawPilotObjectGlowFade()
{
    //added by Mike, 20210810
    // Disable AutoTexture Coordinates
    /*    glDisable(GL_TEXTURE_GEN_S);
     glDisable(GL_TEXTURE_GEN_T);
     */
    
    //    glEnable(GL_TEXTURE_2D);                    // Enable 2D Texture Mapping
    //    glDisable(GL_DEPTH_TEST);                   // Disable Depth Testing
    /*
     glBlendFunc(GL_SRC_ALPHA,GL_ONE);               // Set Blending Mode
     glEnable(GL_BLEND);                     // Enable Blending
     */
    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
    //    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    glPushMatrix();
    /*
     printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
     printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
     */
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    int iGlowFadeEffectCountMax=5;//3;//5; //10;
    
    //      glScalef(1.08f, 1.08f, 1.0f);
    
    //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
    //last accessed: 20210810
    //TO-DO: -reverify: this
    
		for (int iCount=0; iCount<4; iCount++) {
		    
//            glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iCount*0.2f);
            
    	for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {
        	//from red to orange to yellow
        	//    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
        	//    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
        	//edited by Mike, 20210810
	//        glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
	//        glColor4f(1.0f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
	//        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            //edited by Mike, 20210812
//        	glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            //from orange to red; TO-DO: -reverify: due to iCount does not start from inside
            if (iCount<2) {
                glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            }
            else {
                glColor4f(1.0f, 0.3f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
            }
	
        	
        	//from yellow to orange to red
        	//            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        	
        	if ((iGlowFadeEffectCount==0) and (iCount==0)){
            	glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);            	
            	glScalef(1.08f, 1.08f, 1.0f);
        	}
        	
        	//edited by Mike, 20210810
        	//      glScalef(1.08f, 1.08f, 1.0f);
        	//      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
        	//      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
        	glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
        	
        	if ((iGlowFadeEffectCount==0) and (iCount==0)) {
            	//due flipped vertically
            	glRotatef(180, 0.0f, 0.0f, 1.0f);
        	}
        	
		  		        	
        	//added by Mike, 20210809
        	//note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        	//TO-DO: -reverify: this
        	//edited by Mike, 20210811
	//        glTranslatef(0.004f, 0.008f, 0.0f);
	//      glTranslatef(0.002f, 0.002f, 0.0f);
	//        glTranslatef(0.002f, -0.008f, 0.0f);
        	
        	int iDirection=1;

        	if (iCount%2!=0) {
        		iDirection=iDirection*(-1);
        	}
        	
//        	glTranslatef(0.002f*iDirection, 0.0f, 0.0f);

        	//glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
//					if ((iGlowFadeEffectCount==0) and (iCount==0)){

            if (iGlowFadeEffectCount==0){
//        		glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
        		glTranslatef(0.002f*iDirection, 0.002f, 0.0f);
            }

//            glTranslatef(0.002f*iDirection*iGlowFadeEffectCount, 0.002f*iDirection*iGlowFadeEffectCount, 0.0f);


        	//even number
        	if (iGlowFadeEffectCount%2==0) {
	//            glTranslatef(0.002f, -0.008f, 0.0f);
                //edited by Mike, 20210812
//            	glTranslatef(0.002f*iDirection, 0.002f*(iDirection*(-1)), 0.0f);
                glTranslatef((0.002f*(iCount+1))*iDirection, (0.002f*(iCount+1))*(iDirection*(-1)), 0.0f);
        	}
        	//NOT even number
        	else {
	//            glTranslatef(0.002f, 0.008f, 0.0f);
                //edited by Mike, 20210812
//            	glTranslatef(0.002f*iDirection, -0.002f*(iDirection*(-1)), 0.0f);
                glTranslatef((0.002f*(iCount+1))*iDirection, (-0.002f*(iCount+1))*(iDirection*(-1)), 0.0f);
            }

            
        	//removed by Mike, 20210811
	//        glTranslatef(-0.0032f, -0.0072f, 0.0f);
	
        	
        	/*
         	glTranslatef(0.004f*iGlowFadeEffectCount, 0.008f*iGlowFadeEffectCount, 0.0f);
         	glTranslatef(-0.0032f*iGlowFadeEffectCount, -0.0072f*iGlowFadeEffectCount, 0.0f);
         	*/
        	
        	//removed by Mike, 20210810
        	//		}
        	
        	float fTx = 0.0f;
        	float fTy = 0.0f;
        	
        	float fTileSideXAxis = 0.0625f;
        	//from bottom; anchor; start fTy at 1.0f
        	float fTileSideYAxis = -0.0625f;
        	
        	//note: vertex position sequence to be auto-drawn
        	//vertex positions sequence: counter-clockwise sequence to auto-draw front face
        	//edited by Mike, 20210729
        	//    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        	if ((currentFacingState==FACING_RIGHT) ||
            	(((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
             	(prevFacingState==FACING_RIGHT))
            	|| (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
            	
            	//added by Mike, 20210809
            	//        glTranslatef(0.004f, 0.008f, 0.0f);
            	//added by Mike, 20210811
            	//flash blur effect; shadow?
	//            glTranslatef(0.0000032f*iGlowFadeEffectCount, 0.0000000072f*iGlowFadeEffectCount, 0.0f);
	//            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.000001f*iGlowFadeEffectCount, 0.0f);
	//            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.0f, 0.0f);
            	
            	
            	//note: vertex positions sequence: counter-clock-wise
            	//note:texture positions sequence: clock-wise
            	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            	//glTexCoord2f(fTx, fTy);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	
            	//glTexCoord2f(fTx, fTy + fTileSideYAxis);
            	glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
            	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	glEnd();
        	}
        	//edited by Mike, 20210729
        	//    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
        	else if ((currentFacingState==FACING_LEFT) ||
                 	(((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
                  	(prevFacingState==FACING_LEFT))
                 	|| (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
            	
            	
            	//added by Mike, 20210809
            	//        glTranslatef(0.004f, 0.008f, 0.0f);
            	
            	//note: vertex positions sequence: counter-clock-wise
            	//note:texture positions sequence: counter-clock-wise
            	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            	//glTexCoord2f(fTx, fTy);
            	//				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy);
            	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            	
            	//glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            	//glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	
            	//glTexCoord2f(fTx, fTy + fTileSideYAxis);
            	//glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            	glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);     	
            	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            	glEnd();    	
        	}
        	//TO-DO: -add: facing_up, facing_down
        	else {
        	}
        	
        	//added by Mike, 20210810
    	}
    }
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}


//added by Mike, 20210811; removed by Mike, 20210811
//TO-DO: -reverify: glow fade computer instructions
/*
void Robotship::drawPilotObjectGlowFade()
{
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //added by Mike, 20210809
    //https://stackoverflow.com/questions/4361023/opengl-es-1-1-how-to-change-texture-color-without-losing-luminance;
    //last accessed: 20210809T1402
    //answer by: Ozirus, 20101206T2236
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
    //    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    glPushMatrix();
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    int iGlowFadeEffectCountMax=5; //10;
    
    //      glScalef(1.08f, 1.08f, 1.0f);
    
    //reference: http://nehe.gamedev.net/tutorial/radial_blur__rendering_to_a_texture/18004/;
    //last accessed: 20210810
    //TO-DO: -reverify: this
    for (iGlowFadeEffectCount=0; iGlowFadeEffectCount<iGlowFadeEffectCountMax; iGlowFadeEffectCount++) {
        //from red to orange to yellow
        //    	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //red
        //    	glColor4f(1.0f, 0.0825f*iGlowFadeEffectCount, 0.0f, 1.0f);
        //edited by Mike, 20210810
        //        glColor4f(1.0f, 1.0f-0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        //        glColor4f(1.0f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        //        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        glColor4f(0.8f, 0.0f, 0.0f, 1.0f-iGlowFadeEffectCount*0.2f);
        
        
        //from yellow to orange to red
        //            glColor4f(1.0f, 0.1f*iGlowFadeEffectCount, 0.0f, 1.0f-iGlowFadeEffectCount*0.1f);
        
        if (iGlowFadeEffectCount==0) {
            glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
            
            glScalef(1.08f, 1.08f, 1.0f);
        }
        
        //edited by Mike, 20210810
        //      glScalef(1.08f, 1.08f, 1.0f);
        //      glScalef(1.08f*(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f)), 1.08f, 1.0f);
        //      glScalef(1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f+(iGlowFadeEffectCount/(iGlowFadeEffectCountMax*1.0f))/10, 1.0f);
        glScalef(1.0f+(iGlowFadeEffectCount*0.001f), 1.0f+(iGlowFadeEffectCount*0.001f), 1.0f);
        
        if (iGlowFadeEffectCount==0) {
            //due flipped vertically
            glRotatef(180, 0.0f, 0.0f, 1.0f);
        }
        
        //added by Mike, 20210809
        //note: FACING LEFT OR RIGHT, graphics library translate COMMAND equal
        //TO-DO: -reverify: this
        //edited by Mike, 20210811
        //        glTranslatef(0.004f, 0.008f, 0.0f);
        //      glTranslatef(0.002f, 0.002f, 0.0f);
        //        glTranslatef(0.002f, -0.008f, 0.0f);
        glTranslatef(0.002f, 0.0f, 0.0f);
        
        //even number
        if (iGlowFadeEffectCount%2==0) {
            //            glTranslatef(0.002f, -0.008f, 0.0f);
            glTranslatef(0.002f, 0.002f, 0.0f);
        }
        //NOT even number
        else {
            //            glTranslatef(0.002f, 0.008f, 0.0f);
            glTranslatef(0.002f, -0.002f, 0.0f);
        }
        
        //removed by Mike, 20210811
        //        glTranslatef(-0.0032f, -0.0072f, 0.0f);
        
        //removed by Mike, 20210810
        //		}
        
        float fTx = 0.0f;
        float fTy = 0.0f;
        
        float fTileSideXAxis = 0.0625f;
        //from bottom; anchor; start fTy at 1.0f
        float fTileSideYAxis = -0.0625f;
        
        //note: vertex position sequence to be auto-drawn
        //vertex positions sequence: counter-clockwise sequence to auto-draw front face
        //edited by Mike, 20210729
        //    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        if ((currentFacingState==FACING_RIGHT) ||
            (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
             (prevFacingState==FACING_RIGHT))
            || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
            
            //added by Mike, 20210809
            //        glTranslatef(0.004f, 0.008f, 0.0f);
            //added by Mike, 20210811
            //flash blur effect; shadow?
            //            glTranslatef(0.0000032f*iGlowFadeEffectCount, 0.0000000072f*iGlowFadeEffectCount, 0.0f);
            //            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.000001f*iGlowFadeEffectCount, 0.0f);
            //            glTranslatef(0.000001f*iGlowFadeEffectCount, 0.0f, 0.0f);
            
            
            //note: vertex positions sequence: counter-clock-wise
            //note:texture positions sequence: clock-wise
            glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            //glTexCoord2f(fTx, fTy);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            
            //glTexCoord2f(fTx, fTy + fTileSideYAxis);
            glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f); //0.5f);
            glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            glEnd();
        }
        //edited by Mike, 20210729
        //    else if ((currentFacingState==FACING_LEFT) || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
        else if ((currentFacingState==FACING_LEFT) ||
                 (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
                  (prevFacingState==FACING_LEFT))
                 || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
            
            
            //added by Mike, 20210809
            //        glTranslatef(0.004f, 0.008f, 0.0f);
            
            //note: vertex positions sequence: counter-clock-wise
            //note:texture positions sequence: counter-clock-wise
            glBegin(GL_QUADS); // Each set of 4 vertices form a quad
            //glTexCoord2f(fTx, fTy);
            //				glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy);
            //glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
            
            //glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
            //glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glTexCoord2f(0.25f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            
            //glTexCoord2f(fTx, fTy + fTileSideYAxis);
            //glTexCoord2f(0.25+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glTexCoord2f(0.0f+fTaoAnimationFrameOffset, fTaoAnimationFrameOffsetYAxis+0.25f);
            glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
            glEnd();
        }
        //TO-DO: -add: facing_up, facing_down
        else {
        }
        
        //added by Mike, 20210810
    }
    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}
*/

//added by Mike, 20210805
void Robotship::drawAccelerationEffectAsQuadWithTexture()
{
/*	//removed by Mike, 20210805    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
*/    
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
/* //removed by Mike, 20211025
    printf("drawAccelerationEffectAsQuadWithTexture\n");
*/
    
    //TO-DO: -update: this
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -

/*		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
*/
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

 		//put at back of Pilot's feet
/* 		
    if ((currentFacingState==FACING_RIGHT) ||
        (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
            (prevFacingState==FACING_RIGHT))
        || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {

    }
    else if ((currentFacingState==FACING_LEFT) ||
             (((currentFacingState==FACING_UP) || (currentFacingState==FACING_DOWN)) and
              (prevFacingState==FACING_LEFT))
             || (currentFacingState==FACING_LEFT_AND_UP) || (currentFacingState==FACING_LEFT_AND_DOWN)) {
             
         glTranslatef(0.0f+fGridTileWidthVertexPosition*2, 0.0f, 0.0f);
		}		
		else if (currentFacingState==FACING_UP) {
		}
		else if (currentFacingState==FACING_DOWN) {
		}
*/

    //added by Mike, 20210724
    //background color of tile
    //-----
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //    glColor3f(1.0f, 0.0f, 0.0f); //red

    if (currentFacingState==FACING_RIGHT) {
        //edited by Mike, 20211007; divided Pilot Height by 2.0f
//        glTranslatef(0.0f-fGridTileWidthVertexPosition*4.0, 0.0f+fGridTileHeightVertexPosition*0.5f, 0.0f);
        glTranslatef(0.0f-fGridTileWidthVertexPosition*2.5f, 0.0f+fGridTileHeightVertexPosition*0.4f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        //edited by Mike, 20211007; divided Pilot Height by 2.0f
//        glTranslatef(0.0f-fGridTileWidthVertexPosition*3.0, 0.0f+fGridTileHeightVertexPosition*0.5f, 0.0f);
        glTranslatef(0.0f-fGridTileWidthVertexPosition*2.0f, 0.0f+fGridTileHeightVertexPosition*0.4f, 0.0f);
    }
    
    //Reference: https://stackoverflow.com/questions/46258919/drawing-circle-with-gl-polygon-radius-out-of-scale;
    //question by: emic, 20170916T2216;
    //edited by: Rabbid76, 20180531T1930
    //answer by: Rabbid76, 20170917T0735
    
    //TO-DO: -reverify: this due to drawn shape ellipse, not circle
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    //glOrtho( -myWindowWidth/2.0f, myWindowWidth/2.0f, -myWindowHeight/2.0f, myWindowHeight/2.0f, -1.0, 1.0 );
    //note: -reverify: anchor; facing right at center, facing left at back; trailing
  
/* //edited by Mike, 20210815    
    printf("myWindowWidth: %i\n",myWindowWidth); //example output: 1366
    printf("myWindowHeight: %i\n",myWindowHeight); //example output: 768
*/    
    //		glScalef(0.56f, 1.0f, 1.0f);
    /* //removed by Mike, 20211025
    printf("fMyWindowWidth: %f\n",fMyWindowWidth); //example output: 1366
    printf("fMyWindowHeight: %f\n",fMyWindowHeight); //example output: 768
*/
    
    //TO-DO: -add: auto-identify if computer monitor rectangle, i.e. NOT square;
    //AND which side is longer
    //edited by Mike, 20210815
//    glScalef(myWindowHeight/(myWindowWidth*1.0f), 1.0f, 1.0f);
		//removed by Mike, 20211006
//    glScalef(fMyWindowHeight/(fMyWindowWidth*1.0f), 1.0f, 1.0f);
    
//--

    //TO-DO: -add: in loop, increasing size AND movement
for (int iCount=0; iCount<3; iCount++) {
    if (currentFacingState==FACING_RIGHT) {
        //edited by Mike, 20211007
//        glTranslatef(0.0f-fGridTileWidthVertexPosition/2.0f, 0.0f, 0.0f);
        glTranslatef(0.0f-fGridTileWidthVertexPosition/2.0f/2.0f, 0.0f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        //edited by Mike, 20211007
//        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f, 0.0f);
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f/2.0f, 0.0f, 0.0f);
    }
	else {
	  	glPopMatrix();
	  	return;
	}    
/*    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	

        //added by Mike, 20210805
        //glScalef(1.0f*(iCount+1), 1.0f*(iCount+1), 1.0f);
        //glScalef(0.5f*(iCount+1), 0.5f*(iCount+1), 1.0f);
        //edited by Mike, 20211006
//        glScalef(0.6f*(iCount+1), 0.6f*(iCount+1), 1.0f);
      //edited by Mike, 20211006		
//        glScalef(1.0f*(iCount+1), 1.0f*(iCount+1), 1.0f);
			//edited by Mike, 20211006
//        glScalef(1.0f+1.0f/(iCount+1), 1.0f+1.0f/(iCount+1), 1.0f);
        glScalef(1.2f+1.0f/(iCount+1), 1.2f+1.0f/(iCount+1), 1.0f);
    
		float fCircleCenterX=0.0f;
		float fCircleCenterY=0.0f;
		float fPI=3.14f;
			//edited by Mike, 20211006
//          float fRadius=0.02f;//0.25f; //0.1f; //0.25f;
            //edited Mike, 20211007
//          float fRadius=0.03f;
    float fRadius=0.03f/2.0f;
				
		//draw ellipse
    glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * fPI; i += fPI / 24) {
            glVertex3f((cos(i) * fRadius) + fCircleCenterX,(sin(i) * fRadius) + fCircleCenterY, 0.0);
        }
    glEnd();   	
   			
		//reset scaled shape
		glScalef(1.0f, 1.0f, 1.0f);  	
}
//-----		


/*	//removed by Mike, 20210805    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/
    
    glPopMatrix();
}

//added by Mike, 20210809
//sonic shield?
void Robotship::drawShieldEffectAsQuadWithTexture()
{
/*	//removed by Mike, 20210805    
    glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);
    glEnable(GL_TEXTURE_2D);
*/    
    //added by Mike, 20210803
//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
		//note: alpha last input parameter
//    glColor4f(0.0f, 1.0f, 0.85f, 0.80f); //set to default, i.e. green + blue; 80% transparent
		//note: fully charged to be ON; red charging; from Android
		//reminder: additional computation due to transparent effect
    glColor4f(0.18f, 1.0f, 0.16f, 0.60f); //set to default, i.e. lime green; 60% transparent

		iShieldEffectCount=(iShieldEffectCount+1)%2;
		if (iShieldEffectCount==0) {
			return;
		}



    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
    printf("drawAccelerationEffectAsQuadWithTexture\n");
    
    //TO-DO: -update: this
/*    
    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000
*/
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -

/*		
		//added by Mike, 20210727        
    glTranslatef(0.0f, -fGridTileHeightVertexPosition, 0.0f);
*/
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

    //added by Mike, 20210724
    //background color of tile
    //-----
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    //added by Mike, 20210809
    glTranslatef(0.0f, 0.0f+fGridTileHeightVertexPosition/3.0f, 0.0f);

    //added by Mike, 20210805
    if (currentFacingState==FACING_RIGHT) {
        //added by Mike, 20210805
        //note: remove when drawing quad, instead of circle
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
        //        glTranslatef(0.0f-fGridTileWidthVertexPosition/2.0f, 0.0f, 0.0f);
        
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        //note: circle radius 0.25f; OK; reminder: anchor top-left
        //glTranslatef(0.0f+fGridTileWidthVertexPosition*2, 0.0f, 0.0f);
        //note: y-axis:  0.0f-fGridTileHeightVertexPosition/2.0f at middle
//        glTranslatef(0.0f+fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
        glTranslatef(0.0f+fGridTileWidthVertexPosition/2.0f, 0.0f-fGridTileHeightVertexPosition/1.1f, 0.0f);
    }
    
    //Reference: https://stackoverflow.com/questions/46258919/drawing-circle-with-gl-polygon-radius-out-of-scale;
    //question by: emic, 20170916T2216;
    //edited by: Rabbid76, 20180531T1930
    //answer by: Rabbid76, 20170917T0735
    
    //TO-DO: -reverify: this due to drawn shape ellipse, not circle
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    //glOrtho( -myWindowWidth/2.0f, myWindowWidth/2.0f, -myWindowHeight/2.0f, myWindowHeight/2.0f, -1.0, 1.0 );
    //note: -reverify: anchor; facing right at center, facing left at back; trailing
/*    
    printf("myWindowWidth: %i\n",myWindowWidth); //example output: 1366
    printf("myWindowHeight: %i\n",myWindowHeight); //example output: 768
    //		glScalef(0.56f, 1.0f, 1.0f);
*/
    
    //TO-DO: -add: auto-identify if computer monitor rectangle, i.e. NOT square;
    //AND which side is longer
    //edited by Mike, 20210815
//    glScalef(myWindowHeight/(myWindowWidth*1.0f), 1.0f, 1.0f);
    glScalef(fMyWindowHeight/(fMyWindowWidth*1.0f), 1.0f, 1.0f);
    
//--
/*
    //TO-DO: -add: in loop, increasing size AND movement
for (int iCount=0; iCount<3; iCount++) {
    
    if (currentFacingState==FACING_RIGHT) {
        glTranslatef(0.0f-fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
    else if (currentFacingState==FACING_LEFT) {
        glTranslatef(0.0f+fGridTileWidthVertexPosition/4.0f, 0.0f, 0.0f);
    }
	else {
	  	glPopMatrix();
	  	return;
	}
*/
    
/*    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
*/   	

        //added by Mike, 20210805
        //glScalef(1.0f*(iCount+1), 1.0f*(iCount+1), 1.0f);
        //glScalef(0.5f*(iCount+1), 0.5f*(iCount+1), 1.0f);
/*        glScalef(0.6f*(iCount+1), 0.6f*(iCount+1), 1.0f);
*/    
		float fCircleCenterX=0.0f;
		float fCircleCenterY=0.0f;
		float fPI=3.14f;
    float fRadius=0.2f; //0.25f; //0.02f;//0.25f; //0.1f; //0.25f;
				
		//draw ellipse
    glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * fPI; i += fPI / 24) {
            glVertex3f((cos(i) * fRadius) + fCircleCenterX,(sin(i) * fRadius) + fCircleCenterY, 0.0);
        }
    glEnd();   	
   			
		//reset scaled shape
		glScalef(1.0f, 1.0f, 1.0f);		
/*   	
}*/
//-----		


/*	//removed by Mike, 20210805    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
*/
    
    glPopMatrix();
}


/*
//added by Mike, 20210805
//Tile Block delivery trailer?
void Robotship::drawAccelerationEffectAsQuadWithTextureTileBlock()
{
    //added by Mike, 20210803
    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white

    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625

    glPushMatrix();
    
    printf("drawAccelerationEffectAsQuadWithTexture\n");
    
    //TO-DO: -update: this
    
//    printf(">>Pilot.cpp; fGridSquareWidth: %f",fGridSquareWidth); //example: 71.111115
//    printf(">>Pilot.cpp; fGridSquareHeight: %f",fGridSquareHeight); //example: 80.000000

    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
//    printf(">>>fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
 
    //get positive value
    if (fGridTileWidthVertexPosition<0) {
        fGridTileWidthVertexPosition=fGridTileWidthVertexPosition*(-1);
    }    
    if (fGridTileHeightVertexPosition<0) {
        fGridTileHeightVertexPosition=fGridTileHeightVertexPosition*(-1);
    }
    
    //  	printf (">>> fGridTileWidthVertexPosition: %f; fGridTileHeightVertexPosition: %f",fGridTileWidthVertexPosition,fGridTileHeightVertexPosition);
    
    //added by Mike, 20210715
    //note: add this set of instructions due to excess border visible
    //verified: screen/monitor width and height to be OK
    //10/3 = 3.3333... cause of problem?
    //added by Mike, 20210717
    //note: partly border visible occurs in Linux machine;
    //due to instructions that I wrote
    
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //added by Mike, 20210720
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -
    
    float fTx = 0.0f; 
    float fTy = 0.0f;
    
    float fTileSideXAxis = 0.0625f;
    //from bottom; anchor; start fTy at 1.0f
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//background color of tile

//    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    glColor3f(1.0f, 0.0f, 0.0f); //red
    
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
   	
//-----		

    glPopMatrix();
}
*/

//added: by Mike, 20210423
//TO-DO: -add: in PolygonPool
//note: image textured quad as 3D object bends as its positions go outward from center
//edited by Mike, 20210523
void Robotship::drawPilotObjectWith3DVertexPositions()
{	
	//added by Mike, 20210422	
	glPushMatrix();
		//added by Mike, 20210420
		glColor3f(1.0f, 1.0f, 1.0f); // white
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, MIKE_TEXTURE_A);		

	//	glTranslatef(30.0f, 0.0f, 25.0f); //removed by Mike, 20210424
		glRotatef(180, 1.0f, 0.0f, 0.0f);

		//added by Mike, 20210522
		//TO-DO: -reverify: cause of blurry image texture output
		//added by Mike, 20210523
		//TO-DO: -reverify: Pilot movement, e.g. horizontally, vertically, due to not exact with grid coordinate
	
		//added by Mike, 20210422; edited by Mike, 20210424
		//TO-DO: -reverify: quad size and texture size
	//    glScalef(1.5f, 1.5f, 1.0f);	
		//edited by Mike, 20210521
	//  glScalef(1.2f, 2.0f, 1.0f);	
		//edited by Mike, 20210522
        //edited by Mike, 20210523
//    glScalef(5.0f, 5.0f, 1.0f);
//        glScalef(10.0f, 10.0f, 1.0f);
    glScalef(3.2f, 3.2f, 3.2f);
    
	//    glScalef(10.f, 10.0f, 1.0f);	
//		glScalef(20.0f, 20.0f, 1.0f);	

	/*	//removed by Mike, 20210423	
		//edited by Mike, 20210420
		//note: 3 animation frames; .tga image file has 4 frames @128x256, i.e. width x height
	//	iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%2;
		iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;

	//	fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.5;
		fTaoAnimationFrameOffset=iCountTaoAnimationFrame*0.25;

		printf("iCountTaoAnimationFrame: %i",iCountTaoAnimationFrame);

		iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;		
		//printf("iTaoAnimationFrameOffset: %i",iTaoAnimationFrameOffset);
	*/	


		//added by Mike, 20210424
	/*    if (currentFacingState==FACING_LEFT) 
		{  
		} 
		else */
	//edited by Mike, 20210425
	/*	if (currentFacingState==FACING_RIGHT) {    
	*/
	//TO-DO: -update: this
    
    //added by Mike, 20210523
    //TO-DO: -fix: problem with objects, e.g. Pilot, bend if position not at center
    /* //edited by Mike, 20210523
     //note: vertex coordinate positions to create quad using triangles not exact with input image file causes blurry output
     //image textured quad scale size also causes blurry output if set to be too small
    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
			//added by Mike, 20210424
			//notes: use folding paper to assist in quickly identifying location, e.g. texture coordinates 
			//set vertex positions clock-wise
	//      glRotatef(45, 0.0f, 1.0f, 0.0f); //slanted to wall facing left
			glBegin(GL_TRIANGLES);	
				//triangle#6 //back face left part
				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1; face left
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2		
				glVertex3f(-1.000000,1.000000,-1.000000); //A1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424		
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C2
				glVertex3f(1.000000,-1.000000,-1.000000); //B1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424				
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1; face left
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1 
				glVertex3f(-1.000000,-1.000000,-1.000000); //C1	

				//triangle#12 //back face right part		
				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A2; face lefT
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2		
				glVertex3f(-1.000000,1.000000,-1.000000); //A2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1
				glVertex3f(1.000000,1.000000,-1.000000); //B2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210424
	//			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //C2; face left
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1	
				glVertex3f(1.000000,-1.000000,-1.000000); //C2	
			glEnd();
		}
		else {	
			glBegin(GL_TRIANGLES);	
				//triangle#6 //back face left part
				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	
				glVertex3f(-1.000000,1.000000,-1.000000); //A1

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);
				glVertex3f(1.000000,-1.000000,-1.000000); //B1

				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	
				glVertex3f(-1.000000,-1.000000,-1.000000); //C1	


				//triangle#12 //back face right part		
				glNormal3f(0.0000,0.0000,-1.0000);
				glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	
				glVertex3f(-1.000000,1.000000,-1.000000); //A2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420	
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,0.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,0.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0);
				glVertex3f(1.000000,1.000000,-1.000000); //B2

				glNormal3f(0.0000,0.0000,-1.0000);
				//edited by Mike, 20210420	
			//	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
			//	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
				glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);	
				glVertex3f(1.000000,-1.000000,-1.000000); //C2	
			glEnd();
		}	
*/
    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_RIGHT_AND_UP) || (currentFacingState==FACING_RIGHT_AND_DOWN)) {
        //added by Mike, 20210424
        //notes: use folding paper to assist in quickly identifying location, e.g. texture coordinates
        //set vertex positions clock-wise
        //      glRotatef(45, 0.0f, 1.0f, 0.0f); //slanted to wall facing left
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C2
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1; face left
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //B1
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A2; face lefT
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2
        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0); //B2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);	//A1
        glVertex3f(1.000000,4.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210424
        //			glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0); //C2; face left
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);	//C1
        glVertex3f(1.000000,-1.000000,-1.000000); //C2
        glEnd();
    }
    else {
        glBegin(GL_TRIANGLES);
        //triangle#6 //back face left part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
        glVertex3f(-1.000000,4.000000,-1.000000); //A1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);
        glVertex3f(1.000000,-1.000000,-1.000000); //B1
        
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,1.0);
        glVertex3f(-1.000000,-1.000000,-1.000000); //C1
        
        
        //triangle#12 //back face right part
        glNormal3f(0.0000,0.0000,-1.0000);
        glTexCoord2f(0.0+fTaoAnimationFrameOffset,0.0);
        glVertex3f(-1.000000,4.000000,-1.000000); //A2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,0.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,0.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,0.0);
        glVertex3f(1.000000,4.000000,-1.000000); //B2
        
        glNormal3f(0.0000,0.0000,-1.0000);
        //edited by Mike, 20210420	
        //	glTexCoord2f(1.0+iTaoAnimationFrameOffset,1.0);
        //	glTexCoord2f(0.5+fTaoAnimationFrameOffset,1.0);
        glTexCoord2f(0.25+fTaoAnimationFrameOffset,1.0);	
        glVertex3f(1.000000,-1.000000,-1.000000); //C2	
        glEnd();
    }
    
    
		//added by Mike, 20210422
		glScalef(1.0f, 1.0f, 1.0f);	

		glRotatef(-180, 1.0f, 0.0f, 0.0f);
	//	glTranslatef(-30.0f, 0.0f, -25.0f); //removed by Mike, 20210424

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix(); //added by Mike, 20210422	
}

//added by Mike, 20210423; edited by Mike, 20210807
//TO-DO: -update: this
void Robotship::update(float dt)
{

    switch (currentState)
    {
           case INITIALIZING_STATE:
           case MOVING_STATE:   
           //removed by Mike, 20210807
/*              
				switch(currentMovingState) {
		           case WALKING_MOVING_STATE:
                        //added by Mike, 20210613
                        bIsExecutingKick=false;
                        iKickAnimationCount=0;
                        iKickAnimationCountDelay=0;
		                break;
		            case ATTACKING_MOVING_STATE:
                        //added by Mike, 20210613
                        if (bIsExecutingKick) {
                            if (iKickAnimationCount<MAX_KICKING_ANIMATION_COUNT) {
                                if ((iKickAnimationCountDelay)%2==0) {
                                    iKickAnimationCount+=1;
                                    iKickAnimationCountDelay=0;
                                }
                                iKickAnimationCountDelay+=1;
                            }
                            //added by Mike, 20210613
                            //+added: no continuous kick via hold kick button
                            else {
                                if (iKickAnimationCountDelay<0) { //<5
                                }
                                else {
                        //added by Mike, 20210613
                        //TO-DO: -fix: no need to release pressed KEY_K
                                    
                        //removed by Mike, 20210613
//                                    if (myKeysDown[KEY_K]==FALSE) {
                                        bIsExecutingKick=false;
                                        iKickAnimationCount=0;
                                        iKickAnimationCountDelay=0;
  //removed by Mike, 20210613
////                                        armAngles[RIGHT]=0.0f;
////                                        armAngles[LEFT]=0.0f;

                                    
//removed by Mike, 20210613
//                                    }
                                }
                                iKickAnimationCountDelay+=1;
                            }
                        }
                        
                        
		            	if (bIsExecutingPunch) {
		            		if (iPunchAnimationCount<MAX_PUNCHING_ANIMATION_COUNT) {
								if ((iPunchAnimationCountDelay)%2==0) {
									iPunchAnimationCount+=1;
									iPunchAnimationCountDelay=0;
								}
								iPunchAnimationCountDelay+=1;
							}
							//added by Mike, 20210123
							//+added: no continuous punch via hold punch button
							else {
								//edited by Mike, 20210123; edited again by Mike, 20210124
								if (iPunchAnimationCountDelay<0) { //<5
								}
								else {
									//edited by Mike, 20210123
		    						if (myKeysDown[KEY_U]==FALSE) {  
										bIsExecutingPunch=false;
										iPunchAnimationCount=0;
										iPunchAnimationCountDelay=0;

										//added by Mike, 20210124
								   		armAngles[RIGHT]=0.0f;
										armAngles[LEFT]=0.0f;
									} 
								}
								iPunchAnimationCountDelay+=1;
							}
						}
						
						if (bIsExecutingDefend) {
    						if (myKeysDown[KEY_H]==FALSE) {  
								bIsExecutingDefend=false;

								//added by Mike, 20210124
						   		armAngles[RIGHT]=0.0f;
								armAngles[LEFT]=0.0f;
							} 
						}						
		            	break;
		                
		            default: //STANDING STATE		            
		              break;//do nothing    
				}
*/

				if (myKeysDown[KEY_D]==FALSE) {
					if (iInputWaitCountArray[KEY_D]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_D]+=1;
					}
				}
				if (myKeysDown[KEY_A]==FALSE) {
					if (iInputWaitCountArray[KEY_A]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_A]+=1;
					}
				}
/*	//removed by Mike, 20210812				
				if (myKeysDown[KEY_W]==FALSE) {
					if (iInputWaitCountArray[KEY_W]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_W]+=1;
					}
				}
				if (myKeysDown[KEY_S]==FALSE) {
					if (iInputWaitCountArray[KEY_S]<MAX_WAIT_COUNT) {
						iInputWaitCountArray[KEY_S]+=1;
					}
				}
*/

           		rotationAngle=0; //TO-DO: -update: this
				break;
            case IN_TITLE_STATE:                
                  rotationAngle+=5;//rotationStep;
                break;
            default: //STANDING STATE
              break;//do nothing    
    }
}

void Robotship::changeState(int s)
{
  currentState=s;                  
}

//added by Mike, 20201226
void Robotship::keyDownSideScrollView(int keyCode) {
	myKeysDown[keyCode] = TRUE;
	
	//added by Mike, 20210812
	myKeysDown[KEY_W] = FALSE;
	myKeysDown[KEY_S] = FALSE;

/* //removed by Mike, 20210917; TO-DO: -add: this
	//added by Mike, 20210127; edited by Mike, 20210128
	autoVerifyDashStateWithKeyDown();//keyCode);
*/	
}


//added by Mike, 20201226
void Robotship::keyDown(int keyCode) {
	myKeysDown[keyCode] = TRUE;
	
/* //removed by Mike, 20210921	
	//added by Mike, 20210812
	myKeysDown[KEY_W] = FALSE;
	myKeysDown[KEY_S] = FALSE;
*/	

/* //removed by Mike, 20210917; TO-DO: -add: this
	//added by Mike, 20210127; edited by Mike, 20210128
	autoVerifyDashStateWithKeyDown();//keyCode);
*/	
}

//added by Mike, 20201227; edited by Mike, 20210128
//void Robotship::setDashStateWithKeyDown() {
void Robotship::setDashStateWithKeyDown(int keyCode) {
	if (bIsDashReady==true) {
/*		//edited by Mike, 20210128
		if (iInputWaitCount<MAX_WAIT_COUNT) {
			//edited by Mike, 20210128
			//bIsExecutingDash=true;
			bIsExecutingDashArray[keyCode]=true;			
		}
*/		
		if (iInputWaitCountArray[keyCode]<MAX_WAIT_COUNT) {
			//edited by Mike, 20210129
//			bIsExecutingDashArray[keyCode]=true;			
			//verify if any directional key already executes dash
			for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
				if (bIsExecutingDashArray[keyCode]) {
					return;
				}							
			}

			bIsExecutingDashArray[keyCode]=true;
		}
	}
}

//added by Mike, 20210521
//TO-DO: -reverify: dash for KEY's A and D

//added by Mike, 20201226; edited by Mike, 20210128
//void Robotship::autoVerifyDashStateWithKeyDown(int keyCode) {
void Robotship::autoVerifyDashStateWithKeyDown() { //int keyCode) {
	//edited by Mike, 20210128
	//if (myKeysDown[KEY_RIGHT]==TRUE) {
	//edited by Mike, 20210130
//	if ((myKeysDown[KEY_RIGHT]==TRUE) || (myKeysDown[KEY_D]==TRUE)) {		
	if (myKeysDown[KEY_D]==TRUE) {
		setDashStateWithKeyDown(KEY_D);
	}
	//edited by Mike, 20210130
//	else if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
/*	//removed by Mike, 20210812				
	else if (myKeysDown[KEY_W]==TRUE) {
		setDashStateWithKeyDown(KEY_W);//KEY_UP);
	}
*/	
	else if (myKeysDown[KEY_A]==TRUE) {
		setDashStateWithKeyDown(KEY_A);
	}
/*	//removed by Mike, 20210812					
	else if (myKeysDown[KEY_S]==TRUE) {
		setDashStateWithKeyDown(KEY_S);
	}
*/	
}

void Robotship::keyUp(int keyCode) {
	//added by Mike, 20210127
	autoVerifyDashStateWithKeyUp(keyCode);

	myKeysDown[keyCode] = FALSE;	
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
void Robotship::setDashStateWithKeyUp() {
	if (bIsExecutingDash) {
		//edited by Mike, 20210128
//		bIsExecutingDash=false;
		for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
			bIsExecutingDashArray[iCount]=false;			
		}

		bIsDashReady=false;			
	}
	else {
		bIsDashReady=true;			
		iInputWaitCount=0;
	}
}
*/
//added by Mike, 20210127; edited by Mike, 20210129
//void Robotship::setDashStateWithKeyUp() {
void Robotship::setDashStateWithKeyUp(int keyCode) {
	//edited by Mike, 20210128
	bool bIsExecutingDash=false;

	//added by Mike, 20210202
	//if removed, dash persists
	for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		if (bIsExecutingDashArray[iCount]) {
			bIsExecutingDash=true;
			break;
		}			
	}	

	if (bIsExecutingDash) {
		//if (bIsExecutingDashArray[KEY_RIGHT]) {
		//edited by Mike, 20210130
//		if ((bIsExecutingDashArray[KEY_RIGHT]) || (bIsExecutingDashArray[KEY_D])) {		
		if (bIsExecutingDashArray[KEY_D]) {
			bIsExecutingDashArray[KEY_D]=false;
			bIsDashReady=false;
		}
		//edited by Mike, 20210130
//		else if ((bIsExecutingDashArray[KEY_UP]) || (bIsExecutingDashArray[KEY_W])) {
/*	//removed by Mike, 20210812				
		if (bIsExecutingDashArray[KEY_W]) {
			bIsExecutingDashArray[KEY_W]=false;//KEY_UP);
			bIsDashReady=false;
		}
*/		
		else if (bIsExecutingDashArray[KEY_A]) {
			bIsExecutingDashArray[KEY_A]=false;
			bIsDashReady=false;
		}
/*	//removed by Mike, 20210812						
		else if (bIsExecutingDashArray[KEY_S]) {
			bIsExecutingDashArray[KEY_S]=false;
			bIsDashReady=false;
		}
*/		
	}
	else {
		bIsDashReady=true;			
		//edited by Mike, 20210128		
//		iInputWaitCount=0;

		//edited by Mike, 20210129
/*		for (int iCountKey=0; iCountKey<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCountKey++) {
			iInputWaitCountArray[iCountKey]=0;
		}		
*/		
		iInputWaitCountArray[keyCode]=0;
	}
	
}

//added by Mike, 20210127
void Robotship::autoVerifyDashStateWithKeyUp(int keyCode) {
	//added by Mike, 20210126; edited by Mike, 20210128
//	if (keyCode==KEY_RIGHT) {
	//edited by Mike, 20210130
//	if ((keyCode==KEY_RIGHT) || (keyCode==KEY_D)) {
	if (keyCode==KEY_D) {
		//edited by Mike, 20210128
//		if (myKeysDown[KEY_RIGHT]==TRUE) {
		if (myKeysDown[KEY_D]==TRUE) {
			//edited by Mike, 20210129myZPosAsPixel
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_D);
		}
	}
	//edited by Mike, 20210130
//	else if ((keyCode==KEY_UP) || (keyCode==KEY_W)) {
/*	//removed by Mike, 20210812				
	else if (keyCode==KEY_W) {
		//edited by Mike, 20210130
//		if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
		if (myKeysDown[KEY_W]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_W);
		}
	}
*/	
	else if (keyCode==KEY_A) {
		if (myKeysDown[KEY_A]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_A);
		}
	}
/*	//removed by Mike, 20210812					
	else if (keyCode==KEY_S) {
		if (myKeysDown[KEY_S]==TRUE) {
			//edited by Mike, 20210129
//			setDashStateWithKeyUp();
			setDashStateWithKeyUp(KEY_S);
		}
	}	
*/	
	//removed by Mike, 20210128
/*
	else {
		//edited by Mike, 20210128
//		bIsExecutingDash=false;
		for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
			bIsExecutingDashArray[iCount]=false;			
		}
		
		bIsDashReady=false;			
	}
*/	
}


void Robotship::move(int key)
{
   //Note: Unit member as Pilot has to release hold of directional keys,
   //so that RobotshipShip faces in the correct direction;
   //holding the fire beam key, e.g. KEY_I, causes RobotshipShip 
   //to move without changing the direction that it faces
	
   //added by Mike, 20201201; removed by Mike, 20201226
//   currentMovingState=WALKING_MOVING_STATE;

   //added by Mike, 20201225; removed by Mike, 20201225
   //bIsFiringBeam=false;
   
   //TO-DO: -add: bIsMovingLeft, etc, while facing up, etc

   //added by Mike, 20201226; removed by Mike, 2myZPosAsPixel0201226
//   myKeysDown[key] = TRUE;	

	//removed by Mike, 20210203
/*	//added by Mike, 20210203
	if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_LEFT_AND_UP;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_RIGHT_AND_UP;
	}
	//added by Mike, 20210202
	else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_LEFT_AND_DOWN;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_RIGHT_AND_DOWN;
	}
*/

   switch (key)
   {
     //added by Mike, 20201218
     case KEY_I:
     	//removed by Mike, 20201225
/*   		  
		  currentMovingState=ATTACKING_MOVING_STATE;		   
          currentFacingState=FACING_UP;
*/          
          //added by Mike, 20201225
          bIsFiringBeam=true;
          
          //added by Mike, 20201226
          //iNumOfKeyTypes
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
          //TO-DO: +reverified: Robotshipship does not execute correctly
		  //when down and left buttons are pressed while firing beam down
		  //AND when up and left buttons are pressed while firing beam up		  
		  //in Windows Machine
		  //problem did not occur on Linux Machine (with external USB keyboard)			
		  //added by Mike, 20210131
		  //note: add use of external USB keyboard solves the problem		  

//          for (int iCount=0; iCount<10; iCount++) {
		   //edited by Mike, 20210130
		   //note: in RobotshipShip.h set PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT=4, not 6,
		   //causes RobotshipShip movement error
		   //edited by Mike, 20210202
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
//          for (int iCount=0; iCount<4; iCount++) {
			  if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
		  }          
          break;
     //added by Mike, 20201226
     case -KEY_I:
//		  currentMovingState=IDLE_MOVING_STATE;
          bIsFiringBeam=false;
          //removed by Mike, 20201226
//          bHasPressedADirectionalKey=false; //added by Mike, 20201226

   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;	

     //added by Mike, 2021011
     //TO-DO: -update: this to be defend using shield
     case KEY_H:
     		 //added by Mike, 20210809; removed by Mike, 20210809
//		     drawShieldEffectAsQuadWithTexture();
     
/* //removed by Mike, 20210809; executes kick Command     
          bIsExecutingDefend=true;
          
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
   		    if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }         
*/ 
          break;
     case -KEY_H:
/*          //removed by Mike, 20210124
			bIsExecutingDefend=false;
*/
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;
	
	//added by Mike, 20210121
    case KEY_U:
		  //removed by Mike, 20210123
//          bIsExecutingPunch=true;
		            
		  //added by Mike, 20210122; edited by Mike, 202101213
		  if (iPunchAnimationCount==0){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {
            bIsExecutingPunch=true;
		  }
		             
          bHasPressedADirectionalKey=false;

          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		   	if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }

		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }          
          break;
     case -KEY_U:
    	//TO-DO: -reverify: arm angles after release of punch button and then press move down
     	
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;

//	 case KEY_UP: //removed by Mike, 20210130
     case KEY_W:
       //added by Mike, 20210111
       if (bIsExecutingPunch) {
	   }
       //added by Mike, 20210121
	   else if (bIsExecutingDefend) {
	   }
       //added by Mike, 20210613
       else if (bIsExecutingKick) {
       }
       else {
//added by Mike, 20210521
//----------
         //added by Mike, 20210725
         if (myLevel3D->isLevel2DCollideWith(this)) {
//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
             return;
        	//		break;
         }
         else {         
         			//added by Mike, 20210728
         			//edited by Mike, 20210729; TO-DO: -update: this
         		  if ((bHasHitWall) and (getCurrentFacingState()==FACING_UP)) {
//         		  if ((bHasHitWall)) {
         		  	return;
         		  }

/* //removed by Mike, 20210921         		           		  
         		    //edited by Mike, 20210805
                //in macOS machine, Pilot does not move up due to gravity;
                //OK in LUBUNTU machine; JUMP height low; exerting effort to move up, but pulled by heavy force
  
  						 //edited by Mike, 20210806           
//             iStepYCountMax=20;
             iStepYCountMax=10;

             if (iStepYCount>=iStepYCountMax) {
             			//added by Mike, 20210806;
             			//removed by Mike, 20210806;
             			//Yosshi- Dinosaur?
//             		return;

            			currentMovingState=IDLE_MOVING_STATE;
              		currentFacingState=FACING_UP;
                 
                 break;
             }
             else {
             		//edited by Mike, 20210830
//                myYPosAsPixel+=(-stepY*1.1);                
								//edited by Mike, 20210831
                myYPos+=(-stepY*1.1);
                //myYPos+=(stepY*1.1);
                iStepYCount+=1;
            
             }
*/
/* //edited by Mike, 20211025
								 if (getIsPlayer1()) { //Player1: Unit Chief
            					myYPos+=-stepY;
         					}
         					else {
            					myYPos+=-stepY;
         					}
*/
            					myYPos+=-stepY;


		 				bHasHitWall=false;

		 		}
			//added by Mike, 20210521		
//----------		
       }
	
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_UP;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
					//added by Mike, 20210728; edited by Mike, 20210729
					//TO-DO: -update: this
              //added by Mike, 20210729; removed by Mike, 20210729;
              //yoko scroll
//              prevFacingState=currentFacingState;
              
              currentFacingState=FACING_UP;
		  }
		  
          currentMovingState=WALKING_MOVING_STATE;
          break;

		   //edited by Mike, 20210812; removed by Mike, 20210923
//           return;
//           break;

 //     case KEY_DOWN:  //removed by Mike, 20210130
     case KEY_S: //added by Mike, 20210128
				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    			//added by Mike, 20210613
    			else if (bIsExecutingKick) {
    			}
				else {
         			//added by Mike, 20210725
         			if (myLevel3D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//										currentMovingState=IDLE_MOVING_STATE;
										bHasHitWall=true;
        						return;
         			}
         			else {				
					  			//added by Mike, 20210728
         					if ((bHasHitWall) and (getCurrentFacingState()==FACING_DOWN)) {
         		  			return;
         					}

/* //edited by Mike, 20211025         					
        					if (getIsPlayer1()) { //Player1: Unit Chief
             					//edited by Mike, 20210830
            					//myYPosAsPixel+=stepY;        					
//            					myYPos+=stepY;
										//edited by Mike, 20210831
            					myYPos+=stepY; 
//										myYPos+=(-stepY);
            					
        					}
        					else {
             					//edited by Mike, 20210830
            					//myYPosAsPixel+=stepY/2;
										//edited by Mike, 20210831
            					myYPos+=stepY/2; 
//										myYPos+=(-stepY/2);
            									
        					}
*/
            					myYPos+=stepY; 
       					
                                }

 				if ((bIsExecutingDashArray[KEY_S])) {
             			//edited by Mike, 20210830
//            			myYPosAsPixel+=stepY*2;
						//edited by Mike, 20210831
						myYPos+=stepY*2;
//						myYPos+=(-(stepY*2));             			
					}
							bHasHitWall=false;
				}
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_DOWN;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; removed by Mike, 20210729
              //yoko scroll
//              prevFacingState=currentFacingState;

              //added by Mike, 20210728
              currentFacingState=FACING_DOWN;
		  }

           currentMovingState=WALKING_MOVING_STATE;
          break;
        //added by Mike, 20210806; removed by Mike, 20210923
//           return;
//			break;
			
			
//     case KEY_LEFT: //removed by Mike, 20210130
     case KEY_A: //added by Mike, 20210128		   
     		//removed by Mike, 20201001
//          rotationAngle+=rotationStep;
/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=-0.1f;
*/
					//added by Mike, 20210111
					if (bIsExecutingPunch) {
					}
					//added by Mike, 20210121
					else if (bIsExecutingDefend) {
					}
    				//added by Mike, 20210613
    				else if (bIsExecutingKick) {
    				}
					else {
         				//added by Mike, 20210728
         				if (myLevel3D->isLevel2DCollideWith(this)) {
				//            printf(">>>>COLLISION!");
//											currentMovingState=IDLE_MOVING_STATE;
											bHasHitWall=true;
        							return;
         				}
         				else {
										//added by Mike, 20210728
         						if ((bHasHitWall) and (getCurrentFacingState()==FACING_LEFT)) {
         							return;
         						}
        					
								//edited by Mike, 20210728
								//TO-DO: -add: acceleration
/*								
        					myXPosAsPixel+=-stepX;
							
									if ((bIsExecutingDashArray[KEY_A])) {		
											//edited by Mike, 20210527
										myXPosAsPixel+=-stepX;
									}
*/									

/* //edited by Mike, 20211025
         					if (getIsPlayer1()) { //Player1: Unit Chief
             					//edited by Mike, 20210830         					
//            					myXPosAsPixel+=-stepX;
            					myXPos+=-stepX;
         					}
         					else {
             					//edited by Mike, 20210830
//            					myXPosAsPixel+=-stepX;
            					myXPos+=-stepX;
         					}
*/
            					myXPos+=-stepX;
         					

/* //removed by Mike, 20210917; TO-DO: -add: this																											
         					if (bIsExecutingDashArray[KEY_A]) {
								//edited by Mike, 20210807
            					//myXPosAsPixel+=-stepX*2;   
								//dash Command not executed during free fall
								//TO-DO: -update: this due to incorrect output
								//if during free fall, at WALKING_MOVING_STATE
								//dash Command still executed;
								//TURBO?
            					if (currentMovingState==IDLE_MOVING_STATE) {
            					}
								else {
             						//edited by Mike, 20210830								
//            						myXPosAsPixel+=-stepX*2;						
            						myXPos+=-stepX*2;						          						
								}         								  
		 					}									
*/											
									bHasHitWall=false;
								}					
					}
	
/*          
          char str[700];                                       
          sprintf(str,"rotationAngle: %f",rotationAngle);
          MessageBox(NULL, str, "Welcome!", MB_OK);
*/
		   
		   
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_LEFT;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; edited by Mike, 20210729
              //prevFacingState=currentFacingState;
//              if (currentFacingState==FACING_LEFT) {
              if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                  prevFacingState=currentFacingState;
              }

              //added by Mike, 20210502/home/unit_member/Documents/USBONG/pagong-main
              currentFacingState=FACING_LEFT;
		  }

           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/* //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
          break;
          
      //note: combination of FACING RIGHT and DOWN actions cause spin movement          
//     case KEY_RIGHT: //removed by Mike, 20210130
     case KEY_D: //added by Mike, 20210128
		   //removed by Mike, 20201001
//          rotationAngle-=rotationStep;

/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=0.1f;
*/

				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    		//added by Mike, 20210613
    		else if (bIsExecutingKick) {
    		}
				else {
      			//added by Mike, 20210728
      			if (myLevel3D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
        			return;
      			}
      			else {
							//added by Mike, 20210728
         			if ((bHasHitWall) and (getCurrentFacingState()==FACING_RIGHT)) {
         				return;
         			}      			
      			
							//edited by Mike, 20210728
/*							//TO-DO: -add: acceleration
        			//myXPosAsPixel+=stepX*2;
        			myXPosAsPixel+=stepX;
					
							if ((bIsExecutingDashArray[KEY_D])) {
								//edited by Mike, 20210527
								myXPosAsPixel+=stepX;
							}		
*/
/* //edited by Mike, 20211025							
         			if (getIsPlayer1()) { //Player1: Unit Chief
             			//edited by Mike, 20210830								
//            			myXPosAsPixel+=stepX;
            			myXPos+=stepX;          			
         			}
         			else {
             			//edited by Mike, 20210830								
//            			myXPosAsPixel+=stepX;
            			myXPos+=stepX; 
         			}
*/
            			myXPos+=stepX;          			
         			
					
/* //removed by Mike, 20210917; TO-DO: -add: this																																
         			if (bIsExecutingDashArray[KEY_D]) {
         				//edited by Mike, 20210807
            			//myXPosAsPixel+=stepX*2;   
						//dash Command not executed during free fall            			
            			if (currentMovingState==IDLE_MOVING_STATE) {
            			}
						else {
             			//edited by Mike, 20210830								
//            				myXPosAsPixel+=stepX*2;						
            				myXPos+=stepX*2;						
						}           			         			
		 			}									
*/
							
							bHasHitWall=false;
						}
				}
		   		
	  		//added by Mike, 20201201; edited by Mike, 20201225
        		//currentFacingState=FACING_RIGHT;
	  		if (bIsFiringBeam) {	      	
				}
				else {
                    //added by Mike, 20210729; edited by Mike, 20210729
                    //prevFacingState=currentFacingState;
                    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                        prevFacingState=currentFacingState;
                    }
                    
                    currentFacingState=FACING_RIGHT;
				}
		  
           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/*  //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
		  break;
		  
		  //added by Mike, 20210611
			case KEY_K:
                //added by Mike, 20210613
                if (iKickAnimationCount==0){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {
                    bIsExecutingKick=true;
                }
           
           bHasPressedADirectionalKey=false;
           
           //based on enum Keys
           for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
               if (myKeysDown[iCount]==TRUE) {
                   bHasPressedADirectionalKey=true;
                   break;
               }
           }
           
           //removed to hitBy(...) by Mike, 20210612
           currentMovingState=ATTACKING_MOVING_STATE;
           break;
           //added by Mike, 20210613
       case -KEY_K:
           //TO-DO: -reverify: arm angles after release of kick button and then press move down
           if (currentMovingState==WALKING_MOVING_STATE) {
           }
           else if (currentMovingState==ATTACKING_MOVING_STATE) {
           }
           else {
               currentMovingState=IDLE_MOVING_STATE;
           }			
           break;
                      					  			
		//added by Mike, 20201201
		default:
          //edited by Mike, 20210604
		  //currentMovingState=IDLE_MOVING_STATE;
//           if (getIsPlayer1()) { //note: Player1 as Unit Chief
             currentMovingState=IDLE_MOVING_STATE;
//           }
           
		  bIsFiringBeam=false; //added by Mike, 20201226
		  //removed by Mike, 20210123
		  //bIsExecutingPunch=false; //added by Mike, 20210111
		  
		  bIsExecutingDefend=false; //added by Mike, 20210121
           
          //added by Mike, 20210805
          iStepYCount=0;
          bIsExecutingDash=false;
          
/* //removed by Mike, 20210921; gravity          
          //added by Mike, 20210807
          if (myLevel3D->isLevel2DCollideWith(this)) {          	
          }
          else {
          	//+gravity when at free fall
          	//TO-DO: -add: animation sprite image?
          	//note: stepY*2, et cetera is over what the ground/floor tile can push up
            //edited by Mike, 20210830								          	
						myYPos+=stepY;
			
						//added by Mike, 20210812
						//note: this set of instructions NOT executed 
						//when there exists input to execute LEFT or RIGHT movement
						//TO-DO: -update: instructions when computer receives both LEFT and RIGHT input Commands
						if (myLevel3D->isLevel2DCollideWith(this)) {    
						}
						else {
        			//edited by Mike, 20210830								          	
			//				myYPosAsPixel+=stepY; ///2.0
									myYPos+=stepY; //2.0
							
						}									
          }
*/          
		  		break;		  		  
   }

    //added by Mike, 20210613
    if (bIsExecutingKick) {
        currentMovingState=ATTACKING_MOVING_STATE;
    }

    
	//added by Mike, 20210111
	if (bIsExecutingPunch) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}

	//added by Mike, 20210121
	if (bIsExecutingDefend) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}   

/*  //edited by Mike, 20210725
	//added by Mike, 20210724
	//note: in previous computer instructions, we used z-pos for the y-pos now
	//TO-DO: -update: this
	myXPosAsPixel=(int)myXPos;
    //edited by Mike, 20210725
//	myYPosAsPixel=(int)myZPos;
    myYPosAsPixel=(int)myYPos;
*/


	//added by Mike, 20210502
	//TO-DO: -add: FACING_LEFT based on opponent position, e.g. left of pilot

/* //removed by Mike, 20210613
	//added by Mike, 20210104
//	if (!bIsExecutingDefend) {	
	if (!bIsFiringBeam) {
		//added by Mike, 20210203
		//TO-DO: -reverify: fire beam, move diagonal, move non-diagonal direction
		//added by Mike, 20210201
		if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
			//removed by Mike, 20210502
//	        currentFacingState=FACING_LEFT_AND_UP;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
	        currentFacingState=FACING_RIGHT_AND_UP;
		}
		//added by Mike, 20210202
		else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
			//removed by Mike, 20210502			
//	        currentFacingState=FACING_LEFT_AND_DOWN;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
	        currentFacingState=FACING_RIGHT_AND_DOWN;
		}
	}
*/
		
		//edited by Mike, 20211029
		//TO-DO: -add: bIsPlayer1 container in MyDynamicObject.h
		if (bIsPlayer1) {
			
			//TO-DO: -delete: excess instructions; OpenGLCanvas.cpp, Level2D.cpp, Pilot.cpp
    	//TO-DO: -reverify: this; incorrect output when with as input DASH Command
			//TO-DO: -update: if received as input DASH Command
			//edited by Mike, 20210923
	//		if (myLevel3D->getFMyCanvasPosX()-fGridSquareWidth<=0) {
			//edited by Mike, 20210923
	//		if (myLevel3D->getFMyCanvasPosX()<=0) {
			//edited by Mike, 20210924
	//		if (myLevel3D->getFMyCanvasPosX()<0) {
			if (myXPos<0) {	
							//added by Mike, 20210923
            	myXPos=0;
    	}
			//edited by Mike, 20210923
			//TO-DO: -reverify: this
			else if (myLevel3D->getFMyCanvasPosX()<(fMyWindowWidth/2-getWidth())) {
	//			printf(">>>>Pilot myXPos: %f",myXPos); 
					if ((currentFacingState==FACING_LEFT) &&
							(currentMovingState==WALKING_MOVING_STATE)) {
  							//TO-DO: -reverify: this due to still has excess acceleration        
								myXPos=myXPos+stepX/2;
					}
					else {
        		if (myXPos<(fMyWindowWidth/2-getWidth())) {
						}
						else {
								//edited by Mike, 20211025s
	//          	myXPos=fMyWindowWidth/2-getWidth();
          		myXPos=fMyWindowWidth/2-getWidth()*2; //TO-DO: -update: to make 2 based on iPangkatFormationCount
						}
					}							
			}
			else {
								//edited by Mike, 20211025s
	//          	myXPos=fMyWindowWidth/2-getWidth();
          		myXPos=fMyWindowWidth/2-getWidth()*2; //TO-DO: -update: to make 2 based on iPangkatFormationCount
    	}
					
			//edited by Mike, 20210923
	//		if (myLevel3D->getFMyCanvasPosY()-fGridSquareHeight<=0) {
			//edited by Mike, 20210923
	//		if (myLevel3D->getFMyCanvasPosY()<=0) {
			//edited by Mike, 20210924
	//		if (myLevel3D->getFMyCanvasPosY()<0) {
			if (myYPos<0) {
							//added by Mike, 20210923
            	myYPos=0;
    	}				
			//edited by Mike, 20210921
			//TO-DO: -reverify: excess acceleration when Pilot and Canvas both move
	//		else if (myLevel3D->getFMyCanvasPosY()<=(fMyWindowHeight/2-getHeight())) {
			else if (myLevel3D->getFMyCanvasPosY()<(fMyWindowHeight/2-getHeight())) {
	//		else if (myLevel3D->getFMyCanvasPosY()<=fMyWindowHeight/2) {
	//		else if (myLevel3D->getFMyCanvasPosY()<=(fMyWindowHeight/2-getHeight()-stepY)) {
	
	//			printf(">>>>Pilot myYPos: %f",myYPos);
	
	//      myYPos=fMyWindowHeight/2-getHeight();
	
					if ((currentFacingState==FACING_UP) &&
							(currentMovingState==WALKING_MOVING_STATE)) {
							myYPos=myYPos+stepY/2;
	//							myYPos=myYPos+stepY; //shall not move upward
					}
					else {
        		if (myYPos<(fMyWindowHeight/2-getHeight())) {
						}
						else {
								//edited by Mike, 20211025s
          			//myYPos=fMyWindowHeight/2-getHeight();
          	          		myYPos=fMyWindowHeight/2-getHeight()*2; //TO-DO: -update: to make 2 based on iPangkatFormationCount
						}
					}
			}
			else {
				//edited by Mike, 20211025s
      	//myYPos=fMyWindowHeight/2-getHeight();
      	myYPos=fMyWindowHeight/2-getHeight()*2; //TO-DO: -update: to make 2 based on iPangkatFormationCount
			}
	  //added by Mike, 20211029
		}
		//TO-DO: -add: if max at canvas's x position
		//TO-DO: -add: if max at canvas's y position
		else {
					if (myXPos<0) {	
            	myXPos=0;
    			}
    			
					if (myYPos<0) {
            	myYPos=0;
    			}
		}

/* //removed by Mike, 20210921
    //added by Mike, 20210804
    //gravity
    if (myLevel3D->isLevel2DCollideWith(this)) {
        bHasHitWall=true;
        return;
    }
    else {        
        if (bHasHitWall) {
            return;
        }
        
        if (getIsPlayer1()) { //Player1: Unit Chief
						//edited by Mike, 20210831
            myYPos+=stepY;
        }
    }    
*/
    
    bHasHitWall=false;
}

//added by Mike, 20210921
void Robotship::moveSideScrollView(int key)
{
   //Note: Unit member as Pilot has to release hold of directional keys,
   //so that RobotshipShip faces in the correct direction;
   //holding the fire beam key, e.g. KEY_I, causes RobotshipShip 
   //to move without changing the direction that it faces
	
   //added by Mike, 20201201; removed by Mike, 20201226
//   currentMovingState=WALKING_MOVING_STATE;

   //added by Mike, 20201225; removed by Mike, 20201225
   //bIsFiringBeam=false;
   
   //TO-DO: -add: bIsMovingLeft, etc, while facing up, etc

   //added by Mike, 20201226; removed by Mike, 2myZPosAsPixel0201226
//   myKeysDown[key] = TRUE;	 

	//removed by Mike, 20210203
/*	//added by Mike, 20210203
	if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_LEFT_AND_UP;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
        currentFacingState=FACING_RIGHT_AND_UP;
	}
	//added by Mike, 20210202
	else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_LEFT_AND_DOWN;
	}
	else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
        currentFacingState=FACING_RIGHT_AND_DOWN;
	}
*/

   switch (key)
   {
     //added by Mike, 20201218
     case KEY_I:
     	//removed by Mike, 20201225
/*   		  
		  currentMovingState=ATTACKING_MOVING_STATE;		   
          currentFacingState=FACING_UP;
*/          
          //added by Mike, 20201225
          bIsFiringBeam=true;
          
          //added by Mike, 20201226
          //iNumOfKeyTypes
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
          //TO-DO: +reverified: Robotshipship does not execute correctly
		  //when down and left buttons are pressed while firing beam down
		  //AND when up and left buttons are pressed while firing beam up		  
		  //in Windows Machine
		  //problem did not occur on Linux Machine (with external USB keyboard)			
		  //added by Mike, 20210131
		  //note: add use of external USB keyboard solves the problem		  

//          for (int iCount=0; iCount<10; iCount++) {
		   //edited by Mike, 20210130
		   //note: in RobotshipShip.h set PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT=4, not 6,
		   //causes RobotshipShip movement error
		   //edited by Mike, 20210202
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
//          for (int iCount=0; iCount<4; iCount++) {
			  if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
		  }          
          break;
     //added by Mike, 20201226
     case -KEY_I:
//		  currentMovingState=IDLE_MOVING_STATE;
          bIsFiringBeam=false;
          //removed by Mike, 20201226
//          bHasPressedADirectionalKey=false; //added by Mike, 20201226

   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;	

     //added by Mike, 2021011
     //TO-DO: -update: this to be defend using shield
     case KEY_H:
     		 //added by Mike, 20210809; removed by Mike, 20210809
//		     drawShieldEffectAsQuadWithTexture();
     
/* //removed by Mike, 20210809; executes kick Command     
          bIsExecutingDefend=true;
          
          bHasPressedADirectionalKey=false;
          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
   		    if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }
		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }         
*/ 
          break;
     case -KEY_H:
/*          //removed by Mike, 20210124
			bIsExecutingDefend=false;
*/
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;
	
	//added by Mike, 20210121
    case KEY_U:
		  //removed by Mike, 20210123
//          bIsExecutingPunch=true;
		            
		  //added by Mike, 20210122; edited by Mike, 202101213
		  if (iPunchAnimationCount==0){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {
            bIsExecutingPunch=true;
		  }
		             
          bHasPressedADirectionalKey=false;

          //based on enum Keys 
          //edited by Mike, 20210129
//          for (int iCount=0; iCount<10; iCount++) {
          for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
		   	if (myKeysDown[iCount]==TRUE) {
          		bHasPressedADirectionalKey=true;
   		    	break;
			}
		  }

		  
//		  if (!bHasPressedADirectionalKey) {
		  	currentMovingState=ATTACKING_MOVING_STATE;		   		  	
//		  }          
          break;
     case -KEY_U:
    	//TO-DO: -reverify: arm angles after release of punch button and then press move down
     	
   		  if (currentMovingState==WALKING_MOVING_STATE) {   		  	
		  }
		  //added by Mike, 20201226
 		  else if (currentMovingState==ATTACKING_MOVING_STATE) {   		  	
		  }
		  else {
		  	currentMovingState=IDLE_MOVING_STATE;
		  }			
          break;

//	 case KEY_UP: //removed by Mike, 20210130
     case KEY_W:
       //added by Mike, 20210111
       if (bIsExecutingPunch) {
	   }
       //added by Mike, 20210121
	   else if (bIsExecutingDefend) {
	   }
       //added by Mike, 20210613
       else if (bIsExecutingKick) {
       }
       else {
//added by Mike, 20210521
//----------
         //added by Mike, 20210725
         if (myLevel3D->isLevel2DCollideWith(this)) {
//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
             return;
        	//		break;
         }
         else {         
         			//added by Mike, 20210728
         			//edited by Mike, 20210729; TO-DO: -update: this
         		  if ((bHasHitWall) and (getCurrentFacingState()==FACING_UP)) {
//         		  if ((bHasHitWall)) {
         		  	return;
         		  }
         		       
         		           		  
         		                    //edited by Mike, 20210805
                //in macOS machine, Pilot does not move up due to gravity;
                //OK in LUBUNTU machine; JUMP height low; exerting effort to move up, but pulled by heavy force
//            	myYPosAsPixel+=-stepY;
//            	myYPosAsPixel+=(-stepY*2);
//            	myYPosAsPixel+=(-stepY*1.1);
  
  						 //edited by Mike, 20210806           
//             iStepYCountMax=20;
             iStepYCountMax=10;

             if (iStepYCount>=iStepYCountMax) {
             			//added by Mike, 20210806;
             			//removed by Mike, 20210806;
             			//Yosshi- Dinosaur?
//             		return;

            			currentMovingState=IDLE_MOVING_STATE;
              		currentFacingState=FACING_UP;
                 
                 break;
             }
             else {
             		//edited by Mike, 20210830
//                myYPosAsPixel+=(-stepY*1.1);                
								//edited by Mike, 20210831
                myYPos+=(-stepY*1.1);
                //myYPos+=(stepY*1.1);
                iStepYCount+=1;
            
             }

		 					bHasHitWall=false;
		 		}
			//added by Mike, 20210521		
//----------		
       }
	
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_UP;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
					//added by Mike, 20210728; edited by Mike, 20210729
					//TO-DO: -update: this
              //added by Mike, 20210729; removed by Mike, 20210729;
              //yoko scroll
//              prevFacingState=currentFacingState;
              
              currentFacingState=FACING_UP;
		  }
		  
          currentMovingState=WALKING_MOVING_STATE;
          break;

		   //edited by Mike, 20210812
           return;
//           break;

 //     case KEY_DOWN:  //removed by Mike, 20210130
     case KEY_S: //added by Mike, 20210128
				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    			//added by Mike, 20210613
    			else if (bIsExecutingKick) {
    			}
				else {
         			//added by Mike, 20210725
         			if (myLevel3D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//										currentMovingState=IDLE_MOVING_STATE;
										bHasHitWall=true;
        						return;
         			}
         			else {				
					  			//added by Mike, 20210728
         					if ((bHasHitWall) and (getCurrentFacingState()==FACING_DOWN)) {
         		  			return;
         					}
         					
/* //edited by Mike, 20211025         					
        					if (getIsPlayer1()) { //Player1: Unit Chief
             					//edited by Mike, 20210830
            					//myYPosAsPixel+=stepY;        					
//            					myYPos+=stepY;
										//edited by Mike, 20210831
            					myYPos+=stepY; 
//										myYPos+=(-stepY);
            					
        					}
        					else {
             					//edited by Mike, 20210830
            					//myYPosAsPixel+=stepY/2;
										//edited by Mike, 20210831
            					myYPos+=stepY/2; 
//										myYPos+=(-stepY/2);
            									
        					}
*/
           					myYPos+=stepY;        					
                                }
                                
/* //removed by Mike, 20211025
 if ((bIsExecutingDashArray[KEY_S])) {
             			//edited by Mike, 20210830
//            			myYPosAsPixel+=stepY*2;
						//edited by Mike, 20210831
						myYPos+=stepY*2;
//						myYPos+=(-(stepY*2));             			
					}
*/					
					
							bHasHitWall=false;
				}
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_DOWN;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; removed by Mike, 20210729
              //yoko scroll
//              prevFacingState=currentFacingState;

              //added by Mike, 20210728
              currentFacingState=FACING_DOWN;
		  }

           currentMovingState=WALKING_MOVING_STATE;
          break;
        //added by Mike, 20210806; edited by Mike, 20210812
           return;
//			break;
			
			
//     case KEY_LEFT: //removed by Mike, 20210130
     case KEY_A: //added by Mike, 20210128		   
     		//removed by Mike, 20201001
//          rotationAngle+=rotationStep;
/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=-0.1f;
*/
					//added by Mike, 20210111
					if (bIsExecutingPunch) {
					}
					//added by Mike, 20210121
					else if (bIsExecutingDefend) {
					}
    				//added by Mike, 20210613
    				else if (bIsExecutingKick) {
    				}
					else {
         				//added by Mike, 20210728
         				if (myLevel3D->isLevel2DCollideWith(this)) {
				//            printf(">>>>COLLISION!");
//											currentMovingState=IDLE_MOVING_STATE;
											bHasHitWall=true;
        							return;
         				}
         				else {
										//added by Mike, 20210728
         						if ((bHasHitWall) and (getCurrentFacingState()==FACING_LEFT)) {
         							return;
         						}
        					
								//edited by Mike, 20210728
								//TO-DO: -add: acceleration
/*								
        					myXPosAsPixel+=-stepX;
							
									if ((bIsExecutingDashArray[KEY_A])) {		
											//edited by Mike, 20210527
										myXPosAsPixel+=-stepX;
									}
*/									
         					if (getIsPlayer1()) { //Player1: Unit Chief
             					//edited by Mike, 20210830         					
//            					myXPosAsPixel+=-stepX;
            					myXPos+=-stepX;
         					}
         					else {
             					//edited by Mike, 20210830
//            					myXPosAsPixel+=-stepX;
            					myXPos+=-stepX;
         					}

/* //removed by Mike, 20210917; TO-DO: -add: this																											
         					if (bIsExecutingDashArray[KEY_A]) {
								//edited by Mike, 20210807
            					//myXPosAsPixel+=-stepX*2;   
								//dash Command not executed during free fall
								//TO-DO: -update: this due to incorrect output
								//if during free fall, at WALKING_MOVING_STATE
								//dash Command still executed;
								//TURBO?
            					if (currentMovingState==IDLE_MOVING_STATE) {
            					}
								else {
             						//edited by Mike, 20210830								
//            						myXPosAsPixel+=-stepX*2;						
            						myXPos+=-stepX*2;						          						
								}         								  
		 					}									
*/											
									bHasHitWall=false;
								}					
					}
	
/*          
          char str[700];                                       
          sprintf(str,"rotationAngle: %f",rotationAngle);
          MessageBox(NULL, str, "Welcome!", MB_OK);
*/
		   
		   
	      //added by Mike, 20201201; edited by Mike, 20201225
          //currentFacingState=FACING_LEFT;
	      if (bIsFiringBeam) {	      	
		  }
		  else {
              //added by Mike, 20210729; edited by Mike, 20210729
              //prevFacingState=currentFacingState;
//              if (currentFacingState==FACING_LEFT) {
              if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                  prevFacingState=currentFacingState;
              }

              //added by Mike, 20210502
              currentFacingState=FACING_LEFT;
		  }

           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/* //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
          break;
          
      //note: combination of FACING RIGHT and DOWN actions cause spin movement          
//     case KEY_RIGHT: //removed by Mike, 20210130
     case KEY_D: //added by Mike, 20210128
		   //removed by Mike, 20201001
//          rotationAngle-=rotationStep;

/*		//removed by Mike, 20201014
     	  //added by Mike, 20201001
          if (thrust<thrustMax)
            thrust+=0.1f;
*/

				//added by Mike, 20210111
				if (bIsExecutingPunch) {
				}
				//added by Mike, 20210121
				else if (bIsExecutingDefend) {
				}
    		//added by Mike, 20210613
    		else if (bIsExecutingKick) {
    		}
				else {
      			//added by Mike, 20210728
      			if (myLevel3D->isLevel2DCollideWith(this)) {
			//            printf(">>>>COLLISION!");
//							currentMovingState=IDLE_MOVING_STATE;
							bHasHitWall=true;
        			return;
      			}
      			else {
							//added by Mike, 20210728
         			if ((bHasHitWall) and (getCurrentFacingState()==FACING_RIGHT)) {
         				return;
         			}      			
      			
							//edited by Mike, 20210728
/*							//TO-DO: -add: acceleration
        			//myXPosAsPixel+=stepX*2;
        			myXPosAsPixel+=stepX;
					
							if ((bIsExecutingDashArray[KEY_D])) {
								//edited by Mike, 20210527
								myXPosAsPixel+=stepX;
							}		
*/							
         			if (getIsPlayer1()) { //Player1: Unit Chief
             			//edited by Mike, 20210830								
//            			myXPosAsPixel+=stepX;
            			myXPos+=stepX;          			
         			}
         			else {
             			//edited by Mike, 20210830								
//            			myXPosAsPixel+=stepX;
            			myXPos+=stepX; 
         			}
					
/* //removed by Mike, 20210917; TO-DO: -add: this																																
         			if (bIsExecutingDashArray[KEY_D]) {
         				//edited by Mike, 20210807
            			//myXPosAsPixel+=stepX*2;   
						//dash Command not executed during free fall            			
            			if (currentMovingState==IDLE_MOVING_STATE) {
            			}
						else {
             			//edited by Mike, 20210830								
//            				myXPosAsPixel+=stepX*2;						
            				myXPos+=stepX*2;						
						}           			         			
		 			}									
*/
							
							bHasHitWall=false;
						}
				}
		   		
	  		//added by Mike, 20201201; edited by Mike, 20201225
        		//currentFacingState=FACING_RIGHT;
	  		if (bIsFiringBeam) {	      	
				}
				else {
                    //added by Mike, 20210729; edited by Mike, 20210729
                    //prevFacingState=currentFacingState;
                    if ((currentFacingState==FACING_RIGHT) || (currentFacingState==FACING_LEFT)) {
                        prevFacingState=currentFacingState;
                    }
                    
                    currentFacingState=FACING_RIGHT;
				}
		  
           //edited by Mike, 20210613
           currentMovingState=WALKING_MOVING_STATE;
/*  //removed by Mike, 20210613
           if (bIsExecutingKick) {
               currentMovingState=ATTACKING_MOVING_STATE;
           }
*/
		  break;
		  
		  //added by Mike, 20210611
			case KEY_K:
                //added by Mike, 20210613
                if (iKickAnimationCount==0){// or (iPunchAnimationCount>=MAX_PUNCH_ANIMATION_COUNT)) {
                    bIsExecutingKick=true;
                }
           
           bHasPressedADirectionalKey=false;
           
           //based on enum Keys
           for (int iCount=0; iCount<PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT; iCount++) {
               if (myKeysDown[iCount]==TRUE) {
                   bHasPressedADirectionalKey=true;
                   break;
               }
           }
           
           //removed to hitBy(...) by Mike, 20210612
           currentMovingState=ATTACKING_MOVING_STATE;
           break;
           //added by Mike, 20210613
       case -KEY_K:
           //TO-DO: -reverify: arm angles after release of kick button and then press move down
           if (currentMovingState==WALKING_MOVING_STATE) {
           }
           else if (currentMovingState==ATTACKING_MOVING_STATE) {
           }
           else {
               currentMovingState=IDLE_MOVING_STATE;
           }			
           break;
                      					  			
		//added by Mike, 20201201
		default:
          //edited by Mike, 20210604
		  //currentMovingState=IDLE_MOVING_STATE;
//           if (getIsPlayer1()) { //note: Player1 as Unit Chief
             currentMovingState=IDLE_MOVING_STATE;
//           }
           
		  bIsFiringBeam=false; //added by Mike, 20201226
		  //removed by Mike, 20210123
		  //bIsExecutingPunch=false; //added by Mike, 20210111
		  
		  bIsExecutingDefend=false; //added by Mike, 20210121
           
          //added by Mike, 20210805
          iStepYCount=0;
          bIsExecutingDash=false;
          
					//note: gravity          
          //added by Mike, 20210807
          if (myLevel3D->isLevel2DCollideWith(this)) {          	
          }
          else {
          	//+gravity when at free fall
          	//TO-DO: -add: animation sprite image?
          	//note: stepY*2, et cetera is over what the ground/floor tile can push up
            //edited by Mike, 20210830								          	
						myYPos+=stepY;
			
						//added by Mike, 20210812
						//note: this set of instructions NOT executed 
						//when there exists input to execute LEFT or RIGHT movement
						//TO-DO: -update: instructions when computer receives both LEFT and RIGHT input Commands
						if (myLevel3D->isLevel2DCollideWith(this)) {    
						}
						else {
        			//edited by Mike, 20210830								          	
			//				myYPosAsPixel+=stepY; ///2.0
									myYPos+=stepY; //2.0
							
						}									
          }
		  		break;		  		  
   }

    //added by Mike, 20210613
    if (bIsExecutingKick) {
        currentMovingState=ATTACKING_MOVING_STATE;
    }

    
	//added by Mike, 20210111
	if (bIsExecutingPunch) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}

	//added by Mike, 20210121
	if (bIsExecutingDefend) {
		currentMovingState=ATTACKING_MOVING_STATE;
		bIsFiringBeam=false;
	}   

/*  //edited by Mike, 20210725
	//added by Mike, 20210724
	//note: in previous computer instructions, we used z-pos for the y-pos now
	//TO-DO: -update: this
	myXPosAsPixel=(int)myXPos;
    //edited by Mike, 20210725
//	myYPosAsPixel=(int)myZPos;
    myYPosAsPixel=(int)myYPos;
*/


	//added by Mike, 20210502
	//TO-DO: -add: FACING_LEFT based on opponent position, e.g. left of pilot

/* //removed by Mike, 20210613
	//added by Mike, 20210104
//	if (!bIsExecutingDefend) {	
	if (!bIsFiringBeam) {
		//added by Mike, 20210203
		//TO-DO: -reverify: fire beam, move diagonal, move non-diagonal direction
		//added by Mike, 20210201
		if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_W])) {
			//removed by Mike, 20210502
//	        currentFacingState=FACING_LEFT_AND_UP;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_W])) {
	        currentFacingState=FACING_RIGHT_AND_UP;
		}
		//added by Mike, 20210202
		else if ((myKeysDown[KEY_A]) && (myKeysDown[KEY_S])) {
			//removed by Mike, 20210502			
//	        currentFacingState=FACING_LEFT_AND_DOWN;
		}
		else if ((myKeysDown[KEY_D]) && (myKeysDown[KEY_S])) {
	        currentFacingState=FACING_RIGHT_AND_DOWN;
		}
	}
*/
		

		//TO-DO: -delete: excess instructions; OpenGLCanvas.cpp, Level2D.cpp, Pilot.cpp
    //TO-DO: -reverify: this; incorrect output when with as input DASH Command
		//TO-DO: -update: if received as input DASH Command
        //edited by Mike, 20210922
		if (myLevel3D->getFMyCanvasPosX()-fGridSquareWidth<=0) {
//        if (myLevel3D->getFMyCanvasPosX()<=0) {

        }
		else if (myLevel3D->getFMyCanvasPosX()<=(fMyWindowWidth/2-getWidth())) {
        if (myXPos<=(fMyWindowWidth/2-getWidth())) {
        }
        else {
        		//edited by Mike, 20210922
          myXPos=fMyWindowWidth/2-getWidth();
//            myXPos=fMyWindowWidth/2-getWidth()-getStepX();

            //added by Mike, 20210922
//            myLevel3D->setFMyCanvasPosX(myLevel3D->getFMyCanvasPosX()+getStepX());
        }
		}
		else {
      //edited by Mike, 20210922
    myXPos=fMyWindowWidth/2-getWidth();
//      myXPos=fMyWindowWidth/2-getWidth()-getStepX();

      //added by Mike, 20210922
//      myLevel3D->setFMyCanvasPosX(myLevel3D->getFMyCanvasPosX()+getStepX());
		}

    //added by Mike, 20210804
    //gravity
    if (myLevel3D->isLevel2DCollideWith(this)) {
        bHasHitWall=true;
        return;
    }
    else {        
        if (bHasHitWall) {
            return;
        }
        
        if (getIsPlayer1()) { //Player1: Unit Chief
						//edited by Mike, 20210831
            myYPos+=stepY;
        }
    }    
        
    bHasHitWall=false;
}


void Robotship::hitBy(MyDynamicObject* mdo)
{
/*	//removed by Mike, 20210611stepX=fGridSquareWidth/8;
		//auto-identify if Ball object
    if (dynamic_cast<Ball*>(mdo)->getIsBall()) {    	
		}
*/
        //added by Mike, 20210613; edited by Mike, 20210613
/*
        bIsExecutingKick=true;
        currentMovingState=ATTACKING_MOVING_STATE;
        iKickAnimationCount=0;
*/
  
/* //removed by Mike, 20210725
    //edited by Mike, 20210613
//    move(KEY_K);
    if (!mdo->getIsMovingDown()) {
        move(KEY_K);
    }
*/

/*	//removed by Mike, 20210729
        //added by Mike, 20210613
        if (myKeysDown[KEY_D]==TRUE) {
            currentFacingState=FACING_RIGHT;
        }
        else if (myKeysDown[KEY_A]==TRUE) {
            currentFacingState=FACING_LEFT;
        }
*/
    
    
//		mdo->updateDirection();

	//removed by Mike, 20210527
/*
     //changeState(DEATH_STATE);
     //setCollidable(false);
    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
	////zing = sound->load_sound_clip(RESETSOUND);
	////sound->play_sound_clip(zing);	

    reset();
*/        
}

/*	//removed by Mike, 20210522
void Robotship::setOpenGLCanvas(OpenGLCanvas* c)
{
     myOpenGLCanvas = c;
}
*/

void Robotship::reset()
{
/*  //removed by Mike, 20210801
     changeState(INITIALIZING_STATE);
     myXPos=0;
     myYPos=0;
     rotationAngle=0.0f;
     thrust=0.0f;
    
    //removed by Mike, 20210725
    //setCollidable(false);
 
    invincibleCounter=0;
    
    //added by Mike, 20210613
    bIsExecutingKick=false;
    iKickAnimationCount=0;
    iKickAnimationCountDelay=0;
*/
}
int Robotship::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Robotship::destroy()
{
/*	
	for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
	}
	delete [] explosionParticle;
*/
}

//--------------------------------------------
