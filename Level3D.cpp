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
 * @date updated: 20211001
 * @website address: http://www.usbong.ph
 *
 * Reference:
 * 1) Astle, D. and Hawkin, K. (2004). "Beginning OpenGL game programming". USA: Thomson Course Technology
 *
 * 2) https://www.mathsisfun.com/sine-cosine-tangent.html;
 *	 last accessed: 20210803
 *
 * 3) https://www.libsdl.org/download-2.0.php;
 * 	 last accessed: 20210816
 * --> SDL Installation Intructions on LUBUNTU (20.04); 
 * --> https://askubuntu.com/questions/786300/how-to-install-all-sdl-libraries-in-ubuntu-14-04;
 * --> last accessed: 20210816
 *
 * Note:
 * 1) SDL installation on Linux machine; 
 * --> Terminal Commands (select Libraries, version Downgrade to Insall)
 * --> sudo apt-get install aptitude
 * --> sudo aptitude install libsdl2-dev
 *
 * 2) SDL Image Tool installtion on Linux machine;
 * --> Terminal Commands (select Libraries, version Downgrade to Insall)
 * --> sudo aptitude install libsdl2-image-dev
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007):
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */
 
//added by Mike, 20210901
//note: re-verify: SUZUKI, YU and team @SEGA 1993~; DAYTONA USA with alps?
//create component parts of world as journey; find game inside as surprise...?

 
//added by Mike, 20210130
//reverify: Robotship does not execute correctly
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

//added by Mike, 20210807
//added: in draw tile sprite texture image whose width and height is over its tile width and height used in collision detection

//added by Mike, 20210815
//TO-DO: -reverify: cause why on macOS tile positions not exact with OpenGLCanvas.cpp grid lines; Window Width x Height; 1280 x 800

#include <stdio.h>
#include <math.h>

//added by Mike, 20210719
#include <iostream>

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

//added by Mike, 20210829
#ifdef _WIN32 //Windows machine
    #include <SDL.h>
    #include <SDL_image.h>
#elif defined(__APPLE__)
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#endif

/* //removed by Mike, 20210826
#include "Button.h"
*/

/* //TO-DO: -add: these
 #include "PolygonUtils.h"
 #include "ModelPool.h"
 */

//added by Mike, 20210613
#include "Level3D.h"


//added by Mike, 20210614; removed by Mike, 20210703
//#include "Font.h"

//added by Mike, 20210516
#include "UsbongUtils.h"

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
    
    //added by Mike, 20201226
    iNumOfKeyTypes
};

//TO-DO: -put: in MyDynamicObject
GLboolean Level3D::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

//edited by Mike, 20201001
//Level3D::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Level3D::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210625
//Level3D::Text(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Level3D::Level3D(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{
    //edited by Mike, 20201001
    //currentState=IN_TITLE_STATE;//MOVING_STATE;
    currentState=MOVING_STATE;
    //added by Mike, 20201201
    currentMovingState=IDLE_MOVING_STATE;
    
    fCountTotalFrames=16.0f; //added by Mike, 20210903
   
    invincibleCounter=0;
    currentDeathFrame=0;
       
    //TO-DO: -update: this
    //note: float xPos as parameter to int myXPosAsPixel not correct output
    //edited by Mike, 20210613
    myXPosAsPixel=(int)xPos;//320;//(int)xPos;
    myYPosAsPixel=(int)yPos;
    myZPosAsPixel=(int)zPos;
    
    //added by Mike, 20210923
    fMyCanvasPosX=0;
    fMyCanvasPosY=0;
    fMyCanvasPosZ=0;
    
    //    printf(">>myXPosAsPixel: %i\n",myXPosAsPixel);
    
    
    /*
     myWidth=0.5f;
     myHeight=0.5f;
     */
    //added by Mike, 20201001; edited again by Mike, 20201001
    //note: initial position, top-left
    /*    myXPos=0.0f;
     myYPos=0+myHeight;//0.1f;
     */
    
    myXPos=xPos;
    myYPos=yPos;
    myZPos=zPos;
    
    fMyWindowWidth=fWindowWidth;
    fMyWindowHeight=fWindowHeight;
    
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0;
      
    //edited by Mike, 20210724; edited by Mike, 20210921
//    iRowCountMax=10;
    iRowCountMax=10+2;
    
    //edited by Mike, 20210920
    //note: column 1 NOT anymore drawn to eliminate noticeable drawing by computer during scroll movement; 
    //start column: 0;
//    iColumnCountMax=18;
//    iColumnCountMax=18+1;
    iColumnCountMax=18+2;

    iHeightCountMax=10;
        
    fGridSquareWidth = (fMyWindowWidth)/iColumnCountMax; //example: 136.60
    fGridSquareHeight = (fMyWindowHeight)/iRowCountMax; //example: 76.80
    
    printf("Level3D.cpp; fGridSquareWidth: %f",fGridSquareWidth); //75.888885, instead of 75.000000
    
    //auto-set width and height based on grid tile
    myWidthAsPixel=fGridSquareWidth;
    myHeightAsPixel=fGridSquareHeight;    
/*
    //edited by Mike, 20210910
    //added by Mike, 20210901
    stepX=fGridSquareWidth/10/4; //0.2    
    stepY=fGridSquareHeight/10/4; //0.2;    
    stepZ=fGridSquareWidth/10/4; //0.2;
*/
/*
    stepX=fGridSquareWidth/10/2;
    //edited by Mike, 20210917
    stepY=fGridSquareHeight/10/2;
//    stepY=fGridSquareHeight/10/4;

    stepZ=fGridSquareWidth/10/2;
*/
    stepX=fGridSquareWidth/10;
    stepY=fGridSquareHeight/10;
    stepZ=fGridSquareWidth/10;
    
    
    //added by Mike, 20210910
    fMyCanvasPosPrevX=0.0f;
    fMyCanvasPosPrevY=0.0f;
    fMyCanvasPosPrevZ=0.0f;
    
    //edited by Mike, 20210918
    iCurrentLevelMapContainerOffsetZ=0;
    iCurrentLevelMapContainerOffsetX=0;
    iCurrentLevelMapContainerOffsetY=0;

/*
		//TO-DO: -update: tile collision instructions due to uses integer,
		//causes incorrect output when movement has fraction using float number classification
		//edited by Mike, 20210920
    iCurrentLevelMapContainerOffsetZ=-1;
    iCurrentLevelMapContainerOffsetX=-1;
    iCurrentLevelMapContainerOffsetY=-1;    
*/

/*
    iCurrentLevelMapContainerOffsetZ=1;
    iCurrentLevelMapContainerOffsetX=1;
    iCurrentLevelMapContainerOffsetY=1;    
*/
    //added by Mike, 20210911
    MAX_X_AXIS_VIEWPORT=iColumnCountMax;//*fGridSquareWidth;
    MAX_Y_AXIS_VIEWPORT=iRowCountMax;//*fGridSquareHeight;
   
/*//removed by Mike, 20210913   
    fPrevX=0.0f;
    fPrevY=0.0f;
    fPrevZ=0.0f;
*/
    
    fStepMovemenGridZ=0;
    fStepMovemenGridX=0;
	fStepMovemenGridY=0;
    
    
    /*
     printf("fGridSquareWidth: %f\n",fGridSquareWidth);
     printf("fGridSquareHeight: %f\n",fGridSquareHeight);
     */
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0; //added by Mike, 20210701
    
    //added by Mike, 20210516
    myUsbongUtils = new UsbongUtils();
    myUsbongUtils->setWindowWidthHeight(fMyWindowWidth, fMyWindowHeight); //added by Mike, 20210626
    
    
/*    //edited by Mike, 20210831
    myWidth=16.0f;
    myHeight=16.0f;
*/
    myWidth=fGridSquareWidth; //64; //16;
    myHeight=fGridSquareHeight; //64; //16;
        
    
//    printf("Level3D.cpp myWindowWidth: %f\n",myWindowWidth); 
    
    //added by Mike, 20210618
    isAtMaxTextCharRow=false;
    idrawPressNextSymbolCount=0;
    
    //removed by Mike, 20201001; added by Mike, 20210423;
    //removed by Mike, 20210722
//    setup();
    
    setCollidable(true);
/*    
    if (checkIsCollidable()) {
    	printf(">>COLLIDABLE");
    }
*/    
    //added: function to be reusable
    //    readInputText("inputHalimbawa.txt");
    //edited by Mike, 20210712
    //    readInputText("inputLevel1.csv");
    read((char*)"inputLevel1.csv");
    
    //TO-DO: -update: this
		//openGLITexture = openGLLoadTexture((char*)"textures/Level3D.png", fMyWindowWidth, fMyWindowHeight);
		//edited by Mike, 20210928
//    openGLITexture = openGLLoadTexture((char*)"textures/Level2D.png", fMyWindowWidth, fMyWindowHeight);
		//edited by Mike, 20210929
//    openGLITexture = openGLLoadTexture((char*)"textures/Level2D.png", myWidth, myHeight);
		//edited by Mike, 20210929
		//OK
//		openGLITexture = openGLLoadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png", myWidth, myHeight);	
//    openGLITexture = openGLLoadTexture((char*)"textures/Level3D.png", myWidth, myHeight);
		openGLITexture = openGLLoadTexture((char*)"textures/level3D.png", myWidth, myHeight);	


   printf(">>>>openGLITexture: %i",openGLITexture);
}

Level3D::~Level3D()
{
}

//added by Mike, 20210826
//TO-DO: -add: CAD tool to assist in identify excess markings in image file
//-add: CAD tool to verify animating sequence
//reminder: we use floating point type, instead of integer to receive exact values after computing as input the screen width and height 
//solution: -reverify: saved .png output file; action: reused imageSpriteExampleMikeWithoutBG.xcf
void Level3D::openGLDrawTextureQuadOKNotYetCube(float x, float y, float textw, float texth, std::string sTileId)
{
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
		
//    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>openGLITexture: %i",openGLITexture);

/*    
	float fTaoAnimationFrameOffset=0.0f;
	float fTaoAnimationFrameOffsetYAxis=0.0f;
*/	


	//added by Mike, 20210927
	//note: updated: texture and vertex positions to be clock-wise 1st quadrant
	//removed by Mike, 20210928
/*	
		glTranslatef(-textw,-texth,0.0f);
		glColor3f(1.0f, 1.0f, 1.0f); // white
*/

		//added by Mike, 20210830
   	float fTx = 0.0f;
    float fTy = 0.0f;
    
    //added by Mike, 20210725; removed by Mike, 20210725
    //sTileId="0-0";
    std::cout << "sTileId: " << sTileId << "\n";
	
	  fTx = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
    fTy = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    
	
    std::cout << "fTx: " << fTx << "\n";
    std::cout << "fTy: " << fTy << "\n";
	

	//added by Mike, 20210928
	//note: updated: texture and vertex positions to be clock-wise 1st quadrant
	glTranslatef(-textw,-texth,0.0f);
//	glTranslatef(-textw*2,-texth*2,0.0f);

	//added by Mike, 20210918
	glColor3f(1.0f, 1.0f, 1.0f); // white
//	glColor3f(0.0f, 0.0f, 1.0f); // blue
	
	
	//TO-DO: -reverify: rotated x-axis to add height in view, instead of flat due to TOP-VIEW
	//TO-DO: -update: instructions to use cubes
  //added by Mike, 20210929
//  glRotatef(45,1.0f,0.0f,0.0f);
//  glRotatef(45,1.0f,0.0f,1.0f);
	//isometric view of auto-drawn objects; 
	//Recommended Reference: Newsletter 2020-09;
	//https://www.usbong.ph/excel/excel-2020-09; last accessed: 20210929
/* //edited by Mike, 20210930;
	//note: there exists space between tiles as quad; NOT yet cube
  glRotatef(45,1.0f,0.0f,0.0f);
	glRotatef(60,0.0f,0.0f,1.0f);
*/
//  glRotatef(45,1.0f,0.0f,0.0f);
  glRotatef(30,1.0f,0.0f,0.0f);
	glRotatef(60,0.0f,0.0f,1.0f);

	
  if (sTileId.compare("0-2") == 0) {//True    
/* //removed by Mike, 20210929  
  		//TO-DO: -reverify: this
      glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f-textw, 0.0f-texth, 0.0f);
        glVertex3f(0.0f, 0.0f-texth, 0.0f);
      glEnd();
*/      
  }
  else {
/*    
    //note: 3rd quadrant; counter clock-wise
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-myWidth, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-myWidth, 0.0f-myHeight, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-myHeight, 0.0f);
   	glEnd();
*/
/*   	  
 //removed by Mike, 20210928
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //note: 1st quadrant; clock-wise
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f, 0.0f+texth, 0.0f);
    	glVertex3f(0.0f+textw, 0.0f+texth, 0.0f);    	
    	glVertex3f(0.0f+textw, 0.0f, 0.0f);    	
   	glEnd();
*/   	
 	}
		//edited by Mike, 20210930
		//TO-DO: -add: more quads to make Cube
		//TO-DO: -reverify: this

		//set vertex clock-wise; 1st quadrant; NOT inverted Y-axis;
		//texture position clock-wise
		//texture: TOP-LEFT origin
		//texture positions U shape, clock-wise			
		glBegin(GL_QUADS);
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, y, 0);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, y + texth, 0);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, y + texth, 0);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, y, 0);
		glEnd();

    
    glDisable(GL_TEXTURE_2D);
    
    //removed by Mike, 20210929
//    glBindTexture(GL_TEXTURE_2D, 0); //added by Mike, 20210918
}

//note: draw tile as cube sequence left to right, top to bottom;
//right-most over left-most
//bottom-most over top-most
void Level3D::openGLDrawTexture(float x, float y, float z, float textw, float texth, std::string sTileId)
{
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
		
		//added by Mike, 20210830
   	float fTx = 0.0f;
    float fTy = 0.0f;
    
    //added by Mike, 20210725; removed by Mike, 20210725
    //sTileId="0-0";
    std::cout << "sTileId: " << sTileId << "\n";
	
	  fTx = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
    fTy = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    
	
    std::cout << "fTx: " << fTx << "\n";
    std::cout << "fTy: " << fTy << "\n";
	

	//added by Mike, 20210928
	//note: updated: texture and vertex positions to be clock-wise 1st quadrant
	glTranslatef(-textw,-texth,0.0f);

	//added by Mike, 20210918
	glColor3f(1.0f, 1.0f, 1.0f); // white
//	glColor3f(0.0f, 0.0f, 1.0f); // blue
	
	
	//isometric view of auto-drawn objects; 
	//Recommended Reference: Newsletter 2020-09;
	//https://www.usbong.ph/excel/excel-2020-09; last accessed: 20210929
/* //edited by Mike, 20210930;
	//note: there exists space between tiles as quad; NOT yet cube
  glRotatef(45,1.0f,0.0f,0.0f);
	glRotatef(60,0.0f,0.0f,1.0f);
*/
  glRotatef(30,1.0f,0.0f,0.0f);
	glRotatef(60,0.0f,0.0f,1.0f);

	//TO-DO: -eliminate excess instructions
	
  if (sTileId.compare("0-2") == 0) {//True    
/* //removed by Mike, 20210929  
  		//TO-DO: -reverify: this
      glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f-textw, 0.0f-texth, 0.0f);
        glVertex3f(0.0f, 0.0f-texth, 0.0f);
      glEnd();
*/      
  }
  else {
/*    
    //note: 3rd quadrant; counter clock-wise
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f-myWidth, 0.0f, 0.0f);    	
    	glVertex3f(0.0f-myWidth, 0.0f-myHeight, 0.0f);    	
    	glVertex3f(0.0f, 0.0f-myHeight, 0.0f);
   	glEnd();
*/
/*   	  
 //removed by Mike, 20210928
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //note: 1st quadrant; clock-wise
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	glVertex3f(0.0f, 0.0f, 0.0f);   	
    	glVertex3f(0.0f, 0.0f+texth, 0.0f);
    	glVertex3f(0.0f+textw, 0.0f+texth, 0.0f);    	
    	glVertex3f(0.0f+textw, 0.0f, 0.0f);    	
   	glEnd();
*/   	
 	}

		//set vertex clock-wise; 1st quadrant; NOT inverted Y-axis;
		//texture position clock-wise
		//texture: TOP-LEFT origin
		//texture positions U shape, clock-wise			
/* //edited by Mike, 20210930
		glBegin(GL_QUADS);
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, y, 0);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, y + texth, 0);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, y + texth, 0);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, y, 0);
		glEnd();
*/
		//note: wire line NOT all sides auto-drawn
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBegin(GL_QUADS);
		
			//note: texture coordinate positions equal with all faces		
/*			
			// bottom face; OK; isometric rotated, now still bottom side
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, y, 0+texth);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, y + texth, 0+texth);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, y + texth, 0+texth);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, y, 0+texth);			
*/
			// top face; OK; isometric rotated, now still top side
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, y, 0);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, y + texth, 0);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, y + texth, 0);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, y, 0);
						
			// front face; OK; isometric rotated, now right side
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, 0, z);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, 0, z + texth);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, 0, z + texth);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, 0, z);

/*						
			// back face; OK; isometric rotated, now left side
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(x, 0 + texth, z);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(x, 0 + texth, z + texth);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(x + textw, 0 + texth, z + texth);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(x + textw, 0 + texth, z);
*/
	
			
			// right face; OK; isometric rotated, now top-right side 
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(0, y, z);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(0, y + texth, z);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(0, y + texth, z + texth);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(0, y, z + texth);
												
			// left face; OK; isometric rotated, now bottom-left side 
			glTexCoord2f(fTx+0.0625f, fTy+0.0625f);		
			glVertex3f(0 + textw, y, z);

			glTexCoord2f(fTx+0.0625f, 0+fTy);
			glVertex3f(0 + textw, y + texth, z);
			
			glTexCoord2f(0+fTx, 0+fTy);				
			glVertex3f(0 + textw, y + texth, z + texth);

			glTexCoord2f(0+fTx, fTy+0.0625f);
			glVertex3f(0 + textw, y, z + texth);

		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDisable(GL_TEXTURE_2D);
    
    //removed by Mike, 20210929
//    glBindTexture(GL_TEXTURE_2D, 0); //added by Mike, 20210918
}

//edited by Mike, 20210929
//void Level3D::openGLDrawTexture(float x, float y, float textw, float texth)
//instructions from Pilot.cpp
void Level3D::openGLDrawTextureOK(float x, float y, float textw, float texth, std::string sTileId)
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
	glTranslatef(-textw,-texth,0.0f);

	//added by Mike, 20210918
	glColor3f(1.0f, 1.0f, 1.0f); // white

	float fTaoAnimationFrameOffset=0;
	float fTaoAnimationFrameOffsetYAxis=0;
		
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
			
			
	glDisable(GL_TEXTURE_2D);
}

//added by Mike, 20210827
//TO-DO: -remove: this
void Level3D::setupLevel(int myLevelTextureObject)
{
    //removed by Mike, 20201010
    //due to blank output
    //glEnable(GL_DEPTH_TEST);
    
    iLevelTextureObject = myLevelTextureObject;
    
    /* select texture 1 */
    glBindTexture(GL_TEXTURE_2D, iLevelTextureObject);
    
    /* create OpenGL texture out of targa file */
/* //removed by Mike, 20210826    
    //edited by Mike, 20210420; edited again by Mike, 20210818
		//TO-DO: -add: version using SDL without OpenGL    
    load_tga("textures/Level3D.tga");
*/        
//    load_png("textures/Level3D.tga");
    
    //    load_tga("textures/concrete.tga");
    
    /* set texture parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       
/*	//edited by Mike, 20210722; this is due to displayed image is blurred
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);                    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
    
    /* unselect texture myFontTextureObject */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /* setup alpha blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
/* //removed by Mike, 20210722    
    // set background color to bluish //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // unselect texture myFontTextureObject
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // setup alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
*/    
    //removed by Mike, 20201012
    /* set background color to bluish to demonstrate font transparency */
    //    glClearColor(0.0f, 0.0f, 0.25f, 1.0f); /* to demonstrate font transparency */
    
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Level3D::getXYZPos()
{
    //float myXYZ[3];
    float* myXYZ;
    myXYZ = (float*) malloc(3*sizeof(float));
    
    myXYZ[0]=myXPos;
    myXYZ[1]=myYPos;
    myXYZ[2]=myZPos;
    
    return myXYZ;
}

//edited by Mike, 20210712
void Level3D::drawTileAsQuadWithoutTexture()
{
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPushMatrix();
    
    //glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //  glColor3f(0.0f, 0.0f, 0.0f); //set to default, i.e. black
    glColor3f(1.0f, 0.0f, 0.0f); //red
    
    //TO-DO: -add: tile with auto-drawn pattern; without using image texture object
    //  	printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>> HALLO");
    
    //  	printf (">>> fGridSquareWidth: %f; fGridSquareHeight: %f",fGridSquareWidth,fGridSquareHeight);
    
    
    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
    
    //TO-DO: -reverify: cause of /2.0f/4.0f; and 2.0f/2.0f;
    //		fGridTileWidthVertexPosition = fGridTileWidthVertexPosition/2.0f/3.0f;
    fGridTileWidthVertexPosition = fGridTileWidthVertexPosition/2.0f/4.0f;
    fGridTileHeightVertexPosition = fGridTileHeightVertexPosition/2.0f/2.0f;
    
    //added by Mike, 20210713
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
#if defined(__APPLE__)
    //note: right border of tile only partly visible
    fGridTileWidthVertexPosition=fGridTileWidthVertexPosition+0.0006f;
#endif
    
    //note: vertex position sequence to be auto-drawn
    //counter-clockwise sequence to auto-draw front face
    
    //add this due to 3rd quadrant
    //size of tile width: 0.1f
    //		glTranslatef(-0.1f-0.05f, 0.0f, 0.0f);
    //re-verify: cause of 0.01f; due to Linux machine?
    //		glTranslatef(-0.1f-0.05f-0.01f, 0.0f, 0.0f);
    
    //TO-DO: -update: this
    //note: 3rd quadrant
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    //        glTexCoord2f(tx, ty);
    glVertex3f(0.0f, 0.0f, 0.0f);
    
    //        glTexCoord2f(tx + 0.078125f, ty);
    glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);
    
    //        glTexCoord2f(tx + 0.078125f, ty + 0.125f);
    glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    
    //				glTexCoord2f(tx, ty + 0.125f);
    glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	glEnd();
    
    glPopMatrix();
}

//edited by Mike, 20210716; edited by Mike, 20210828
//void Level3D::drawTileAsQuadWithTexture()
//edited by Mike, 20210720
//void Level3D::drawTileAsQuadWithTexture(std::string sTileId)
void Level3D::drawTileAsQuadWithTexturePrev(std::string sTileId)
{

	//added by Mike, 20210827
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


		//added by Mike, 20210803
		sTileId = myUsbongUtils->autoDeleteQuotationMark(sTileId);
//std::cout << "sTileId: " << sTileId << "\n";

    //edited by Mike, 20210827
    //glBindTexture(GL_TEXTURE_2D, iLevelTextureObject);
		glBindTexture(GL_TEXTURE_2D, openGLITexture);
    glEnable(GL_TEXTURE_2D);
    	    
    
    //added by Mike, 20210809
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    //notes: TO-DO: -reverify: indexed 64 colors max segmentation fault problem
    //16x16pixels; 256x256pixels width x height .tga image file
    //texture coordinates;
    //width: 1.0/16.0 = 0.0625
    //height: 1.0/16.0 = 0.0625
    
    /*
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
    glPushMatrix();

    float fGridTileWidthVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fGridSquareWidth);
    float fGridTileHeightVertexPosition = myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fGridSquareHeight);
   
    //added by Mike, 20210713
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
        
    //added by Mike, 20210720; edited by Mike, 20210814
    fGridTileWidthVertexPosition=1.0f-fGridTileWidthVertexPosition;
//    fGridTileWidthVertexPosition=1.0f-(fGridTileWidthVertexPosition)+1.0/18;  
    fGridTileHeightVertexPosition=1.0f-fGridTileHeightVertexPosition; //note: +, instead of -

    //note: vertex position sequence to be auto-drawn
    //counter-clockwise sequence to auto-draw front face

    //add this due to 3rd quadrant
    //size of tile width: 0.1f
    //		glTranslatef(-0.1f-0.05f, 0.0f, 0.0f);
    //re-verify: cause of 0.01f; due to Linux machine?
    //		glTranslatef(-0.1f-0.05f-0.01f, 0.0f, 0.0f);

    //edited by Mike, 20210801
    glTranslatef(0.0f, fGridTileHeightVertexPosition, 0.0f);
    
    float fTx = 0.0f;
    float fTy = 0.0f;
    
    //added by Mike, 20210725; removed by Mike, 20210725
    //sTileId="0-0";
//    std::cout << "sTileId: " << sTileId << "\n";
		
	  fTx = 0.0f+0.0625f*(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)); //column
		
		//edited by Mike, 20210721    
    //texture y-axis; start from bottom; anchor
    //CAPCOM's GHOSTS'N GOBLINS? (Arcade, Family Computer; Year 1985) 
    //May tattoo ng puso ang braso ng mga Dambuhalang Goblin.
    //Umaaligid-aligid sila sa loob ng Gusali;
    //paalala: Goblin = uri ng halimaw; bakemono, i.e. naibang bagay
    //Maaaring dalawang set ng pagsalakay upang mapuksa ang Dambuhalang Goblin.
    //May tapang din ang pagtalon mula sa itaas na palapag upang magbalik at sumalakay muli; 
    //Ito'y sa halip na harapin ang halimaw, at tanggapin nang nakikipaglaban ang tiyak na kamatayan.
    //paalala: ang patay, patay na.
		//16.0f due to tile x16 to be height max
		//0.0625f*16.0f=1.0f
    fTy = 1.0f-0.0625f*(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)); //row    

/*    	
		printf(">>>%i\n",(myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId)));
		printf(">>>%i\n",(myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId)));
*/

//    printf(">>>fTx: %f;fTy: %f\n",fTx,fTy);

  	
  	//added by Mike, 20210814
  	//note: texture coordinates
    float fTileSideXAxis = 0.0625f;

    //from bottom; anchor; start fTy at 1.0f
    //edited by Mike, 20210827
//    float fTileSideYAxis = -0.0625f;
    float fTileSideYAxis = -0.0625f;

		//added by Mike, 20210724
		//TO-DO: -add: animation sequence based on sTileId
		//background color of tile
//-----

    glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
    //    glColor3f(1.0f, 0.0f, 0.0f); //red

		//added by Mike, 20210803
//std::cout << "sTileId: " << sTileId << "\n";
//		sTileId = myUsbongUtils->autoDeleteQuotationMark(sTileId);

    //edited by Mike, 20210803
    //triangle tile with 90degrees angle
    if (sTileId.compare("0-2") == 0) {//True    
        glBegin(GL_TRIANGLES);
          glVertex3f(0.0f, 0.0f, 0.0f);
          glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
          glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
        glEnd();
    }
    else {
      //note: 3rd quadrant; counter clock-wise
      glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	  glVertex3f(0.0f, 0.0f, 0.0f);   	
    	  glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);    	
    	  glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);    	
    	  glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
   	  glEnd();
    }
        
//-----		

    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
      glTexCoord2f(fTx + fTileSideXAxis, fTy);
      glVertex3f(0.0f, 0.0f, 0.0f);
    
      glTexCoord2f(fTx, fTy);
      glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f, 0.0f);

      glTexCoord2f(fTx, fTy + fTileSideYAxis);
      glVertex3f(0.0f-fGridTileWidthVertexPosition, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    
      glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
      glVertex3f(0.0f, 0.0f-fGridTileHeightVertexPosition, 0.0f);
    glEnd();
/*    
	//added by Mike, 20210827
	//set vertex counter-clock-wise
	glBegin(GL_QUADS);
    glTexCoord2f(fTx + fTileSideXAxis, fTy);
		glVertex3f(0.0f, 0.0f, 0);
		
    glTexCoord2f(fTx, fTy);
		glVertex3f(0.0f + fGridTileWidthVertexPosition, 0.0f, 0);
		
    glTexCoord2f(fTx, fTy + fTileSideYAxis);
		glVertex3f(0.0f + fGridTileWidthVertexPosition, 0.0f + fGridTileHeightVertexPosition, 0);
		
    glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
		glVertex3f(0.0f, 0.0f + fGridTileHeightVertexPosition, 0);
	glEnd();    
*/    
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
}

//added by Mike, 20210917
void Level3D::drawGrid() {
/*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
*/
  //TO-DO: -reverify: texture drawing; auto-drawn when computer received movement COMMAND
  //note: CAPCOM Mickey Mouse series, e.g. Circus Mystery; Super Family Computer version
  //コンピュータ　プログラマー、どうなったのかな？
  //こっちの将来的な方向がわかるようになる・・・
  //Pilot + Mecha Robot Ship...
    

//    glPushMatrix();
    
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    /*     //added by Mike, 20210918
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     */
    
     //---------------------
     //part 1: draw tile grid
     //---------------------
     iRowCountMax=10;
     iColumnCountMax=18;
     
     // Draw a Green Line
     //rows
     for (int iRowCount=0; iRowCount<=iRowCountMax; iRowCount++) {
     	 // Draw a Green Line top-left origin
     	 //horizontal line
     	 glBegin(GL_LINES);
     	   glColor3f(0.0f, 0.8f, 0.0f); // Green
/*	//edited by Mike, 20210927     	
     		glVertex2f(0.0f, fGridSquareHeight*iRowCount);    // x, y
     		glVertex2f(fGridSquareWidth*iColumnCountMax, fGridSquareHeight*iRowCount);
*/     		
				 // x, y
     		 glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f), 
     		 						myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCount));
     		 glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCountMax), 
     		 						myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCount));
       glEnd();
     }
     
     //columns
     for (int iColumnCount=0; iColumnCount<=iColumnCountMax; iColumnCount++) {
     		// Draw a Green Line top-left origin
     		//vertical line
     		glBegin(GL_LINES);
     			glColor3f(0.0f, 0.8f, 0.0f); // Green
/*     		
     			//edited by Mike, 20210927
     			glVertex2f(fGridSquareWidth*iColumnCount, 0.0f);    // x,y
     			glVertex2f(fGridSquareWidth*iColumnCount, fGridSquareHeight*iRowCountMax);
*/
					// x,y
     			glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCount), 
     								 myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f));
     			glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCount), 		
     								 myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCountMax));	
     		glEnd();
     }
     
    
    //---------------------
    //part 2: draw collision detection grid
    //---------------------
    //add for collision detection
    //edited by Mike, 20210918
//     for (int iRowCount=iCurrentLevelMapContainerOffsetY; iRowCount<iCurrentLevelMapContainerOffsetMaxViewPortY; iRowCount++) {
     for (int iRowCount=0; iRowCount<iCurrentLevelMapContainerOffsetMaxViewPortY-iCurrentLevelMapContainerOffsetY; iRowCount++) {
     
         //horizontal line
         glBegin(GL_LINES);
         	 glColor3f(0.8f, 0.0f, 0.0f); // Red

/* //edited by Mike, 20210927         
         	 glVertex2f(0.0f, fGridSquareHeight*iRowCount);    // x, y
         	 glVertex2f(fGridSquareWidth*iColumnCountMax, fGridSquareHeight*iRowCount);
*/
					 // x, y
         	 glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f), 
         	 						myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCount));
         	 glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCountMax), 
         	 						myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCount));
         	 
         glEnd();
    }
		
		//edited by Mike, 20210918
//    for (int iColumnCount=iCurrentLevelMapContainerOffsetX; iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX; iColumnCount++) {
    for (int iColumnCount=0; iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX-iCurrentLevelMapContainerOffsetX; iColumnCount++) {

        //vertical line
        glBegin(GL_LINES);
        	glColor3f(0.8f, 0.0f, 0.0f); // Red
        
/* //edited by Mike, 20210927         
        glVertex2f(fGridSquareWidth*iColumnCount, 0.0f);    // x,y
        glVertex2f(fGridSquareWidth*iColumnCount, fGridSquareHeight*iRowCountMax);
*/
					// x,y
					glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCount), 
										 myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f));    
	        glVertex2f(myUsbongUtils->autoConvertFromPixelToVertexPointX(fGridSquareWidth*iColumnCount), 
	        					 myUsbongUtils->autoConvertFromPixelToVertexPointY(fGridSquareHeight*iRowCountMax));
        
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 0.0f); //reset to white
    
//    glPopMatrix();
}

//edited by Mike, 20210716; edited by Mike, 20210828
//void Level3D::drawTileAsQuadWithTexture()
//edited by Mike, 20210720
//void Level3D::drawTileAsQuadWithTexture(std::string sTileId)
	//TO-DO: -update: this
void Level3D::drawTileAsQuadWithTexture(std::string sTileId)
{
	//added by Mike, 20210826
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //removed by Mike, 20210829
	glLoadIdentity();
    
  //added by Mike, 20210927
  glDisable(GL_CULL_FACE);
    
    //added by Mike, 20210830; removed by Mike, 20210830
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    
/*  //edited by Mike, 20210830
    myXPosAsPixel=0;//300; //0;
    myYPosAsPixel=0;//300;//0;
*/

/* //removed by Mike, 20210928
    //note: we increase the size value to make texture larger than the actual pixel size in the image file
    //TO-DO: -reverify: larger image to be blurred
    //edited by Mike, 20210830
    myWidth=fGridSquareWidth; //64; //16;
    myHeight=fGridSquareHeight; //64; //16;
*/    

//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), 0);

		//TO-DO: -reverify: x, y tile positions
    
		//removed by Mike, 20210927
//    openGLDrawTexture(myXPos-myWidth, myYPos-myHeight, myWidth, myHeight, sTileId);    

//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos-myWidth), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos-myHeight), 0);

    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos), 0);
		
		//added by Mike, 20210927
		//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
		//use correct width x height ratio; window 10x18; row x column
//		glScalef(0.20f, 0.35f, 0.0f);
//		glScalef(0.12f, 0.22f, 0.0f);

 //edited by Mike, 20210928
//	  openGLDrawTexture(0.0f, 0.0f, myWidth, myHeight, sTileId);    
/*
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight),
	  								  sTileId);    
*/
/*
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(1.0f),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(1.0f),
	  								  sTileId);    
*/
/*
		//note: 1.0f half of width; max height: 10 rows
		//1.0f half of height; max width: 18 columns
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(1.0f/9.0f),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(1.0f/5.0f),
	  								  sTileId);    
*/
		
		//edited by Mike, 20210929
//		glScalef(1.0f/9.0f, 1.0f/5.0f, 0.0f);
		glScalef(1.0f/8.2f, 1.0f/4.2f, 0.0f);

/*
		//TO-DO: -reverify: cause of myWidth and myHeight inputs, output: NOT whole tile
		//myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(...) instructions?
		//myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(...) instructions?
		
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(0.0001f),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(0.0001f),
	  								  sTileId);    
*/
		//TO-DO: -update: texture computations due to output: white tile
/*
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(1.0f),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(1.0f),
	  								  sTileId);    
*/
/* //edited by Mike, 20210930
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight),
	  								  sTileId);    
*/
	  openGLDrawTexture(0.0f, 
	  									0.0f, 
	  									0.0f, 
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth),
	  								  myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight),
	  								  sTileId);    
	  								  
}

//added by Mike, 20210925
void Level3D::drawTileAsCubeWithTexture(std::string sTileId)
{
	//added by Mike, 20210826
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //removed by Mike, 20210829
	glLoadIdentity();
    
    //added by Mike, 20210830; removed by Mike, 20210830
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    
/*  //edited by Mike, 20210830
    myXPosAsPixel=0;//300; //0;
    myYPosAsPixel=0;//300;//0;
*/
    //note: we increase the size value to make texture larger than the actual pixel size in the image file
    //TO-DO: -reverify: larger image to be blurred
    //edited by Mike, 20210830
    myWidth=fGridSquareWidth; //64; //16;
    myHeight=fGridSquareHeight; //64; //16;

//    glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPosAsPixel), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPosAsPixel), 0);

		//added by Mike, 20210830
    //triangle tile with 90degrees angle
    if (sTileId.compare("0-2") == 0) {//True    
        glBegin(GL_TRIANGLES);
          glVertex3f(0.0f, 0.0f, 0.0f);
          glVertex3f(0.0f-myWidth, 0.0f-myHeight, 0.0f);
          glVertex3f(0.0f, 0.0f-myHeight, 0.0f);
        glEnd();
    }
    else {
      //note: 3rd quadrant; counter clock-wise
      	//edited by Mike, 20210925; TO-DO: -add: in PolygonUtils.cpp
//      glBegin(GL_QUADS); // Each set of 4 vertices form a quad
		//TO-DO: -reverify: this due to wireframe still drawn with cube
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      	glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    	
    	  //TO-DO: -add: remaining cube faces
    	  
    		//TOP FACE
    	  glVertex3f(0.0f, 0.0f, 0.0f);   	
    	  glVertex3f(0.0f-myWidth, 0.0f, 0.0f);    	
    	  glVertex3f(0.0f-myWidth, 0.0f-myHeight, 0.0f);    	
    	  glVertex3f(0.0f, 0.0f-myHeight, 0.0f);
   	  glEnd();
    }
    
    //removed by Mike, 20210930
    //openGLDrawTexture(myXPos-myWidth, myYPos-myHeight, myWidth, myHeight, sTileId);    
}


//added by Mike, 20210708; edited by Mike, 20210910
//TO-DO: -add: function with tile patterns
void Level3D::drawLevelWithTextureUsingInputFileNoScrollYet()
{
    /*
     for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW_RAM; iRowCountToSetDefault++) {
     	for (int iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
     //verified: in macOS, with Japanese keyboard ro-maji input, "¥0", backspace is "¥"
     tempText[iRowCountToSetDefault][iColumnCount]='\0';
     	}
     }
     */
    
/* //removed by Mike, 20210830
    //TO-DO: -update: this
    glPushMatrix();
        drawTileAsQuadWithTexture("0-0");
    glPopMatrix();
    return;
*/
    
    //edited by Mike, 20210724
    iRowCountMax=10;
    iColumnCountMax=18;
    iHeightCountMax=10;
        
    for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
        //iCurrentMaxColumnCountPerRowContainer[iRowCount];
        for (int iColumnCount=0; iColumnCount<iColumnCountMax; iColumnCount++) {
            //                if (cCurrentTextContainer[iRowCount][iColumnCount]) {
  					//edited by Mike, 20210719
//            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("\"A1\"") == 0) { //TRUE
    				//note: "0" for empty, instead of "-1"
    				//with "0", no need to add quotation marks
            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("0") == 0) { //TRUE
            }
            else {
                glPushMatrix();
                	//add +1.0f in x-axis and y-axis due to 3rd quadrant in the draw function
                	//center 0,0,0 origin; vertex positions
  								
  								//edited by Mike, 20210722              	
  								//note: collision detection OK; updated: drawing of tile x and y positions
/*                	glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+fGridSquareWidth*(iColumnCount+1.0f)), myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+fGridSquareHeight*(iRowCount+1.0f)), 0.0f);
*/
           
/* //edited by Mike, 20210830; TO-DO: -reverify: origin now at TOP-LEFT, instead of Center
                glTranslatef(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+(fGridSquareWidth)*(iColumnCount+1.0f)), myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+(fGridSquareHeight)*(iRowCount+1.0f)), 0.0f);
*/
/*
                printf(">>fGridSquareWidth: %f",fGridSquareWidth);
                printf("; fGridSquareHeight: %f",fGridSquareHeight);
*/
//                glTranslatef(0.0f+(fGridSquareWidth)*(iColumnCount+1.0f), 0.0f+(fGridSquareHeight)*(iRowCount+1.0f), 0.0f);
								//edited by Mike, 20210920
                myXPos=0.0f+(fGridSquareWidth)*(iColumnCount);//+1.0f);
//                myXPos=0.0f+(fGridSquareWidth)*(iColumnCount-1.0f);

                myYPos=0.0f+(fGridSquareHeight)*(iRowCount);//+1.0f);
                
/*
printf("autoConvertFromPixelToVertexPointX: %f",(myUsbongUtils->autoConvertFromPixelToVertexPointX(0.0f+(fGridSquareWidth)*(iColumnCount+1.0f)))); 										
printf("autoConvertFromPixelToVertexPointY: %f",myUsbongUtils->autoConvertFromPixelToVertexPointY(0.0f+(fGridSquareHeight)*(iRowCount+1.0f))); 					
*/
 										
 										//edited by Mike, 20210719
//                	drawTileA	glBindTexture(GL_TEXTURE_2D, textureid);
										glEnable(GL_TEXTURE_2D);
//	drawTileAsQuadWithTexture();
//note: incorrect output if we use printf(...) with std::string as input to %s
                
                //added by Mike, 20210725
                //note: use this Command to verify if inputLevel1.csv uses correct quotation mark encoding
                //reminder: we use the same quotation mark for opening and closing
                //in macOS, use XCode to edit input file, e.g. inputLevel1.csv;
                //this is instead of TextEdit
//std::cout << "sCurrentLevelMapContainer[iRowCount][iColumnCount]): " << sCurrentLevelMapContainer[iRowCount][iColumnCount] << "\n";
                
                		drawTileAsQuadWithTexture(sCurrentLevelMapContainer[iRowCount][iColumnCount]);
                glPopMatrix();
            }
        }
    }
    
}

 //edited by Mike, 20210923
//added by Mike, 20210911
void Level3D::setPilotStep(float fPilotStepX, float fPilotStepY, float fPilotStepZ) {
    
//    printf(">>fPilotStepX: %f",fPilotStepX);
    
    stepX=fPilotStepX;
    stepY=fPilotStepY;
    stepZ=fPilotStepZ;
}

/*
void Level3D::setPilot(Pilot* myPilotInput) {
	myPilot = myPilotInput;
}
*/

//added by Mike, 20210708; edited by Mike, 20210712
//TO-DO: -add: function with tile patterns

//added by Mike, 20210916
//TO-DO: -update: instructions to already auto-draw the column after the 1st column or last column of viewport
//This action is to eliminate noticeable drawing of tile during scroll movement, e.g. horizontal scroll action

//edited by Mike, 20210923
//void Level3D::drawLevelMapInViewPort(GLfloat fMyCanvasPosXInput, GLfloat fMyCanvasPosYInput, GLfloat fMyCanvasPosZInput, GLfloat fX, GLfloat fY, GLfloat fZ)
//TO-DO: -reverify: instructions due to fMyCanvasPosX and fMyCanvasPosY do NOT execute movement
void Level3D::drawLevelMapInViewPort(float fX, float fY, float fZ)

{
/* //removed by Mike, 20210923
	 //added by Mike, 20210916
	  fMyCanvasPosX = fMyCanvasPosXInput;
    fMyCanvasPosY = fMyCanvasPosYInput;
    fMyCanvasPosZ = fMyCanvasPosYInput;
*/
    
    float fMovementGridZ=0.0f;
    float fMovementGridX=0.0f;
    float fMovementGridY=0.0f;
 
    fMovementGridX=0;
    fMovementGridY=0; //added by Mike, 20210921
        
/*	//removed by Mike, 20210914
    printf(">>fMyCanvasPosPrevX: %f; fMyCanvasPosX: %f\n",fMyCanvasPosPrevX,fMyCanvasPosX);
    printf(">>fX: %f\n",fX);    
*/
    
    if (fMyCanvasPosX<=0) {
    		fMyCanvasPosX=0;
        fMyCanvasPosPrevX=0;        
    		iCurrentLevelMapContainerOffsetX=0;
				fStepMovemenGridX=0;        
    }
    
    //added by Mike, 20210921; edited by Mike, 20210924
    if (fMyCanvasPosY<=0) {
//    if (fY<=0) {
				fMyCanvasPosY=0;
        fMyCanvasPosPrevY=0;
    		iCurrentLevelMapContainerOffsetY=0;
				fStepMovemenGridY=0;        
    }
		
		//added by Mike, 20210922
		//TO-DO: -fix: fMyCanvasPosX adds negatively when moving left at left-most border
		//TO-DO: -reverify: fMyCanvasPosY 

//    printf(">>getStepX(): %f\n",getStepX());


		//added by Mike, 20210922		
	  printf(">>fMyCanvasPosPrevX: %f; fMyCanvasPosX: %f\n",fMyCanvasPosPrevX,fMyCanvasPosX);
    printf(">>fX: %f\n",fX);    
//    printf(">>fMyWindowWidth/2: %f\n",fMyWindowWidth/2);    		
    printf(">>fMyWindowWidth/2-getWidth(): %f\n",fMyWindowWidth/2-getWidth());
    printf(">>getStepX(): %f\n",getStepX()); //added by Mike, 20210922


/*
	  printf(">>fMyCanvasPosPrevY: %f; fMyCanvasPosY: %f\n",fMyCanvasPosPrevY,fMyCanvasPosY);
    printf(">>fY: %f\n",fY);    
//    printf(">>fMyWindowWidth/2: %f\n",fMyWindowWidth/2);    		
    printf(">>fMyWindowHeight/2-getHeight(): %f\n",fMyWindowHeight/2-getHeight());    		
    printf(">>getStepY(): %f\n",getStepY()); //added by Mike, 20210922
*/
		
		//edited by Mike, 20210924
//		if (fX==(fMyWindowWidth/2-getWidth())) {
		if (fX<=(fMyWindowWidth/2-getWidth())) {
    		if (fMyCanvasPosPrevX!=fMyCanvasPosX) {
/*
	  printf(">>fMyCanvasPosPrevX: %f; fMyCanvasPosX: %f\n",fMyCanvasPosPrevX,fMyCanvasPosX);
    printf(">>fX: %f\n",fX);    
*/

	//edited by Mike, 20210913
   fMovementGridX = (fMyCanvasPosX-fMyCanvasPosPrevX);
 
   //x-axis
   //TO-DO: -update: this to use set value of canvasStepX
   //note: canvasStepX=3.2f (previous); now 3.5...
   if (fMovementGridX < 0) { //moved backward
     fStepMovemenGridX=(fStepMovemenGridX-getStepX());
   }
   else if (fMovementGridX == 0) { //no movement in X-axis
   }
   else {
     fStepMovemenGridX=(fStepMovemenGridX+getStepX());
   }	
   
//    std::cout << "fStepMovemenGridX: " << fStepMovemenGridX << "\n";
//    std::cout << ">>>>fGridSquareWidth: " << fGridSquareWidth << "\n";

    fMovementGridX = 0;
    
    //added by Mike, 20210911; edited by Mike, 20210916
		//TO-DO: -reverify: collision detection and auto-drawn tile object NOT synchronized


		//edited by Mike, 20210914
    if (fStepMovemenGridX>=fGridSquareWidth) {
        fMovementGridX = 1*(fStepMovemenGridX/fGridSquareWidth); //1;
        fStepMovemenGridX=fStepMovemenGridX-fGridSquareWidth;//0;
    }
    else if (fStepMovemenGridX<=-fGridSquareWidth) {    
        fMovementGridX = 1*(fStepMovemenGridX/fGridSquareWidth); //-1
        fStepMovemenGridX=fStepMovemenGridX+fGridSquareWidth;//0;
    }
    

    		std::cout << "fMovementGridX: " << fMovementGridX << "\n";
		
				
    		iCurrentLevelMapContainerOffsetX += (int) fMovementGridX;
                		
    		//added by Mike, 20210916
    		//TO-DO: -reverify: if there exists as input DASH Command
            //edited by Mike, 20210922
		//    if (fX<=0) {
    		if ((fX<=0) || (fX<fGridSquareWidth)){
    			//edited by Mike, 20210922
    			fMyCanvasPosX=0;
//    			fMyCanvasPosX=0+getStepX();
    		    			    			
    			iCurrentLevelMapContainerOffsetX=0;
    			
    			//added by Mike, 20210922
					fX=0;
                
                //edited by Mike, 20210923
                //TO-DO: -reverify: cause of need for Pilot to return to fX <=0
                //to stop excess fMyCanvasPosX + Pilot's stepX for 1 time.
                //afterward, excess stepX canvas movement returns;
                //return to fX <=0 to again stop
					fStepMovemenGridX=0;
//                fStepMovemenGridX=0+getStepX();

            }
		}

   	//added by Mike, 20210922
    else {
//    	   fStepMovemenGridX = fMyWindowWidth/2-myWidth+fX; 

/*	//note: using std::cout, float value trimmed or rounded
    std::cout << ">>>>getFMyCanvasPosX(): " << getFMyCanvasPosX() << "\n";
    std::cout << ">>>>fX: " << fX << "\n";
*/

		printf("getFMyCanvasPosX: %f\n",getFMyCanvasPosX());    
		printf("fX: %f\n",fX);    
/*
		//notes:
>>fMyCanvasPosPrevX: 607.111023; fMyCanvasPosX: 607.111023
>>fX: 607.111084
>>fMyWindowWidth/2: 683.000000
getFMyCanvasPosX: 607.111023 //macOS: 561.777771

--

>>fMyCanvasPosPrevX: 614.699890; fMyCanvasPosX: 614.699890
>>fX: 607.111084
>>fMyWindowWidth/2: 683.000000
getFMyCanvasPosX: 614.699890 //macOS: 554.666687

--

>>getStepX(): 7.588889 //macOS: 7.111112

//excess stepX
*/

        
/*
				if (getFMyCanvasPosX()<=(fMyWindowWidth/2-getWidth())) {
        	if (fX<=(fMyWindowWidth/2-getWidth())) {
						fMyCanvasPosX=fMyCanvasPosX+getStepX();
        	}
				}        	
*/				
/* //removed by Mike, 20210922
        //TO-DO: -reverify: this
				if (getFMyCanvasPosX()==(fMyWfMyWindowWidthindowWidth/2-getWidth())) {
						fMyCanvasPosX=fMyCanvasPosX-getStepX();
				}
*/
    }    
}
//-------------------------------------------------		
/*
	  printf(">>fMyCanvasPosPrevY: %f; fMyCanvasPosY: %f\n",fMyCanvasPosPrevY,fMyCanvasPosY);
    printf(">>fY: %f\n",fY);    
    printf(">>fMyWindowHeight/2: %f\n",fMyWindowHeight/2);    
*/		
		//reminder: TOP-LEFT origin
		//added by Mike, 20210921
		//edited by Mike, 20210921
/*	//edited by Mike, 2fMyCanvasPosX0210922		
		if ((fY==(fMyWindowHeight/2-getHeight())) &&
//		if ((fY==(fMyWindowHeight/2+getHeight())) &&
    		(fMyCanvasPosPrevY!=fMyCanvasPosY)) {
*/
		//edited by Mike, 20210924
//		if (fY==(fMyWindowHeight/2-getHeight())) {		
		if (fY<=(fMyWindowHeight/2-getHeight())) {		
    		if (fMyCanvasPosPrevY!=fMyCanvasPosY) {
/*
	  printf(">>fMyCanvasPosPrevY: %f; fMyCanvasPosY: %f\n",fMyCanvasPosPrevY,fMyCanvasPosY);
    printf(">>fY: %f\n",fY);    
*/

	 //edited by Mike, 20210921; reminder: origin: TOP-LEFT
   fMovementGridY = (fMyCanvasPosY-fMyCanvasPosPrevY);
//   fMovementGridY = (fMyCanvasPosPrevY-fMyCanvasPosY);
 
   //y-axis
   if (fMovementGridY < 0) { //moved upward
     fStepMovemenGridY=(fStepMovemenGridY-getStepY());				
		}
   else if (fMovementGridY == 0) { //no movement in Y-axis
   }
   else {
     fStepMovemenGridY=(fStepMovemenGridY+getStepY());     
   }	
   
//    std::cout << "fStepMovemenGridY: " << fStepMovemenGridY << "\n";
//    std::cout << ">>>>fGridSquareHeight: " << fGridSquareHeight << "\n";

    fMovementGridY = 0;
    
    //TO-DO: -eliminate: excess instructions
    if (fStepMovemenGridY>=fGridSquareHeight) {
        fMovementGridY = 1*(fStepMovemenGridY/fGridSquareHeight); //1;
        fStepMovemenGridY=fStepMovemenGridY-fGridSquareHeight;//0;
//        fStepMovemenGridY=fStepMovemenGridY+fGridSquareHeight;//0;
    }
    else if (fStepMovemenGridY<=-fGridSquareHeight) {    
        fMovementGridY = 1*(fStepMovemenGridY/fGridSquareHeight); //-1
       	fStepMovemenGridY=fStepMovemenGridY+fGridSquareHeight;//0;
//        fStepMovemenGridY=fStepMovemenGridY-fGridSquareHeight;//0;
    }
    

    std::cout << "fMovementGridY: " << fMovementGridY << "\n";

		
    iCurrentLevelMapContainerOffsetY += (int) fMovementGridY;
                   
    //added by Mike, 20210922
    //TO-DO: -reverify: excess fStepMovemenGridY to cause incorrect grids for auto-drawing and collision detection 
    //TO-DO: -reverify: excess fStepMovemenGridX to cause incorrect grids for auto-drawing and collision detection 
                   
    //added by Mike, 20210916
    //TO-DO: -reverify: if there exists as input DASH Command
    //edited by Mike, 20210922
//    if (fY<=0) {
    	if ((fY<=0) || (fY<fGridSquareHeight)){
    		fMyCanvasPosY=0;
    		iCurrentLevelMapContainerOffsetY=0;
    		
    		//added by Mike, 20210922
				fY=0;
				fStepMovemenGridY=0;
    	}    
    }
    //added by Mike, 20210922
    else {
    	
    }    
}
//-------------------------------------------------		
    
	
/*
    //added by Mike, 20210309
   //TO-DO: -reverify: iMovementGridZ, etc value
   std::cout << "iMovementGridZ: " << iMovementGridZ << "\n";
   std::cout << "iMovementGridX: " << iMovementGridX << "\n";
*/
  
/*   	//removed by Mike, 20210921    		
   	iCurrentLevelMapContainerOffsetZ += fMovementGridZ;
    iCurrentLevelMapContainerOffsetY += (int)fMovementGridY;
*/

    //added by Mike, 20210911
    if (iCurrentLevelMapContainerOffsetX<0) {
        iCurrentLevelMapContainerOffsetX=0;
        fMyCanvasPosPrevX=0;
        fMyCanvasPosX=0;
//        fMovementGridX=0; //removed by Mike, 20210915
    }

		//added by Mike, 20210921
    if (iCurrentLevelMapContainerOffsetY<0) {
        iCurrentLevelMapContainerOffsetY=0;
        fMyCanvasPosPrevY=0;
        fMyCanvasPosY=0;
//        fMovementGridY=0; //removed by Mike, 20210915
    }
    
   	int iRowCount=iCurrentLevelMapContainerOffsetY;
   	iCurrentLevelMapContainerOffsetMaxViewPortY=iRowCount+MAX_Y_AXIS_VIEWPORT;
		   
		//added by Mike, 20210921
   	 if ((iRowCount<0) or (iCurrentLevelMapContainerOffsetY<0)) {
	 	 		iRowCount=0;
        //added by Mike, 20210912
        iCurrentLevelMapContainerOffsetMaxViewPortY=iRowCountMax;
		 }
   	 else if (iCurrentLevelMapContainerOffsetY>=MAX_INPUT_TEXT_PER_LINE) {
        //edited by Mike, 20210912
	 			//iColumnCount=MAX_INPUT_TEXT_PER_LINE-1;
        iRowCount=MAX_INPUT_TEXT_PER_LINE-1-iRowCountMax;
         
	 			iCurrentLevelMapContainerOffsetY=MAX_INPUT_TEXT_PER_LINE-1;
   	 }		
		 
		   
 //--
    //added by Mike, 20210912
    //TO-DO: -update: instructions to set Pilot to be at center horizontal and vertical;
    //if at column 0 or at column end, Pilot can move awayGLfloat fX, GLfloat fY, GLfloat fZ,  from center
    //TO-DO: -reverify: scroll movement backward to be OK
    
    //TO-DO: -update: scrolling instructions to be NOT by fGridSquareWidth;
    //--> add: fractions of fGridSquareWidth
    
  	//edited by Mike, 20210910      iCurrentLevelMapContainerOffsetMaxViewPortY
//    for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
		for (;iRowCount<iCurrentLevelMapContainerOffsetMaxViewPortY; iRowCount++) {
		
	 //added by Mike, 20210910; edited by Mike, 20210920
     int iColumnCount=iCurrentLevelMapContainerOffsetX;
     iCurrentLevelMapContainerOffsetMaxViewPortX=iColumnCount+MAX_X_AXIS_VIEWPORT;
		        
//        int iCurrentLevelMapContainerOffsetMaxViewPortX=iColumnCount+MAX_X_AXIS_VIEWPORT-1;

   	 if ((iColumnCount<0) or (iCurrentLevelMapContainerOffsetX<0)) {
	 	 		iColumnCount=0;
        //added by Mike, 20210912
        iCurrentLevelMapContainerOffsetMaxViewPortX=iColumnCountMax;
		 }
   	 else if (iCurrentLevelMapContainerOffsetX>=MAX_INPUT_TEXT_PER_LINE) {
        //edited by Mike, 20210912
	 			//iColumnCount=MAX_INPUT_TEXT_PER_LINE-1;
        iColumnCount=MAX_INPUT_TEXT_PER_LINE-1-iColumnCountMax;
         
	 			iCurrentLevelMapContainerOffsetX=MAX_INPUT_TEXT_PER_LINE-1;
   	 }
       
  			//edited by Mike, 20210920
				for (;iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX; iColumnCount++) {
//				for (;iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX+1; iColumnCount++) {

            //                if (cCurrentTextContainer[iRowCount][iColumnCount]) {
  					//edited by Mike, 20210719
//            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("\"A1\"") == 0) { //TRUE
    				//note: "0" for empty, instead of "-1"
    				//with "0", no need to add quotation marks
            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("0") == 0) { //TRUE
            }
            else {
                glPushMatrix();
                	//add +1.0f in x-axis and y-axis due to 3rd quadrant in the draw function
                	//center 0,0,0 origin; vertex positions
  								
								//TO-DO: -reverify: cause of skipping to previous column when executing backward movement
								//edited by Mike, 20210916
//								if (fMovementGridX==fGridSquareWidth) {
								if ((fMovementGridX==fGridSquareWidth) || (fMovementGridX==(fGridSquareWidth*-1))) {								                									myXPos=0.0f+(fGridSquareWidth)*(iColumnCount-iCurrentLevelMapContainerOffsetX);
								}
								else {
                  //edited by Mike, 20210916
                  myXPos=0.0f+(fGridSquareWidth)*(iColumnCount-iCurrentLevelMapContainerOffsetX)-fStepMovemenGridX;
								}

								//edited by Mike, 20210921
                if ((fMovementGridY==fGridSquareHeight) || (fMovementGridY==(fGridSquareHeight*-1))) {                    
                    myYPos=0.0f+(fGridSquareHeight)*(iRowCount-iCurrentLevelMapContainerOffsetY);
								}
								else {
									//edited by Mike, 20210916
                	myYPos=0.0f+(fGridSquareHeight)*(iRowCount-iCurrentLevelMapContainerOffsetY)-fStepMovemenGridY;
								}
								
								//printf(">>>>myXPos: %f; myYPos: %f\n",myXPos,myYPos);

							  printf(">>myXPos: %f; vertexPosX: %f\n",myXPos,myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos));
							  printf(">>myYPos: %f; vertexPosY: %f\n",myYPos,myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos));
	  

 										//edited by Mike, 20210719
//                	drawTileA	glBindTexture(GL_TEXTURE_2D, textureid);
										glEnable(GL_TEXTURE_2D);

                		drawTileAsQuadWithTexture(sCurrentLevelMapContainer[iRowCount][iColumnCount]);
                glPopMatrix();
            }
        }
    }
    

	//added by Mike, 20210306; edited by Mike, 20210308	
	//note: does NOT yet include the stepX when there is column movement
	fMyCanvasPosPrevX=fMyCanvasPosX;
	fMyCanvasPosPrevY=fMyCanvasPosY;
	fMyCanvasPosPrevZ=fMyCanvasPosZ;
    
}

//added by Mike, 20210423
void Level3D::update(float dt)
{
/*	//removed by Mike, 20210804
    switch (currentState)
    {
        case INITIALIZING_STATE:
        case MOVING_STATE:
            switch(currentMovingState) {
                case WALKING_MOVING_STATE:
                    break;
                case ATTACKING_MOVING_STATE:                  
                    break;
                    
                default: //STANDING STATE
                    break;//do nothing
            }
            break;
        case IN_TITLE_STATE:
            break;
        default: //STANDING STATE
            break;//do nothing
    }
*/    

}

void Level3D::changeState(int s)
{
    currentState=s;
}

//added by Mike, 20201226
void Level3D::keyDown(int keyCode) {
    myKeysDown[keyCode] = TRUE;
}

void Level3D::keyUp(int keyCode) {
    myKeysDown[keyCode] = FALSE;
}

//edited by Mike, 20210923
//void Level3D::move(int key)
void Level3D::move(int key, MyDynamicObject* mdoPilot)
{
    switch(key) {
        case KEY_A:
        		//edited by Mike, 20210923        		
            fMyCanvasPosX = fMyCanvasPosX-stepX;
/*
    				if (fMyCanvasPosX==fMyWindowWidth-getWidth()-stepX) {
            printf(">>>>DITO: %f",fMyCanvasPosX);
						}
						else {
            	fMyCanvasPosX = fMyCanvasPosX-stepX;
						}						
*/						
            break;
        case KEY_D:
        		//edited by Mike, 20211001
        		//TO-DO: -fix: noticeable auto-drawing of tiles @left-most column
        		//--> shift column x1 leftward; 
        		//--> fix: Pilot step movement noticeably slows at canvasPosX = 0
            fMyCanvasPosX = fMyCanvasPosX+stepX;
            
/*	//TO-DO: -reverify: this;   					
  					//edited by Mike, 20211001
  					//note: Level3D 
            if (mdoPilot->getX()<=fMyWindowWidth/2-getWidth()-getStepX()) {
//            if (mdoPilot->getX()<fMyWindowWidth/2) {
						}
						else {
            	fMyCanvasPosX = fMyCanvasPosX+stepX;
						}
*/						
            break;
       case KEY_W:
       			//edited by Mike, 20210924
            fMyCanvasPosY = fMyCanvasPosY-stepY;
                       
/*						
						//edited by Mike, 20210924
            if (mdoPilot->getY()<=fMyWindowHeight/2-getHeight()-getStepY()) {
//            if (fMyCanvasPosY<=fMyWindowHeight/2-getHeight()-getStepY()) {

            	fMyCanvasPosY = fMyCanvasPosY-stepY;
						}
						else {
						}
*/						
            break;
        case KEY_S:
        		//edited by Mike, 20211001
        		//TO-DO: -fix: noticeable auto-drawing of tiles @top-most row
        		//--> shift row x1 upward; 
        		//--> fix: Pilot step movement noticeably slows at canvasPosY = 0
          	fMyCanvasPosY = fMyCanvasPosY+stepY;
            
/* //removed by Mike, 20211001; TO-DO: -reverify: this            
            //edited by Mike, 20210924
//            if (mdoPilot->getY()+getStepY()<fMyWindowHeight/2-getHeight()-getStepY()) {
            if (mdoPilot->getY()<=fMyWindowHeight/2-getHeight()-getStepY()) {
						}
						else {
            	fMyCanvasPosY = fMyCanvasPosY+stepY;
						}
*/						
            break;            
    }        
}

//added by Mike, 20210724; edited by Mike, 20210725
bool Level3D::isLevel2DCollideWith(MyDynamicObject* mdo)
{
    
    if ((!checkIsCollidable())||(!mdo->checkIsCollidable()))    
    {
    		printf(">>>>>NOT COLLIDABLE");
        return false;
    }
 
        //edited by Mike, 20210911
/*		for (int iRowCount=0; iRowCount<iRowCountMax; iRowCount++) {
                for (int iColumnCount=0; iColumnCount<iColumnCountMax; iColumnCount++) {
*/
    
/*	//removed by Mike, 20210927
    //TO-DO: -fix: problem with forward movement, then backward movement;
    //if forward +3 tiles, then backward movement -1 tile, based on auto-drawn tiles
 //removed by Mike, 20210915    
printf(">>>> iCurrentLevelMapContainerOffsetY: %i;",iCurrentLevelMapContainerOffsetY);
//printf(">>>> iCurrentLevelMapContainerOffsetX: %i;\n",iCurrentLevelMapContainerOffsetX);
  
printf(">>>> iCurrentLevelMapContainerOffsetMaxViewPortY: %i;",iCurrentLevelMapContainerOffsetMaxViewPortY);
//printf(">>>> iCurrentLevelMapContainerOffsetMaxViewPortX: %i;\n",iCurrentLevelMapContainerOffsetMaxViewPortX);
*/

    int iStartRowCount=0; //7;
    int iStartColumnCount=0; //1//6; //7;

/*  //removed by Mike, 20210917
  	//added by Mike, 20210917
  	iCurrentLevelMapContainerOffsetMaxViewPortY=10;
  	iCurrentLevelMapContainerOffsetMaxViewPortX=18;
*/
  				//edited by Mike, 20210913  
//        for (int iRowCount=iCurrentLevelMapContainerOffsetY; iRowCount<(iCurrentLevelMapContainerOffsetY+iRowCountMax); iRowCount++) {
					
					//edited by Mike, 20210918
        for (int iRowCount=iStartRowCount+iCurrentLevelMapContainerOffsetY; iRowCount<iCurrentLevelMapContainerOffsetMaxViewPortY; iRowCount++) {
//        for (int iRowCount=0; iRowCount<iCurrentLevelMapContainerOffsetMaxViewPortY-iCurrentLevelMapContainerOffsetY; iRowCount++) {
        
        
        			//edited by Mike, 20210913
//            for (int iColumnCount=iCurrentLevelMapContainerOffsetX; iColumnCount<(iCurrentLevelMapContainerOffsetX+iColumnCountMax); iColumnCount++) {
//           for (int iColumnCount=iStartColumnCount+iCurrentLevelMapContainerOffsetX; iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX; iColumnCount++) {

					//edited by Mike, 20210918
        for (int iColumnCount=iStartColumnCount+iCurrentLevelMapContainerOffsetX; iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX; iColumnCount++) {
//        for (int iColumnCount=0; iColumnCount<iCurrentLevelMapContainerOffsetMaxViewPortX-iCurrentLevelMapContainerOffsetX; iColumnCount++) {
                 
//printf(">>>> iRowCount: %i; iColumnCount: %i;",iRowCount,iColumnCount);
    				//note: "0" for empty, instead of "-1"
    				//with "0", no need to add quotation marks
            if (sCurrentLevelMapContainer[iRowCount][iColumnCount].compare("0") == 0) { //TRUE
            }
            else {
/*            		//edited by Mike, 20210921
                if (mdo->collideWithLevel2DTileRect(0.0f+fGridSquareWidth*(iColumnCount-iCurrentLevelMapContainerOffsetX-1)-fStepMovemenGridX,0.0f+fGridSquareHeight*(iRowCount-iCurrentLevelMapContainerOffsetY)-fStepMovemenGridY, fGridSquareWidth, fGridSquareHeight)) {
*/
                if (mdo->collideWithLevel2DTileRect(0.0f+fGridSquareWidth*(iColumnCount-iCurrentLevelMapContainerOffsetX-1)-fStepMovemenGridX,0.0f+fGridSquareHeight*(iRowCount-iCurrentLevelMapContainerOffsetY-1)-fStepMovemenGridY, fGridSquareWidth, fGridSquareHeight)) {
                    
                printf(">>>>> fGridSquareWidth: %f",fGridSquareWidth);
  	
/*  									//edited by Mike, 20210921          
                    return this->hitByAtTile(mdo, sCurrentLevelMapContainer[iRowCount][iColumnCount],
                                             0.0f+fGridSquareWidth*(iColumnCount-iCurrentLevelMapContainerOffsetX-1)-fStepMovemenGridX,
                                             0.0f+fGridSquareHeight*(iRowCount-iCurrentLevelMapContainerOffsetY)-fStepMovemenGridY);
*/
                    return this->hitByAtTile(mdo, sCurrentLevelMapContainer[iRowCount][iColumnCount],
                                             0.0f+fGridSquareWidth*(iColumnCount-iCurrentLevelMapContainerOffsetX-1)-fStepMovemenGridX,
                                             0.0f+fGridSquareHeight*(iRowCount-iCurrentLevelMapContainerOffsetY-1)-fStepMovemenGridY);

  									}  									
		        }		        
		   }
		}
    
    return false;
}

//added by Mike, 20210725; edited by Mike, 20210728
//void Level3D::hitByAtTile(MyDynamicObject* mdo, std::string sTileId)
//edited by Mike, 20210803
//void Level3D::hitByAtTile(MyDynamicObject* mdo, std::string sTileId, int iTileXPos, int iTileYPos)
bool Level3D::hitByAtTile(MyDynamicObject* mdo, std::string sTileId, int iTileXPos, int iTileYPos)
{    
    //added by Mike, 20210725
//    int iTileColumn = myUsbongUtils->autoIdentifyColumnInputInLevelMapContainer(sTileId); //column
//    int iTileRow = myUsbongUtils->autoIdentifyRowInputInLevelMapContainer(sTileId); //row

//    std::cout << "sTileId: " << sTileId << "\n";
/*
    std::cout << "iTileXPos: " << iTileXPos << "\n";
    std::cout << "iTileYPos: " << iTileYPos << "\n";
*/

		//edited by Mike, 20210729
    //note: there exist quotation marks in sTileId
    //"0-0": wall
		
		//added by Mike, 20210729
    //note: select saved tile .csv file do not include quotation marks    
		sTileId = myUsbongUtils->autoDeleteQuotationMark(sTileId);

//    std::cout << "autoDeleted sTileId: " << sTileId << "\n";

    float fStepDashMultiplier=1.0f; //0.0f;
    
    //edited by Mike, 20210803
		//TO-DO: -set: all tiles in row 0, classifed as wall collision?
    if (sTileId.compare("0-0") == 0) {//True 
  	}
/* //removed by Mike, 20210901  	
    //ground/floor tile
	else if ((sTileId.compare("1-0") == 0) ||
    	 (sTileId.compare("2-0") == 0)) {				
				//edited by Mike, 20210830
				//reminder: added: gravity to exist in world
				//TO-DO: -add: container to store gravity value
//        mdo->setYPosAsPixel(mdo->getYAsPixel()-mdo->getStepY());
				//edited by Mike, 20210831
        mdo->setYPos(mdo->getY()-mdo->getStepY());
//        mdo->setYPos(mdo->getY()-mdo->getStepY()*2);
				
				//edited by Mike, 20210901
//        return true;//false;
        return false;      
    }
*/    
    //TO-DO: -update: this; use Trigonometry; triangle with 90degrees angle
    else if (sTileId.compare("0-2") == 0) {//True
        
        int iTileAngle=45;

 				//Recommended Reading:
 				//1) https://www.mathsisfun.com/sine-cosine-tangent.html;
 				//last accessed: 20210803
 				
 				//added by Mike, 20210806
 				//reminder: SOH-CAH-TOA;
 				//example: sin(iTileAngle)=O/H;
 				//where: O = Opposite; H = Hypothenus;
 				//set O = stepY
 				//example: cos(iTileAngle)=A/H;
 				//where: A = Adjacent; H = Hypothenus
 				//set A = stepX
 					 					
 					//TO-DO: -reverify: this due to incorrect output when climbing stairs without DASH command
 					if (mdo->getIsExecutingDash()) { 					
						fStepDashMultiplier*=2.0f;
 					} 					
//						fStepDashMultiplier*=2.0f;
        
        //added by Mike, 20210806
        if (mdo->getCurrentFacing()==FACING_UP) {
        }
 								
				if (mdo->getCurrentFacing()==FACING_LEFT) {
					//GO LEFT
				   if (mdo->getIsCurrentMovingStateIdleState()) {
                       //going DOWN
                       //TO-DO: -add: sliding sprite image
                       //TO-DO: -add: if sliding, update push up in ground/floor tile;
                       //due to Pilot is pushed down by gravity to create a check mark
				   }
				   else {
           }
 //edited by Mike, 20210831           
          	mdo->setYPos(mdo->getY()+mdo->getStepY()*(1+(mdo->getStepX()*0.02f*fStepDashMultiplier)/cos(iTileAngle))+1);
          	mdo->setXPos(mdo->getX()-mdo->getStepY()*(1+(mdo->getStepY()*0.02f*fStepDashMultiplier)/sin(iTileAngle))-1);
/*          	
          	mdo->setYPos(mdo->getY()+mdo->getStepY()*(mdo->getStepX()*0.02f*fStepDashMultiplier)/cos(iTileAngle)+1);
          	mdo->setXPos(mdo->getX()-mdo->getStepY()*(mdo->getStepY()*0.02f*fStepDashMultiplier)/sin(iTileAngle)-1);
*/

          	//push up
          	//edited by Mike, 20210901
          	//slides on slope
          	mdo->setYPos(mdo->getY()-mdo->getStepY()*fStepDashMultiplier);
          	//bounces on slope
//          	mdo->setYPos(mdo->getY()-mdo->getStepY()*2*fStepDashMultiplier);

						//edited by Mike, 20210901
        		return false;	
        }
        else if (mdo->getCurrentFacing()==FACING_RIGHT) {
					//GO RIGHT
/*        //edited by Mike, 20210806
        	mdo->setYPosAsPixel(mdo->getYAsPixel()-cos(iTileAngle)*mdo->getStepY());
        	mdo->setXPosAsPixel(mdo->getXAsPixel()+sin(iTileAngle)*mdo->getStepX());
        	
        	//added by Mike, 20210803
					//TO-DO: -reverify: this
        	//note: push down; gravity?; sand, e.g. sink hole?
//        	mdo->setYPosAsPixel(mdo->getYAsPixel()+1); //mdo->getStepY());
*/
				   if (mdo->getIsCurrentMovingStateIdleState()) {
                   }
				   else {
/* //edited by Mike, 20210831           
                       mdo->setYPos(mdo->getY()-(mdo->getStepX()*0.02f*fStepDashMultiplier)/cos(iTileAngle)+1);                       
                       mdo->setXPos(mdo->getX()+(mdo->getStepY()*0.02f*fStepDashMultiplier)/sin(iTileAngle)+1);
*/

                       mdo->setYPos(mdo->getY()-(mdo->getStepX()*0.02f*fStepDashMultiplier)/cos(iTileAngle));                       
                       mdo->setXPos(mdo->getX()+(mdo->getStepY()*0.02f*fStepDashMultiplier)/sin(iTileAngle));
/*
                       mdo->setYPos((mdo->getY()-1)-(mdo->getStepX()*0.02f*fStepDashMultiplier)/cos(iTileAngle));                       
                       mdo->setXPos(mdo->getX()+((mdo->getStepY()-1)*0.02f*fStepDashMultiplier)/sin(iTileAngle));
*/                     
											 //note: causes angle value to be smaller than 90degrees
											 //TO-DO: -update: instructions to be exact
//											 fStepDashMultiplier=fStepDashMultiplier*0.9f;
                   }
        }       
  			
        //add this to stop gravity via push upward
        //edited by Mike, 20210806
//        mdo->setYPosAsPixel(mdo->getYAsPixel()-mdo->getStepY()*fStepDashMultiplier);
        //add this to remove excess space between feet and tile
        //TO-DO: -reverify: cause of incorrect output with push upward if we use <0.9
        //getStepY()*0.9
        //reminder: setYPosAsPixel(...) input, integer
        //edited by Mike, 20210901
        mdo->setYPos(mdo->getY()-mdo->getStepY()*fStepDashMultiplier);
//       	mdo->setYPos(mdo->getY()-mdo->getStepY()*2*fStepDashMultiplier);
        
				//edited by Mike, 20210901
        return false;
//        return true;
    }
    //ground/floor tile
		else if ((sTileId.compare("1-0") == 0) ||
    	 (sTileId.compare("2-0") == 0)) {				
				//edited by Mike, 20210830
				//reminder: added: gravity to exist in world
				//TO-DO: -add: container to store gravity value
            //edited by Mike, 20210917
        mdo->setYPos(mdo->getY()-mdo->getStepY());
//            mdo->setYPos(mdo->getY()-mdo->getStepY()-1);
            
				//added by Mike, 20210901
				//note: bounce spring action from ground; sonic 1 via game gear machine?
            //edited by Mike, 20210916
        if (mdo->collideWithLevel2DTileRect(iTileXPos,iTileYPos, fGridSquareWidth, fGridSquareHeight)) {
//            if (mdo->collideWithLevel2DTileRect(iTileXPos+10,iTileYPos+10, fGridSquareWidth-10, fGridSquareHeight-10)) {
        //            mdo->setYPos(mdo->getY()-mdo->getStepY()-1);
            mdo->setYPos(mdo->getY()-mdo->getStepY());

        }
				
        return false;      
    }

    
    return false;
    
        //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */

    //removed by Mike, 20210725
    //reset();
}

void Level3D::hitBy(MyDynamicObject* mdo)
{
    //changeState(DEATH_STATE);
    //setCollidable(false);
//    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */
    //removed by Mike, 20210725
    //reset();
}

/*	//removed by Mike, 20210522
 void Level3D::setOpenGLCanvas(OpenGLCanvas* c)
 {
 myOpenGLCanvas = c;
 }
 */

void Level3D::reset()
{
/* //removed by Mike, 20210801
    changeState(INITIALIZING_STATE);
    myXPos=0;
    myYPos=0;
    rotationAngle=0.0f;
    thrust=0.0f;
    setCollidable(false);
    invincibleCounter=0;
*/
}
int Level3D::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Level3D::destroy()
{
    /*
     for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
     }
     delete [] explosionParticle;
     */
}

//added by Mike, 20210712
void Level3D::read(char *inputFilename) {
    int c;
    FILE *file;
    
    
    //added by Mike, 20210712
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Level3D read(...)\n");
    
    
    //TO-DO: update: this
    //	char** iCurrentLevelMapContainer = new char[100][100];
    int iRowCount=0;
    int iColumnCount=0;
    
    
    /*	//edited by Mike, 20210305
     for (iRowCount=0; iRowCount<MAX_Z_AXIS_VIEWPORT; iRowCount++) {
     for (iColumnCount=0; iColumnCount<MAX_X_AXIS_VIEWPORT; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    //edited by Mike, 20210310
    /*
     for (iRowCount=0; iRowCount<100; iRowCount++) {
     for (iColumnCount=0; iColumnCount<100; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    /*	//edited by Mike, 20210321
     //add +60 to be 160; where 60 : viewport max
     for (iRowCount=0; iRowCount<160; iRowCount++) {
     for (iColumnCount=0; iColumnCount<160; iColumnCount++) {
     sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
     }
     }
     */
    
    for (iRowCount=0; iRowCount<MAX_Y_AXIS_MAP; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_X_AXIS_MAP; iColumnCount++) {
            //edited by Mike, 20210724
//            sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
            sCurrentLevelMapContainer[iRowCount][iColumnCount]=(char*)"0";//-1";//'G';
        }
    }
    
    iRowCount=0;
    iColumnCount=0;
				
    //noted by Mike, 20201210
    //note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
    //I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
    //where memory leaks = not deallocated storage in memory, albeit not used by software application
    //identifying not deallocated storage in memory becomes more difficult with increasing use
    char input[MAX_INPUT_TEXT_PER_LINE]; //max size
    char inputTextLine[MAX_INPUT_TEXT_PER_LINE]; //max size
    char tempInputTextLine[MAX_INPUT_TEXT_PER_LINE]; //max size
    
    strcpy(input, "input/");
    strcat(input, inputFilename); //already includes .txt
    //	strcat(input,".txt");
    
    //	printf("dito: %s",input);
    
    //	file = fopen("input/"+inputFilename, "r"); //.txt file
    //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
    file = fopen(input, "r"); //.txt file
    
    //TO-DO: -reverify: tile positions
    int iCount=0;
    
    if (file) {
        //edited by Mike, 20210210
        //		while ((c = getc(file)) != EOF) {
        while (fgets (input, MAX_INPUT_TEXT_PER_LINE, file)) { /* read each line of input */
            //	putchar(c);
            
            /*	//removed by Mike, 20210210
             char sRow[2] = {(char)c};
             */
            //delimited using new line
            /*			char *chRow = strtok(sRow, "\n");
             */
            sscanf (input, "%s", inputTextLine);
            
            
            //			iCount=0;
            //input text per line
            //			printf("%i;\n",iCount);
            //			printf("%i;",iCount);
            
            iCount=iCount+1;
            
            //added by Mike, 20210208
            iColumnCount=0;
            
            //removed by Mike, 20210210
            /*
             //			char s[2] = {0};
             //			*s = c;
             //edited by Mike, 20210209
             //				char s[2] = {c};
             //				char s[2] = {itoa(c)};
             char s[2] = {(char)c};
             
             //delimited using comma
             char *ch = strtok(s, ",");
             */
            strcpy(tempInputTextLine,inputTextLine);
            
            //note: add "-1" for empty
            //otherwise, comma as column is skipped
            char *ch = strtok(tempInputTextLine, ",");
            
            while (ch != NULL) {
                //				printf("%i,",iColumnCount);
                
                //TO-DO: use String, instead of char
                //TO-DO: -reverify: output due to "G" not put in container
                //				sCurrentLevelMapContainer[iRowCount][iColumnCount]=&ch;
                sCurrentLevelMapContainer[iRowCount][iColumnCount]=ch;
                
                /*	//edited by Mike, 20210211
                 printf("%s:",ch);
                 printf("%i,",iColumnCount);
                 */
                printf("HALLO %i:",iColumnCount);
                printf("%s,",ch);
                
                iColumnCount=iColumnCount+1;
                ch = strtok(NULL, ",");
            }
            
            //edited by Mike, 20210311
            //			if (iRowCount<100) {
            //edited by Mike, 20210321
            //			if (iRowCount<160) {
            if (iRowCount<MAX_X_AXIS_MAP) {
                iRowCount=iRowCount+1;
            }
            else {
                iRowCount=0;
            }
            
            printf("\n");
        }
        fclose(file);
    }
}

//--------------------------------------------
