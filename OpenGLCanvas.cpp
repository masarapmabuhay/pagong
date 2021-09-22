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
 * @date updated: 20210922
 * @website address: http://www.usbong.ph
 *
 * References:
 * 1) https://www.mathsisfun.com/sine-cosine-tangent.html;
 * last accessed: 20201026
 *
 * 2) https://community.khronos.org/t/moving-an-object-with-respect-to-the-camera/40968;
 * last accessed: 20201026
 *
 * 3) https://github.com/opengl-tutorials/ogl/blob/master/tutorial08_basic_shading/tutorial08.cpp;
 * last accessed: 20210330
 *
 * 4) http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/;
 * last accessed: 20210330
 *
 * 5) https://www.mathsisfun.com/algebra/line-equation-2points.html;
 * last accessed: 20210513
 * point-slope equation
 * y − y1 = m(x − x1)
 * m = (change in y) / (change in x)
 * where m=slope
 * add: verify if (iEndPointX - iStartPointX) = 0; example: vertical line
 * if yes, output equation: x=iEndPointX
 * int m = (iEndPointY - iStartPointY) / (iEndPointX - iStartPointX);
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007):
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */
 
//added by Mike, 20210825
//TO-DO: -reuse: with SDL + GL Commands, e.g. SDL_GL_CreateContext(...)

//added by Mike, 20210701
//note: reminded: of text system in select computer games, e.g. Dragon Quest,
//during compile execution on Command Prompt, Terminal Window
//where: compile output = executable file
//note: text system technique reusable in tile-based map system
//know-how of column and row technique in Maparaang Pagbibilang vital


//TO-DO: -update: "Canvas" to "Engine"?
//TO-DO: -update: this

//TO-DO: -update: instructions in auto-drawing of tiles due to lines between tiles are visible

//#include <SDL.h>
//edited by Mike, 20200929

//added by Mike, 20201217
//Reference: https://stackoverflow.com/questions/34152424/autodetection-of-os-in-c-c;
//answer by: Jeegar Patel, 20151208T0940
//auto-identify if Windows Machine
#ifdef _WIN32
#include <windows.h> //Windows Machine
#endif

//added by Mike, 20201121
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h> //added by Mike, 20210623
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> //added by Mike, 20210623
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

#include <stdio.h>
//#include <math.h>

#include "OpenGLCanvas.h"

//added by Mike, 20210827
#include "Level2D.h"
#include "Pilot.h" 

//added by Mike, 20210902
#include "Text.h"


/* //removed by Mike, 20210825
//added by Mike, 20201001
#include "RobotShip.h"

//added by Mike, 20201207

//added by Mike, 20210514
//note: add this here in the .cpp file
#include "Button.h"

//added by Mike, 20210528
#include "Ball.h"

//added by Mike, 20210613
#include "Text.h"

//added by Mike, 20201010
#include "Font.h"

//added by Mike, 20210516
#include "UsbongUtils.h"

//added by Mike, 20201118
#include "Level.h"

//added by Mike, 20201013
#include "Beam.h"
//added by Mike, 20201016
#include "Asteroid.h"
*/

//added by Mike, 20201217
#include <algorithm>
#include <vector>

//#include "Sound.h"

//#include "DynamicObject.h"

//#include "glfont.h"

//removed by Mike, 20201014
//int i;

//TO-DO: -reverify: KEY_D!=OK; KEY_RIGHT=OK
//TO-DO: -reverify: KEY_W!=OK; KEY_UP=OK

//added by Mike, 20210321
#include <iostream> //we use with std::cout

//added by Mike, 20201001
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
    KEY_H, //added by Mike, 20210111
    KEY_U, //added by Mike, 20210121
    
    //added by Mike, 20210513
    KEY_Z,
    
    //added by Mike, 20201201
    iNumOfKeyTypes
};

//added by Mike, 20210510
enum mouseActionIds
{
    MOUSE_LEFT_BUTTON = 0,
    iNumOfMouseActionIdTypes
};

//added by Mike, 20201213
//Reference: https://stackoverflow.com/questions/10287924/fastest-way-to-sort-a-list-of-number-and-their-index;
//last accessed: 20201213
//answer by: Jerry Coffin, 20120423T2045
//edited by: 20131118T1435
//TO-DO: -add: in UsbongUtils
//descending
/* //removed by Mike, 20210825    
struct sortByZPosition {
    //bool operator()(MyDynamicObject const &left, MyDynamicObject const &right) {
    //bool operator()(MyDynamicObject &left, MyDynamicObject &right) {
    bool operator()(MyDynamicObject *left, MyDynamicObject *right) {
        //        return left.getZ() > right.getZ();
        //edited by Mike, 20210509
        //note: higher negative, farther distance in z-axis
        //        return left->getZ() > right->getZ();
        return left->getZ() < right->getZ();
    }
};
*/

OpenGLCanvas::OpenGLCanvas()
{
    myRepaintCounter=0;
    myRepaintDelay=20;
}

OpenGLCanvas::~OpenGLCanvas()
{
}

/* //removed by Mike, 20210826
//added by Mike, 20210825
//TO-DO: -put: this in Pilot.cpp, et cetera
//SDL_Texture *loadTexture(char *filename)
SDL_Texture *loadTexture(char *filename, SDL_Renderer *mySDLRendererInput)
{
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(mySDLRendererInput, filename);

	return texture;
}
*/

/* //removed by Mike, 20210826
//added by Mike, 20210826
GLuint openGLLoadTexture(char *filename, int *textw, int *texth)
{
	SDL_Surface *surface;
	GLenum textureFormat;
	GLuint texture;
	
	surface = IMG_Load(filename);
	
	if (!surface){
		return 0;
	}

//added by Mike, 20210824
//TO-DO: -add: image frame clipping
#if defined(__APPLE__)
    switch (surface->format->BytesPerPixel) {
        case 4:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
//                textureFormat = GL_BGRA;
                textureFormat = GL_RGBA;
            else
//                textureFormat = GL_RGBA;
                textureFormat = GL_BGRA;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
//                textureFormat = GL_BGR;
                textureFormat = GL_RGB;
            else
//                textureFormat = GL_RGB;
                textureFormat = GL_BGR;
            break;
    }
#else
    switch (surface->format->BytesPerPixel) {
        case 4:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                textureFormat = GL_BGRA;
            else
                textureFormat = GL_RGBA;
            break;
            
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                textureFormat = GL_BGR;
            else
                textureFormat = GL_RGB;
            break;
    }
#endif
    
	*textw = surface->w/4;
	*texth = surface->h/4;
	
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w,
	surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	
	SDL_FreeSurface(surface);
	
	return texture;	
}

//added by Mike, 20210826
//TO-DO: -add: CAD tool to assist in identify excess markings in image file
//-add: CAD tool to verify animating sequence
void openGLDrawTexture(int x, int y, GLuint textureid, int textw, int texth)
{
	glBindTexture(GL_TEXTURE_2D, textureid);
	glEnable(GL_TEXTURE_2D);
	
	float fTaoAnimationFrameOffset=0.0f;
	float fTaoAnimationFrameOffsetYAxis=0.0f;

	//added by Mike, 20210826
//	glColor3f(1.0f, 1.0f, 1.0f); // white
	
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
	
	glDisable(GL_TEXTURE_2D);
}
*/

//edited by Mike, 20210725; edited again by Mike, 20210825
//bool OpenGLCanvas::init(int myWindowWidthAsPixelInput, int myWindowHeightAsPixelInput)
bool OpenGLCanvas::init(int myWindowWidthAsPixelInput, int myWindowHeightAsPixelInput, SDL_Renderer *mySDLRendererInput)
//bool OpenGLCanvas::init(float myWindowWidthAsPixelInput, float myWindowHeightAsPixelInput)
{
	  //added by Mike, 20210825
	  mySDLRenderer = mySDLRendererInput;

		iPilotX=0;
		iPilotY=0;	

/*	//edited by Mike, 20210910
    //added by Mike, 20201023
    myCanvasPosX=-3.2f;//0.0f;
    myCanvasPosY=-1.0f;//0.0f;
    myCanvasPosZ=-3.2f;//0.0f;
    myCanvasEyePosX=0.0f;
*/
/* //removed by Mike, 20210913
    myCanvasPosX=0.0f;//0.0f;
    myCanvasPosY=0.0f;//0.0f;
    myCanvasPosZ=0.0f;//0.0f;
    myCanvasEyePosX=0.0f;
*/
    
    //added by Mike, 20210510
    iStartPointX=0;
    iStartPointY=0;
    iEndPointX=0;
    iEndPointY=0;
    
    //edited by Mike, 20201025
    myCanvasStepX=0.3f;//0.32f;//0.3f;//0.1f;
    myCanvasStepY=0.3f;//0.32f;//0.3f;//0.1f;
    myCanvasStepZ=0.3f;//0.32f;//0.3f;//0.1f;
    
    //added by Mike, 20201025; edited by Mike, 20201026
    //add +0.5f to turn right without turning left first
    myCanvasLookAtAngle=-90.0f+0.5f;//-90.0f;
    
    //added by Mike, 20201024; removed by Mike, 20201025
    //myCanvasRotateAxisStepY=0.0f;
    
    //added by Mike, 20201025
    myCanvasEyePosX=0.0f;
    myCanvasEyePosY=1.0f;
    
    //edited by Mike, 20210117
    //note: 3.0f OK if not rapid fire of beams as projectile
    //5.0f OK for rapid fire, but edge of 30x30 grid
    //with window size 6144x6144 already visible
    //    myCanvasEyePosZ=3.0f;//0.0f;//3.0f;
    myCanvasEyePosZ=5.0f;//0.0f;//3.0f;
    
    myCanvasCenterPosX=0.0f;
    myCanvasCenterPosY=0.0f;
    //edited by Mike, 20201026
    myCanvasCenterPosZ=1.0f;//1.0f;
    
    //added by Mike, 20210416
    fKahonRotation=0;    
    keyPressCounter=0;
    
    //added by Mike, 20210512
    hasPressedMouseActionDown=false; //init here due to default value not "false"
    
    //added by Mike, 20200930
    currentState = GAME_SCREEN; //TO-DO: -update: this
  
  	//edited by Mike, 20210830
  	//TO-DO: -update: this due to we now use floating point numbers, instead of integers
    myWindowWidthAsPixel = (int)myWindowWidthAsPixelInput;
    myWindowHeightAsPixel = (int)myWindowHeightAsPixelInput;

    myWindowWidth = myWindowWidthAsPixel;
    myWindowHeight = myWindowHeightAsPixel;

		//added by Mike, 20210913
		//edited by Mike, 20210916
//    myCanvasPosX=0.0f+myWindowWidth/3;
    myCanvasPosX=0.0f;

    myCanvasPosY=0.0f;
    myCanvasPosZ=0.0f;
    myCanvasEyePosX=0.0f;


/* //removed by Mike, 20210826
  	//added by Mike, 20210809    
    printf("HALLO: myWindowWidthAsPixel: %i\n",myWindowWidthAsPixel);
    printf(">>: myWindowHeightAsPixel: %i\n",myWindowHeightAsPixel);
*/
       
    iRowCountMax=10;
    iColumnCountMax=18;
    iHeightCountMax=10;

  	//edited by Mike, 20210830
/*  	    
    fGridSquareWidth = (myWindowWidthAsPixel*(1.0))/iColumnCountMax; //example: 136.60
    fGridSquareHeight = (myWindowHeightAsPixel*(1.0))/iRowCountMax; //example: 76.80
*/
    fGridSquareWidth = (myWindowWidthAsPixel*(1.0))/iColumnCountMax; //example: 136.60
    fGridSquareHeight = (myWindowHeightAsPixel*(1.0))/iRowCountMax; //example: 76.80

    
/* //removed by Mike, 20210826    
    printf("OpenGLCanvas.cpp; fGridSquareWidth: %f",fGridSquareWidth);
*/

/*    
    printf("fGridSquareWidth: %f\n",fGridSquareWidth);
    printf("fGridSquareHeight: %f\n",fGridSquareHeight);
*/
    
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0; //added by Mike, 20210701
    
/* //removed by Mike, 20210827	
	iPilotX=myWindowWidthAsPixel/2;
	iPilotY=myWindowHeightAsPixel/2;
*/    
    

/* //removed by Mike, 20210825            
    //added by Mike, 20210516
    myUsbongUtils = new UsbongUtils();
    myUsbongUtils->setWindowWidthHeight(myWindowWidthAsPixel, myWindowHeightAsPixel);
*/
    
//    printf("OpenGLCanvas.cpp myWindowWidthAsPixel: %f\n",myWindowWidthAsPixel);
    
    
    //added by Mike, 20210517; edited by Mike, 20210606
    /*    myWindowWidthAsPixel = myWindowWidth;
     myWindowHeightAsPixel = myWindowHeight;
     */
    /* //edited by Mike, 20210625
     myWindowWidthAsPixel = myWindowWidthAsPixelInput;
     myWindowHeightAsPixel = myWindowHeightAsPixelInput;
     */
    
/* //removed by Mike, 20210825
    //added by Mike, 20201001
    //edited by Mike, 20201115
    //myRobotShip = new RobotShip;
    //edited by Mike, 20210211
    //myRobotShip = new RobotShip(0.0f,0.0f,0.0f,myWindowWidth,myWindowHeight);
    
    //added by Mike, 20210321
    //TO-DO: -update: these due to getMaxXAxisViewport()... already the actual size
    
    //	printf("%d",myLevel->getMaxXAxisViewport());
    myRobotShip = new RobotShip(0.0f,0.0f,0.0f,myLevel->getMaxXAxisViewport()*fGridSquareWidth,myLevel->getMaxZAxisViewport()*fGridSquareHeight);
    //edited by Mike, 20210522
    //	myRobotShip->setOpenGLCanvas(this);
    //edited by Mike, 20210710
    //    myRobotShip->setOpenGLCanvas(this, fGridSquareWidth);
    myRobotShip->setOpenGLCanvas(this, fGridSquareWidth, fGridSquareHeight);
*/

    myLevel2D = new Level2D(0.0f,0.0f,0.0f,myWindowWidthAsPixelInput,myWindowHeightAsPixelInput);
    //note: width and height not equal due to Window
    //to cause square tile to NOT be square
    myLevel2D->setOpenGLCanvas(this, fGridSquareWidth, fGridSquareHeight);

    //removed by Mike, 20210828
    //myLevel2D->setupLevel(LEVEL_2D_TEXTURE);

    //added by Mike, 20210830; edited by Mike, 20210914
//    myPilot = new Pilot(myWindowWidthAsPixel/3,myWindowHeightAsPixel/2,0.0f,myWindowWidthAsPixel,myWindowHeightAsPixel);
		//edited by Mike, 20210916
//    myPilot = new Pilot(myWindowWidthAsPixel/2,myWindowHeightAsPixel/2,0.0f,myWindowWidthAsPixel,myWindowHeightAsPixel);
		//edited by Mike, 20210921
//    myPilot = new Pilot(0.0f,myWindowHeightAsPixel/2,0.0f,myWindowWidthAsPixel,myWindowHeightAsPixel);
    myPilot = new Pilot(0.0f,0.0f,0.0f,myWindowWidthAsPixel,myWindowHeightAsPixel);


    myPilot->setOpenGLCanvas(this, fGridSquareWidth, fGridSquareHeight);    
    myPilot->setAsPlayer1(); //added by Mike, 20210601    
    //added by Mike, 20210830
    myPilot->setLevel2D(myLevel2D);
    //added by Mike, 20210911
    myLevel2D->setPilotStep(myPilot->getStepX(), myPilot->getStepY(), myPilot->getStepZ());
    
    //added by Mike, 20210903
    //note: positions re-set inside function
    myText = new Text(-1.0f,-1.0f,-1.0f,myWindowWidthAsPixel,myWindowHeightAsPixel);
    myText->setOpenGLCanvas(this, fGridSquareWidth, fGridSquareHeight);
        
    
    //added by Mike, 20210513
    for (int iCount=0; iCount<MAX; iCount++) { //1024
        for (int iCountPoint=0; iCountPoint<LINE_POINTS_SET; iCountPoint++) { //1024
            stepHistoryList[iCount][iCountPoint]=0;
        }
    }
    
    //edited by Mike, 20210701
    //    stepHistoryListCount=0;
    stepHistoryListCount=1;
    
    
    //printf(">>> myWindowWidthAsPixel: %i\n",myWindowWidthAsPixel);

    return true;
}


bool OpenGLCanvas::shutdown()
{
    /*	//removed by Mike, 20200929
     sound->unload_sound_clip(thrust);
     sound->unload_sound_clip(rotate);
     sound->unload_sound_clip(beam);
     sound->unload_sound_clip(explosion);
     sound->unload_sound_clip(zing);
     sound->unload_sound_clip(title);
     sound->unload_sound_clip(bonus);
     
     sound->sound_stop();
     
     glDeleteTextures(1, &currTextureBackground);
     glDeleteTextures(1, &myTextureObjectFont);
     glDeleteTextures(1, &titleBackground);
     glDeleteTextures(1, &gameBackground);
     //myBackground->Release();
     //delete myBackground;
     
     //releaseFont(myFontListBase);
     */
    
/* //removed by Mike, 20210825    
    //added by Mike, 20201016
    for (int i=0; i<MAX_ASTEROID; i++)
    {
        myAsteroid[i]->destroy();
    }
*/    
    return true;
}


bool OpenGLCanvas::setupProjection()
{
    /*	//removed by Mike, 20200929
     glViewport(0, 0, myWindowWidth, myWindowHeight); // reset the viewport to new dimensions
     */
    return true;
}

void OpenGLCanvas::keyDown(int keyCode)
{
    myKeysDown[keyCode] = TRUE;

/* //removed by Mike, 20210825
	//added by Mike, 20210812
	myKeysDown[KEY_W] = FALSE;
	myKeysDown[KEY_S] = FALSE;
*/

    myPilot->keyDown(keyCode);

//    printf(">>OpenGLCanvas::keyDown: %i",keyCode);
    
    //added by Mike, 20210903
    myText->keyDown(keyCode);
    
    //edited by Mike, 20210514
    if (keyCode==KEY_Z) {
        //added by Mike, 20210513
        //edited by Mike, 20210702
        //        if (stepHistoryListCount>0) {
        if (stepHistoryListCount>1) {
            stepHistoryListCount=stepHistoryListCount-1;
            
            stepHistoryList[stepHistoryListCount][0]=0;
            stepHistoryList[stepHistoryListCount][1]=0;
            stepHistoryList[stepHistoryListCount][2]=0;
            stepHistoryList[stepHistoryListCount][3]=0;
            
            //added by Mike, 20210514
            iStartPointX=0;
            iStartPointY=0;
            iEndPointX=0;
            iEndPointY=0;
        }
        //added by Mike, 20210702
        else {
            stepHistoryListCount=1;
        }
    }
}
void OpenGLCanvas::keyUp(int keyCode)
{
    myKeysDown[keyCode] = FALSE;
    
/* //removed by Mike, 20210826    
    //added by Mike, 20201226; edited by Mike, 20210423;
    //added by Mike, 20210507
    myRobotShip->keyUp(keyCode);
*/
    
    myPilot->keyUp(keyCode);
    //added by Mike, 20210604; removed by Mike, 20210604
    //note: myPilot as Unit Chief

/* //removed by Mike, 20210830    
    //added by Mike, 20210602
    myBall->keyUp(keyCode);
*/
    
    //added by Mike, 20210903
    myText->keyUp(keyCode);        
}

//added by Mike, 20210514

void OpenGLCanvas::mouseMotionActionDown(int iMouseActionId, int iXPos, int iYPos)
{
    myMouseActionDown[iMouseActionId] = TRUE;
    
    iEndPointX=iXPos;
    iEndPointY=iYPos;
}

//added by Mike, 20210510
//void OpenGLCanvas::mouseActionDown(int iMouseActionId)
void OpenGLCanvas::mouseActionDown(int iMouseActionId, int iXPos, int iYPos)
{
    myMouseActionDown[iMouseActionId] = TRUE;
    
    iStartPointX=iXPos;
    iStartPointY=iYPos;
    iEndPointX=iXPos;
    iEndPointY=iYPos;

/* //removed by Mike, 20210825        
    //added by Mike, 20210517
    if (myButton->isActive()) {
        //	printf("actionUP Start X,Y: %f,%f\n",myUsbongUtils->autoConvertFromPixelToVertexPointX(iStartPointX),myUsbongUtils->autoConvertFromPixelToVertexPointY(iStartPointY));
        
        //we use pixel instead of vertex points
        //	  myButton->collideWithPressedCoordPos(myUsbongUtils->autoConvertFromPixelToVertexPointX(iStartPointX),myUsbongUtils->autoConvertFromPixelToVertexPointY(iStartPointY));
        myButton->collideWithPressedCoordPos(iStartPointX,iStartPointY);
    }
*/    
    
    /*
     printf("hasPressedMouseActionDown");
     
     if (!hasPressedMouseActionDown) {
     printf(">>");
     iStartPointX=iXPos;
     iStartPointY=iYPos;
     
     iEndPointX=iXPos;
     iEndPointY=iYPos;
     
     hasPressedMouseActionDown=true;
     }
     else {
     iEndPointX=iXPos;
     iEndPointY=iYPos;
     }
     */
}
//void OpenGLCanvas::mouseActionUp(int iMouseActionId)
void OpenGLCanvas::mouseActionUp(int iMouseActionId, int iXPos, int iYPos)
{
    myMouseActionDown[iMouseActionId] = FALSE;
    //	printf(">>");
    
    iEndPointX=iXPos;
    iEndPointY=iYPos;
    
/*    
    //added by Mike, 20210513
    printf("actionUP Start X,Y: %f,%f\n",myUsbongUtils->autoConvertFromPixelToVertexPointX(iStartPointX),myUsbongUtils->autoConvertFromPixelToVertexPointY(iStartPointY));
    printf("actionUP End X,Y: %f,%f\n",myUsbongUtils->autoConvertFromPixelToVertexPointX(iEndPointX),myUsbongUtils->autoConvertFromPixelToVertexPointY(iEndPointY));
*/
    
    //Reference: https://www.mathsisfun.com/algebra/line-equation-2points.html;
    //last accessed: 20210513
    //point-slope equation
    //y − y1 = m(x − x1)
    //m = (change in y) / (change in x)
    //where m=slope
    //add: verify if (iEndPointX - iStartPointX) = 0; example: vertical line
    //if yes, output equation: x=iEndPointX
    //int m = (iEndPointY - iStartPointY) / (iEndPointX - iStartPointX);
    
    //added by Mike, 20210513
    //note: pixel position; not vertex position
    //edited by Mike, 20210514
    if ((stepHistoryListCount>=0) && (stepHistoryListCount<MAX)) {
        stepHistoryList[stepHistoryListCount][0]=iStartPointX;
        stepHistoryList[stepHistoryListCount][1]=iStartPointY;
        stepHistoryList[stepHistoryListCount][2]=iEndPointX;
        stepHistoryList[stepHistoryListCount][3]=iEndPointY;
        
        stepHistoryListCount=stepHistoryListCount+1;
    }
    //TO-DO: -add: notify Unit member that count has reached max
    
    
    /*
     hasPressedMouseActionDown=false;
     
     iEndPointX=iXPos;
     iEndPointY=iYPos;
     
     hasPressedMouseActionDown=false;
     */
}

void OpenGLCanvas::render()
{
	//added by Mike, 20210826
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
//	draw(texture, iPilotX, iPilotY);
	//edited by Mike, 20210826
	//draw(texture, iPilotX, iPilotY, mySDLRenderer);	
/* //removed by Mike, 20210827
	openGLDrawTexture(iPilotX, iPilotY, openGLITexture, iTextureWidth, iTextureHeight);	
*/

	//TO-DO: -reverify: this
//	int iLeftMarginColumnCount=3;

  glPushMatrix();
  	//edited by Mike, 20210910
//    myLevel2D->draw();

    //edited by Mike, 20210913
	myLevel2D->drawLevelMapInViewPort(myCanvasPosX, myCanvasPosY, myCanvasPosZ, myPilot->getX(),myPilot->getY(),myPilot->getZ());
    
    //added by Mike, 20210917
    myLevel2D->drawGrid();

/*    
    //TO-DO: -update: this to be in function update(...)
    myCanvasPosX=0;
    myCanvasPosY=0;
    myCanvasPosZ=0;
*/    
  glPopMatrix();

	//edited by Mike, 20210902
 	glPushMatrix();
		myPilot->draw();
  glPopMatrix();
	

/* //removed by Mike, 20210911
    //added by Mike, 20210903
    glPushMatrix();
//    	glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    	myText->draw();
    glPopMatrix();
*/
    
}

//added by Mike, 20210510
//TO-DO: -update: this
void OpenGLCanvas::update()
{
    if (currentState==GAME_SCREEN) {
        //added by Mike, 20210606
        //TO-DO: -add: goal defender, e.g. animal as nature?
 //removed by Mike, 20210825
				//added by Mike, 20210807
        myPilot->update(1); //dt

        
/* //removed by Mike, 20210825       
        //edited by Mike, 20201014
        //        for(i=0; i<MAX_BEAMS; i++) {
        for(int i=0; i<MAX_BEAMS; i++) {
            if ( (myRobotShip->getState()!=ROBOTSHIP_INITIALIZING_STATE) &&
                (myBeam[i]->isActive()) ){
                //edited by Mike, 20210322
                //            myBeam[i]->update(1);
                //TO-DO: reverify: cause of myCanvasPosX, etc does not move
                //            myBeam[i]->update(myCanvasPosX,myCanvasPosY,myCanvasPosZ);
                myBeam[i]->update(myRobotShip->getX(),myRobotShip->getY(),myRobotShip->getZ());
                
                //check collisions
                //myBeam[i]->collideWith(myEnemy);
                
                //added by Mike, 20201016
                for(int a=0; a<MAX_ASTEROID; a++) {
                    myBeam[i]->collideWith(myAsteroid[a]);
                }
            }
        }
        
        //added by Mike, 20201016
        for(int i=0; i<MAX_ASTEROID; i++) {
            //if (myBeam[i]->isActive())
            myAsteroid[i]->update(1);
            //removed by Mike, 20201016
            //            myAsteroid[i]->collideWith(myRobotShip);
            
            //added by Mike, 20210219
            if (myRobotShip->getIsExecutingPunch()) {
                myAsteroid[i]->collideWith(myRobotShip);
            }
        }
*/        
       	//process input
       	//TO-DO: -update: this
       	//TO-DO: -verify: gamepad        
        //TO-DO: -verify: key movement PLUS action KEY_J, KEY_K Command causes Pilot walking movement
        
        //added by Mike, 20201202
       	//verify if no keys are pressed down
       	int iKeyCount;
       	for (iKeyCount=0; iKeyCount<iNumOfKeyTypes; iKeyCount++) {
            if (myKeysDown[iKeyCount]==TRUE) {
                break;
            }
        }
        
        if (iKeyCount==iNumOfKeyTypes) {
/* //removed by Mike, 20210825        
            //TO-DO: -update: this
            myRobotShip->move(-1); //IDLE_MOVING_STATE
*/            
            //added by Mike, 20210423
            myPilot->move(-1);
        }

       	//added by Mike, 20210111; edited by Mike, 20210121
       	//robotship; defend command
        if(myKeysDown[KEY_H] == TRUE)
        {
/* //removed by Mike, 20210825        
            myRobotShip->move(KEY_H);
*/
            myPilot->move(KEY_H);                                    
        }

/* //removed by Mike, 20210827
       	//added by Mike, 20210121
       	//robotship; punch command
        if(myKeysDown[KEY_U] == TRUE)
        {
            myRobotShip->move(KEY_U);
        }
*/
        
       	//edited by Mike, 20201013; edited again by Mike, 20210128
       	//edited by Mike, 20210130
        //    	if ((myKeysDown[KEY_UP] == TRUE) || (myKeysDown[KEY_W] == TRUE))
        if (myKeysDown[KEY_W] == TRUE)
        {
        		//removed by Mike, 20210901; added by Mike, 20210921
            myPilot->move(KEY_W);       
            
            //added by Mike, 20210921
            myCanvasPosY-=myPilot->getStepY();
            						
            //removed by Mike, 20200929
            //			sound->play_sound_clip(thrust);
        }
       	//edited by Mike, 20201013
        //else if(myKeysDown[KEY_DOWN] == TRUE)myWindowWidthAsPixel
        //edited by Mike, 20201014
        if(myKeysDown[KEY_S] == TRUE)
            //    	else if(myKeysDown[KEY_S] == TRUE)
        {
        		//removed by Mike, 20210901; added by Mike, 20210921        
            myPilot->move(KEY_S);               
                
            //added by Mike, 20210921
            myCanvasPosY+=myPilot->getStepY();
        
            //edited by Mike, 20201115; edited again by Mike, 20210128
            //myRobotShip->move(KEY_DOWN);
            //removed by Mike, 20210502
            //            myRobotShip->move(KEY_S);
        }
       
        if (myKeysDown[KEY_D] == TRUE)
        {
/* //removed by Mike, 20210825        
            myRobotShip->move(KEY_D);
*/                        
            //added by Mike, 20210423
            myPilot->move(KEY_D);

/*
						//added by Mike, 20210910            
						//move right
						//Reference: https://community.khronos.org/t/moving-an-object-with-respect-to-the-camera/40968;
						//last accessed: 20201026
						//answer by: Bob, 200002
						myCanvasPosY-=cos(myCanvasLookAtAngle)*myCanvasStepY;
						myCanvasPosX+=sin(myCanvasLookAtAngle)*myCanvasStepX;            
*/
						myCanvasPosX+=myPilot->getStepX();            
            
  
/* //removed by Mike, 20210825                    
            //added by Mike, 20210524
            //      myPilotPlayer2->move(KEY_W);
            myPilotPlayer2->setToWalkingMovingState();
            
            //added by Mike, 20210530
            myPilotPartner->setToWalkingMovingState();
            myPilotPlayer2Partner->setToWalkingMovingState();
*/            
            //removed by Mike, 20200929
            //			sound->play_sound_clip(thrust);
        }
        
        //added by Mike, 20210130
        //note: +reverified due to sequence if-then instructions causes RobotShip to not execute rapid fire down with move up-left, etc
        //using Windows Machine
        //verified: problem does not occur using Linux Machine (with external USB keyboard)
        //added by Mike, 20210131
        //note: add use of external USB keyboard solves the problem
        
       	//edited by Mike, 20201013
        //else if(myKeysDown[KEY_LEFT] == TRUE)
        //edited by Mike, 20201014
        //else if(myKeysDown[KEY_A] == TRUE)
        if(myKeysDown[KEY_A] == TRUE)
        {
/* //removed by Mike, 20210825                
            //added by Mike, 20201001
            //edited by Mike, 20201115
            //edited by Mike, 20210129
            //            myRobotShip->move(KEY_LEFT);            
            myRobotShip->move(KEY_A);
*/            
            //added by Mike, 20210423
            myPilot->move(KEY_A);

/*	//edited by Mike, 20210910
						//added by Mike, 20210910            
						//move left
						//Reference: https://community.khronos.org/t/moving-an-object-with-respect-to-the-camera/40968;
						//last accessed: 20201026
						//answer by: Bob, 200002
						myCanvasPosY+=cos(myCanvasLookAtAngle)*myCanvasStepY;
						myCanvasPosX-=sin(myCanvasLookAtAngle)*myCanvasStepX;            
*/
						myCanvasPosX-=myPilot->getStepX();            
			
			
/* //removed by Mike, 20210825                    
            //added by Mike, 20210524
            //      myPilotPlayer2->move(KEY_W);
            myPilotPlayer2->setToWalkingMovingState();
            
            //added by Mike, 20210530
            myPilotPartner->setToWalkingMovingState();
            myPilotPlayer2Partner->setToWalkingMovingState();
*/            
            
            //removed by Mike, 20201026
            /////			myCanvasPosX+=myCanvasStepX;
                        
            //removed by Mike, 20200929
            //			sound->play_sound_clip(thrust);
        }
        
        //TO-DO: -update: to execute diagonal beams
        //TO-DO: -reverify: rotation angle
        
        //TO-DO: -reverify beam start position during firing
/* //removed by Mike, 20210825                
        //added by Mike, 20210207
        //note: add this diagonal set of instructions first
        //diagonal weapon attack
        if ((myKeysDown[KEY_I]==TRUE) && (myKeysDown[KEY_L]))
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            
            //edited by Mike, 20210207
            //myRobotShip->setCurrentFacingState(FACING_UP);
            myRobotShip->setCurrentFacingState(FACING_RIGHT_AND_UP);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //UP
                    //				rotationAngle=180;
                    //RIGHT AND UP
                    //				rotationAngle=135; //LEFT AND UP
                    rotationAngle=225;
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    //edited by Mike, 20210207
                    //				beamPosXyz[0]+=2.0f; //center
                    //				beamPosXyz[0]+=0.0f; //left arm
                    beamPosXyz[0]+=4.0f; //right arm
                    
                    if (i%2==0) {
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //added by Mike, 20210207
        //LEFT AND UP keys
        if ((myKeysDown[KEY_I]==TRUE) && (myKeysDown[KEY_J]))
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            
            //edited by Mike, 20210207
            //myRobotShip->setCurrentFacingState(FACING_UP);
            myRobotShip->setCurrentFacingState(FACING_LEFT_AND_UP);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //UP
                    //				rotationAngle=180;
                    //RIGHT AND UP
                    rotationAngle=135; //LEFT AND UP
                    //				rotationAngle=225; //RIGHT AND UP
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    //edited by Mike, 20210207
                    //				beamPosXyz[0]+=2.0f; //center
                    //				beamPosXyz[0]+=0.0f; //left arm
                    //				beamPosXyz[0]+=4.0f; //right arm
                    beamPosXyz[0]+=1.0f; //right arm
                    
                    if (i%2==0) {
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //added by Mike, 20210207
        //LEFT AND DOWN keys
        if ((myKeysDown[KEY_J]) && (myKeysDown[KEY_K]==TRUE))
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            
            //edited by Mike, 20210207
            //myRobotShip->setCurrentFacingState(FACING_UP);
            myRobotShip->setCurrentFacingState(FACING_LEFT_AND_DOWN);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //UP
                    //				rotationAngle=180;
                    //RIGHT AND UP
                    //				rotationAngle=135; //LEFT AND UP
                    //				rotationAngle=225; //RIGHT AND UP
                    rotationAngle=45; //LEFT AND DOWN
                    
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    //edited by Mike, 20210207
                    //				beamPosXyz[0]+=2.0f; //center
                    //				beamPosXyz[0]+=0.0f; //left arm
                    //				beamPosXyz[0]+=4.0f; //right arm
                    //removed by Mike, 20210207
                    //				beamPosXyz[0]+=1.0f; //right arm
                    
                    if (i%2==0) {
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //added by Mike, 20210207
        //RIGHT AND DOWN keys
        if ((myKeysDown[KEY_L]) && (myKeysDown[KEY_K]==TRUE))
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            
            //edited by Mike, 20210207
            //myRobotShip->setCurrentFacingState(FACING_UP);
            myRobotShip->setCurrentFacingState(FACING_RIGHT_AND_DOWN);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //UP
                    //				rotationAngle=180;
                    //RIGHT AND UP
                    //				rotationAngle=135; //LEFT AND UP
                    //				rotationAngle=225; //RIGHT AND UP
                    //				rotationAngle=45; //LEFT AND DOWN
                    rotationAngle=-45; //RIGHT AND DOWN
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    //edited by Mike, 20210207
                    //				beamPosXyz[0]+=2.0f; //center
                    //				beamPosXyz[0]+=0.0f; //left arm
                    //				beamPosXyz[0]+=4.0f; //right arm
                    //				beamPosXyz[0]-=1.0f; //right arm
                    beamPosXyz[2]+=2.0f; //right arm
                    
                    if (i%2==0) {
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        
        //edited by Mike, 20210102
        if(myKeysDown[KEY_J] == TRUE)
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            
            //added by Mike, 20210602; removed by Mike, 20210602
            //            myBall->move(KEY_J);
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            //TO-DO: -update: this; we use KEY_I for now
            //myRobotShip->move(KEY_J);
            myRobotShip->move(KEY_I);
            myRobotShip->setCurrentFacingState(FACING_LEFT); //added by Mike, 20210102
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //LEFT
                    rotationAngle=90;
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float beamPosXyz[3] = myRobotShip->getXYZPos();
                    
                    //edited by Mike, 20201225
                    //              myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                    //note: when held, beam particles move in waves
                    //note: move beams based on direction where robot faces
                    if (i%2==0) {
                        //                	myBeam[i]->move(rotationAngle+4, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        //                	myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //edited by Mike, 20201015
        //    	else if(myKeysDown[KEY_L] == TRUE)
        //edited by Mike, 20210102
        if(myKeysDown[KEY_L] == TRUE)
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            //TO-DO: -update: this; we use KEY_I for now
            //myRobotShip->move(KEY_J);
            myRobotShip->move(KEY_I);
            myRobotShip->setCurrentFacingState(FACING_RIGHT);  //added by Mike, 20210102
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    
                    //RIGHT
                    rotationAngle=-90;
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    //edited by Mike, 20210118
                    //				beamPosXyz[2]+=2.0f;
                    beamPosXyz[2]+=1.5f;
                    
                    if (i%2==0) {
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //edited by Mike, 20201015
        //    	else if(myKeysDown[KEY_I] == TRUE)
        if(myKeysDown[KEY_I] == TRUE)
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            myRobotShip->setCurrentFacingState(FACING_UP);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //UP
                    rotationAngle=180;
                    
                    //added by Mike, 20210112
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    beamPosXyz[0]+=2.0f;
                    
                    //edited by Mike, 20201225
                    //              myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                    //note: when held, beam particles move in waves
                    //note: move beams based on direction where robot faces
                    if (i%2==0) {
                        //                	myBeam[i]->move(rotationAngle+4, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle+4,/home/unit_member/Documents/USBONG/pagong-main beamPosXyz);
                    }
                    else {
                        //                	myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
        
        //edited by Mike, 20201015
        //    	else if(myKeysDown[KEY_K] == TRUE)
        //edited by Mike, 20210102
        if(myKeysDown[KEY_K] == TRUE)
        {
            //edited by Mike, 20201013
            static int i = 0;
            
            //added by Mike, 20210112
            if (!myRobotShip->getIsExecuteWithWeaponReady()) {
                return;
            }
            
            //edited by Mike, 20201218
            //myRobotShip->move(KEY_LEFT);
            myRobotShip->move(KEY_I);
            myRobotShip->setCurrentFacingState(FACING_DOWN);
            
            for(i=0; i<MAX_BEAMS; i++) {
                if (!myBeam[i]->isActive()) {
                    //edited by Mike, 20201013
                    //                myBeam[i]->move(myRobotShip->getRotationAngle(), myRobotShip->getXYZPos());
                    
                    //TO-DO: -update: to immediately move a beam if only single press, i.e. not held
                    //TO-DO: -update: to move beam in curve
                    
                    //DOWN
                    rotationAngle=0;
                    
                    //added by Mike, 20210112
                    //TO-DO: -reverify: if causes memory leak problem
                    float *beamPosXyz = {myRobotShip->getXYZPos()};
                    //				float *beamPosXyz[3] = {myRobotShip->getXYZPos()};
                    
                    //edited by Mike, 20201225
                    //              myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                    //note: when held, beam particles move in waves
                    //note: move beams based on direction where robot faces
                    if (i%2==0) {
                        //                	myBeam[i]->move(rotationAngle+4, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle+4, beamPosXyz);
                    }
                    else {
                        //                	myBeam[i]->move(rotationAngle, myRobotShip->getXYZPos());
                        myBeam[i]->move(rotationAngle, beamPosXyz);
                    }
                    
                    //		        sound->play_sound_clip(beam);
                    return;
                }
            }
        }
*/

/* //edited by Mike, 20210830        
        //edited by Mike, 20210727
        //note: we verify if we continue with step, hit collision
        //if so, we do not add step to position
//        if (myPilot->getXAsPixel() < 0) {
        if (myPilot->getXAsPixel() -myPilot->getStepX() < 0) {        
            myPilot->setXPosAsPixel(0+myPilot->getStepX());
        }
        //max movement with set
//        else if (myPilot->getXAsPixel()+myPilot->getWidthAsPixel() > myWindowWidthAsPixel) {
        else if (myPilot->getXAsPixel()+myPilot->getWidthAsPixel() +myPilot->getStepX() > myWindowWidthAsPixel) {
            myPilot->setXPosAsPixel(myWindowWidthAsPixel-myPilot->getWidthAsPixel()-myPilot->getStepX());
        }
        
//added by Mike, 20210727
        //note: we use y-axis in Level2D; instead of z-axis (Level3D)
        //TO-DO: -reverify: to use z-axis in Level2D
        //edited by Mike, 20210727
//        if (myPilot->getYAsPixel()  < 0) { //max movement with set
        if (myPilot->getYAsPixel() -myPilot->getStepY() < 0) { //max movement with set
            myPilot->setYPosAsPixel(0+myPilot->getStepY());
        }
        //max movement with set
//        else if (myPilot->getYAsPixel()+myPilot->getHeightAsPixel() > myWindowHeightAsPixel) {
        else if (myPilot->getYAsPixel()+myPilot->getHeightAsPixel() +myPilot->getStepY() > myWindowHeightAsPixel) {
            myPilot->setYPosAsPixel(myWindowHeightAsPixel-myPilot->getHeightAsPixel()-myPilot->getStepY());
        }
*/
        
        //note: we verify if we continue with step, hit collision
        //if so, we do not add step to position
        if (myPilot->getX() -myPilot->getStepX() < 0) {        
        		//edited by Mike, 20210916
//            myPilot->setXPos(0+myPilot->getStepX());
            myPilot->setXPos(0);
        }
        
        //max movement with set
        else if (myPilot->getX()+myPilot->getWidth() +myPilot->getStepX() > myWindowWidth) {
            myPilot->setXPos(myWindowWidth-myPilot->getWidth()-myPilot->getStepX());
        }
        
				//added by Mike, 20210727
        //note: we use y-axis in Level2D; instead of z-axis (Level3D)
        //TO-DO: -reverify: to use z-axis in Level2D
        if (myPilot->getY() -myPilot->getStepY() < 0) { //max movement with set
        		//edited by Mike, 20210921
//            myPilot->setYPos(0+myPilot->getStepY());
            myPilot->setYPos(0);            
        }
        //max movement with set
        else if (myPilot->getY()+myPilot->getHeight() +myPilot->getStepY() > myWindowHeight) {
            myPilot->setYPos(myWindowHeight-myPilot->getHeight()-myPilot->getStepY());
        }
        
        //added by Mike, 20210922
        //TO-DO: -add: action if max canvas pos x reached
        if (myCanvasPosX<0) {
            myCanvasPosX=0;
        }

        //TO-DO: -add: action if max canvas pos x reached
        if (myCanvasPosY<0) {
            myCanvasPosY=0;
        }

        
/*	//removed by Mike, 20210920        
        //added by Mike, 20210911; edited by Mike, 20210913
        if (myCanvasPosX<0) {
            myCanvasPosX=0;
        }
        else if (myCanvasPosX>myWindowWidth) {
            myCanvasPosX=myWindowWidth;
        }

        if (myCanvasPosY<0) {
            myCanvasPosY=0;
        }
        else if (myCanvasPosY>myWindowHeight) {
            myCanvasPosY=myWindowHeight;
        }        
        //--
*/
        
    }
    else if (currentState==TITLE_SCREEN)
    {
    }
    else if (currentState==CONTROLS_SCREEN)
    {
    }
    else if (currentState==HIGHSCORE_SCREEN)
    {
    }
    else if (currentState==GAMEOVER_SCREEN)
    {
        /*		//removed by Mike, 20200929
         if(myKeysDown[KEY_SPACE] == TRUE)
         {
         gameReset();
         }
         */
    }
}

void OpenGLCanvas::gameReset(){
    /*	//removed by Mike, 20200929
     score=0;
     scoreBeforeBonus=0;
     myRobotShip->reset();
     rest=MAX_LIFE;
     resetDynamicObjects();
     changeState(GAME_SCREEN);
     */
}

void OpenGLCanvas::resetDynamicObjects(){
    /*	//removed by Mike, 20200929
     myAsteroid[0]->reset(15,15); //TOP RIGHT
     myAsteroid[1]->reset(-15,15); //TOP LEFT
     myAsteroid[2]->reset(-15,-15); //BOTTOM LEFT
     myAsteroid[3]->reset(15,-15); //BOTTOM RIGHT
     for (i=4; i<12; i++) {
     myAsteroid[i]->changeState(CHILD_STATUS);
     myAsteroid[i]->setCollidable(false);
     }
     for (i=12; i<MAX_ASTEROID; i++) {
     myAsteroid[i]->changeState(GRANDCHILD_STATUS);
     myAsteroid[i]->setCollidable(false);
     }
     //for (i=0; i<MAX_ASTEROID; i++)
     numOfAsteroidsLeft=MAX_ASTEROID;
     
     for (i=0; i<MAX_BEAMS; i++)
     myBeam[i]->changeState(HIDDEN_STATE);
     */
}

void OpenGLCanvas::addScore(int s)
{
    /*	//removed by Mike, 20200929
     score+=s;
     scoreBeforeBonus+=s;
     if (scoreBeforeBonus>=REQUIRED_POINTS_FOR_BONUS) {
     sound->play_sound_clip(bonus);
     rest++;
     scoreBeforeBonus-=REQUIRED_POINTS_FOR_BONUS;
     }
     numOfAsteroidsLeft--;
     */
}
void OpenGLCanvas::loseLife()
{
    /*	//removed by Mike, 20200929
     rest-=1;
     if (rest==0) {
     if(score>highScore)
     {
     //save score in a text file
     highScore=score;
     out = fopen(outfile, "w");
     fprintf(out,"%d",highScore);
     fclose(out);
     changeState(HIGHSCORE_SCREEN);
     sound->play_sound_clip(title);
     }
     else changeState(GAMEOVER_SCREEN);
     }
     */
}
void OpenGLCanvas::changeState(int s)
{
    currentState=s;
}
