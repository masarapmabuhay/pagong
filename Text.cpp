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
 * @date updated: 20211005
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
 
//added by Mike, 20210907
//ji ga sukoshi yuganderu...
//--> ang sulat, kaunting bumabaliktot...
//hosoku natta tokoro ari, ookiku natta tokoro mo ari
//--> may pumayat na bahagi, may lumaking bahagi
//DQ ke-tai ban no you na kabe mitai...?
//--> mukhang katulad sa ding-ding ng lathala ng DQ sa dala-dalang telepono
//moshiku ha DQVII no PS1 ban?
//--> O kaya, ang lathala ng DQVII sa PS1?
 
//added by Mike, 20210903
//note: yaritori, i.e. gawa + kuha; palitan, i.e. exchange, ng balita
//kompyu-ta to no yaritori; 
//--> palitan ng balita kasama ang kompyuter;
//compile : CHESS? ragu ga ari, 
//--> compile : CHESS? May lag,
//Cowboy Bebop no Ed ga tooi GYARAKUSHI- ni iru mono to
//--> Si Ed ng Cowboy Bebop, kasama ang Tao/Bagay na nasa malayong GALAXY
//asonderu tte kanji?
//--> naglalaro ba ang pakiramdam?
//compile action : conversation?
//--> compile action : conversation?
//kaita shiji ga kompyu-ta ni taishite, akiraka de aru ka dou ka to iu shitsumon...
//--> ang isinulat na turo+ipakita, i.e. instruction, sa kompyuter, malinaw ba o hindi na tanong...
//kotae wo kaeshite kureru; aimai na shiji nado wo shirasete kureru.
//--> ibinabalik sa akin ang sagot; ipinapaalam sa akin ang malabong mga turo+ipakita, atbp.
//mou akirakade aru to iu joutai to nattara, shizukani, [kocchi no tsugi no shiji wo douzo], 
//--> Kapag naging nasa pook na malinaw na, tahimik na, "ang inyo pong sunod na turo+ipakita",
//mitai na hyouji de matte ite kureru.
//--> tulad sa ganoong pagpapakita, hinihintay ako.
 
//TO-DO: -update: this

//added by Mike, 20210201
//TO-DO: -delete: excess instructions


#include <stdio.h>
#include <math.h>

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

//added by Mike, 20210613
#include "Text.h"

//added by Mike, 20210614
#include "Font.h"

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
GLboolean Text::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

//added by Mike, 20210902
void Text::setup()
{
	openGLITexture = openGLLoadTexture((char*)"textures/textExampleV3.png", myWidth, myHeight);	
}

//edited by Mike, 20201001
//Text::RobotShip(): MyDynamicObject(0,0,300)
//edited by Mike, 20201115
//Text::RobotShip(): MyDynamicObject(0,0,0)
//edited by Mike, 20210625
//Text::Text(float xPos, float yPos, float zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,0.0f, windowWidth, windowHeight)
Text::Text(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{
    //edited by Mike, 20201001
    //currentState=IN_TITLE_STATE;//MOVING_STATE;
    currentState=MOVING_STATE;
    //added by Mike, 20201201
    currentMovingState=IDLE_MOVING_STATE;
    
    fCountTotalFrames=4.0f; //added by Mike, 20210903
    	
    //    myXPos=0.0;
    //    myYPos=0.0;
    //myYPos=300.0;
    //    myZPos=300.0;
    /*
     stepX=0.01;
     stepY=0.01;
     stepZ=0.01;
     */
    /*	//edited by Mike, 20201023
     //added by Mike, 20201001
     stepX=0.03;
     stepY=0.03;
     */
    /*	//edited by Mike, 20201116
     stepX=0.1;
     stepY=0.1;
     */
    
    //edited by Mike, 20201025
    //edited again by Mike, 20210114
    //reverified double step due to double the size of Window
    //4096x4096; update in main.cpp
    //OpenGLCanvas update sleep delay, instead of step
    /*	//edited by Mike, 20210116
     stepX=0.3;
     stepY=0.3;
     stepZ=0.3;
     */
    //edited by Mike, 20210505
    //note: if set to 0.3; noticeable zoom-in, zoom-out cycling movement due to quick speed
    //observed: Samurai Spirits IV's created world executes such camera eye movement
    //that becomes noticeable with background zoom-in, zoom-out via cycling movement
    //    stepX=0.3;
    stepX=0.2;
    
    stepY=0.3;
    stepZ=0.3;
    
    invincibleCounter=0;
    currentDeathFrame=0;
    
    //added by Mike, 20210517
    //TO-DO: -add: auto-identify object width and height
    myWidthAsPixel=128;
    myHeightAsPixel=64;
    
/*  //added by Mike, 20210903; removed by Mike, 20210903
    myWidth=128;
    myHeight=64;
*/    
    
    //TO-DO: -update: this
    //note: float xPos as parameter to int myXPosAsPixel not correct output
    //edited by Mike, 20210613
    myXPosAsPixel=(int)xPos;//320;//(int)xPos;
    myYPosAsPixel=(int)yPos;
    myZPosAsPixel=(int)zPos;
    
    //    printf(">>myXPosAsPixel: %i\n",myXPosAsPixel);
 
/* //removed by Mike, 20210903    
    myXPos=xPos;
    myYPos=yPos;
    myZPos=zPos;
*/    
    
    fMyWindowWidth=fWindowWidth;
    fMyWindowHeight=fWindowHeight;
    
    //added by Mike, 20210626
    fMyWindowWidthAsPixelRatioToHeightPixel=1.0f;
    iMyWindowWidthAsPixelOffset=0;
  
/* //removed by Mike, 20210903    
    if (fMyWindowWidth!=fMyWindowHeight) {
        //added by Mike, 20210626
        //note: causes zero value with myWindowHeight/myWindowWidth;
        //add *1.0f, to be myWindowHeight/(myWindowWidth*1.0f);
        //NOT myWindowHeight/myWindowWidth*1.0f;
        //edited by Mike, 20210628
        //        fMyWindowWidthAsPixelRatioToHeightPixel= 0.56222;
        fMyWindowWidthAsPixelRatioToHeightPixel= fMyWindowHeight/(fMyWindowWidth*1.0f);
        
        //note: width value > height value
        //TO-DO: -add: auto-update
        //height: 768; width: 1366
        iMyWindowWidthAsPixelOffset=(fMyWindowWidth-fMyWindowHeight)/2;
        //			iMyWindowWidthAsPixelOffset=(myWindowWidth-myWindowHeight);
        
//	//removed by Mike, 20210815        
////        printf(">>>DITO: %f",fMyWindowWidthAsPixelRatioToHeightPixel);
////        printf(">>>DITO: myWindowHeight: %i",myWindowHeight);
////        printf(">>>DITO: myWindowWidth: %i",myWindowWidth);
////        printf(">>>DITO: iMyWindowWidthAsPixelOffset: %i",iMyWindowWidthAsPixelOffset);

        
        fMyWindowWidth = fMyWindowHeight; //myWindowWidthAsPixelInput;
        //    	myWindowHeightAsPixel = myWindowHeightAsPixelInput;
    }
*/    
    
    //added by Mike, 20210516
    myUsbongUtils = new UsbongUtils();
    //edited by Mike, 20210815
//    myUsbongUtils->setWindowWidthHeight(myWindowWidth, myWindowHeight); //added by Mike, 20210626
    myUsbongUtils->setWindowWidthHeight(fMyWindowWidth, fMyWindowHeight); //added by Mike, 20210626
    
    //added by Mike, 20211005
    myFont = new Font(fWindowWidth, fWindowHeight);
        
    //added by Mike, 20210903
    myWidth=fMyWindowWidth/1.5f;
    myHeight=fMyWindowHeight/1.5f;
    
    myXPos=0.0f+(fMyWindowWidth-myWidth)/2;
    myYPos=fMyWindowHeight-(fMyWindowHeight-myHeight);
    myZPos=0.0f;
    
    //added by Mike, 20210616
    iTextCurrentMaxColumnCount=0;
    //added by Mike, 20210617
    iTextCurrentMaxRowCount=0;
    
    //init default values
    //previousFacingState=FACING_UP;
    //currentFacingState=FACING_UP;
    
    //added by Mike, 20201225
    bIsFiringBeam=false;
    
    //added by Mike, 20210126
    //	bIsExecutingDash=false, //removed by Mike, 20210128
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
        
    //added by Mike, 20210502
    //note: set this in OpenGLCanvas.cpp
    bIsPlayer2=false;
    
    
    //added by Mike, 20210723
    bHasReachedEndOfTextMessage=false;
    bHasPressedKeyToCloseEndOfTextMessage=false;
    
    //added by Mike, 20210618
    isAtMaxTextCharRow=false;
    idrawPressNextSymbolCount=0;
       
    setCollidable(true);
    
    //added by Mike, 20210614
    //added: function to be reusable
    //  myUsbongUtils->read("inputHalimbawa.txt");
    readInputText((char*)"inputHalimbawa.txt");
    
    //added by Mike, 20210614; edited by Mike, 20210903
//    setupFont(FONT_TEXTURE);		

		//edited by Mike, 20211005
//		glIFontTexture = setupFont((char*)"textures/font.png", fMyWindowWidth, fMyWindowHeight);       
		glIFontTexture = myFont->setupFont((char*)"textures/font.png", fMyWindowWidth, fMyWindowHeight);       
											
		setup();
		
		
		printf(">>glIFontTexture : %i\n",glIFontTexture);				
		printf(">>openGLITexture: %i\n",openGLITexture);			
}

Text::~Text()
{
}

//added by Mike, 20210130
//TO-DO: -reverify: this
float* Text::getXYZPos()
{
    //float myXYZ[3];
    float* myXYZ;
    myXYZ = (float*) malloc(3*sizeof(float));
    
    myXYZ[0]=myXPos;
    myXYZ[1]=myYPos;
    myXYZ[2]=myZPos;
    
    return myXYZ;
}

void Text::drawPressNextSymbolRedSquareQuadOK()
{
/* //removed by Mike, 20211004
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
*/
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0f,0.0f,0.0f); //red	


    float x=myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos);
    float y=myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos);
/* //edited by Mike, 20211004
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);
*/
  
/*    
    float fMySideLength = myWidth/20.0f/1.5f;
        
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMySideLength);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMySideLength);
*/
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);

	
			
			//added by Mike, 20211004
			glTranslatef(x,y,0.0f);			

			//edited by Mike, 20211004
//			glScalef(0.1f, 0.1f, 0.0f);
		//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
		//use correct width x height ratio; window 10x18; row x column
		glScalef(0.20f, 0.35f, 0.0f);


			x=0;
			y=0;

/*
			//set position to be at bottom center
			glScalef(4.0f, 4.0f, 0.0f);
			glTranslatef(0.0f,-texth/2,0.0f);			
*/			
				//note: texture positions inverted
				//set vertex clock-wise
				//texture positions U shape, clock-wise			
				glBegin(GL_QUADS);
//					glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
					glVertex3f(x, y, 0);
					
//					glTexCoord2f(0.0f+0.5f, 0.0f);
					glVertex3f(x, y + texth, 0);
	
//					glTexCoord2f(0.0f, 0.0f);				
					glVertex3f(x + textw, y + texth, 0);
	
//					glTexCoord2f(0.0f, 0.0f+0.5f);				
					glVertex3f(x + textw, y, 0);
				glEnd();

			//added by Mike, 20211004
			//reset
//			glTranslatef(0.0f,texth/2,0.0f);			
			glScalef(1.0f, 1.0f, 1.0f);
			glTranslatef(-x,-y,0.0f);			
			
//	glDisable(GL_TEXTURE_2D);
	
	//reset
	glColor3f(1.0f,1.0f,1.0f); //white	
}

void Text::drawPressNextSymbol()
{
	//added by Mike, 20211005
	glLoadIdentity();

/* //removed by Mike, 20211004
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
*/
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0f,0.0f,0.0f); //red	
	
	float x=myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos);
  float y=myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos);  
  float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
  float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);

  float windowWidth=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMyWindowWidth);
  float windowHeight=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMyWindowHeight);

	//bottom-center
	x=x+windowWidth/2+textw/2;
	//note: /4, instead of 2 due to scale again by 0.5f
	y=0+windowHeight-texth/4; //-texth/2;
			
	//added by Mike, 20211004
	glTranslatef(x,y,0.0f);			

	//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
	//use correct width x height ratio; window 10x18; row x column
	glScalef(0.20f, 0.35f, 0.0f);
	glScalef(0.5f, 0.5f, 0.0f); //added by Mike, 20211005

	x=0;
	y=0;

  //counter-clockwise sequence to auto-draw front face
  //note: origin TOP-LEFT  	
  glBegin(GL_TRIANGLES);
    glVertex3f(x-textw,y,0.0f); //LEFT vertex
    glVertex3f(x,y+texth,0.0f); //BOTTOM-CENTER vertex
    glVertex3f(x+textw,y,0.0f); //RIGHT vertex
  glEnd();

	//added by Mike, 20211004
	//reset
//			glTranslatef(0.0f,texth/2,0.0f);			
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-x,-y,0.0f);			
	
//	glDisable(GL_TEXTURE_2D);
	
	//reset
	glColor3f(1.0f,1.0f,1.0f); //white	
}

void Text::drawPressNextSymbolWithLevel3DAsRedSquareOK()
{
	//added by Mike, 20211005
	glLoadIdentity();
	
/* //removed by Mike, 20211004
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
*/
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0f,0.0f,0.0f); //red	


    float x=myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos);
    float y=myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos);
/* //edited by Mike, 20211004
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);
*/
  
/*    
    float fMySideLength = myWidth/20.0f/1.5f;
        
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMySideLength);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMySideLength);
*/
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);

	
			
			//added by Mike, 20211004
			glTranslatef(x,y,0.0f);			

			//edited by Mike, 20211004
//			glScalef(0.1f, 0.1f, 0.0f);
		//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
		//use correct width x height ratio; window 10x18; row x column
		glScalef(0.20f, 0.35f, 0.0f);


			x=0;
			y=0;

/*
			//set position to be at bottom center
			glScalef(4.0f, 4.0f, 0.0f);
			glTranslatef(0.0f,-texth/2,0.0f);			
*/			
				//note: texture positions inverted
				//set vertex clock-wise
				//texture positions U shape, clock-wise			
				glBegin(GL_QUADS);
//					glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
					glVertex3f(x, y, 0);
					
//					glTexCoord2f(0.0f+0.5f, 0.0f);
					glVertex3f(x, y + texth, 0);
	
//					glTexCoord2f(0.0f, 0.0f);				
					glVertex3f(x + textw, y + texth, 0);
	
//					glTexCoord2f(0.0f, 0.0f+0.5f);				
					glVertex3f(x + textw, y, 0);
				glEnd();

			//added by Mike, 20211004
			//reset
//			glTranslatef(0.0f,texth/2,0.0f);			
			glScalef(1.0f, 1.0f, 1.0f);
			glTranslatef(-x,-y,0.0f);			
			
//	glDisable(GL_TEXTURE_2D);
	
	//reset
	glColor3f(1.0f,1.0f,1.0f); //white	
}

void Text::drawPressNextSymbolWith2DLevelOK()
{
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    glColor3f(1.0f,0.0f,0.0f); //red
  		
  		//note: myWidth length of text background image
//      float fMySideLength = myWidth/20.0f;
      float fMySideLength = myWidth/20.0f/1.5f;

			float fY=myYPos+myHeight/2-fMySideLength;
			float fX=myXPos+myWidth/2;

/*
    	//counter-clockwise sequence to auto-draw front face    	    	
    	glBegin(GL_TRIANGLES);
    		glVertex3f(fX,fY,0.0f); 
    		glVertex3f(fX+fMySideLength,fY,0.0f); 
    		glVertex3f(fX,fY+fMySideLength,0.0f); 
    	glEnd();
*/
    	//counter-clockwise sequence to auto-draw front face
    	//note: origin TOP-LEFT  	
    	glBegin(GL_TRIANGLES);
    		glVertex3f(fX-fMySideLength,fY,0.0f); //LEFT vertex
    		glVertex3f(fX,fY+fMySideLength,0.0f); //BOTTOM-CENTER vertex
    		glVertex3f(fX+fMySideLength,fY,0.0f); //RIGHT vertex
    	glEnd();
    	    	
    	glColor3f(1.0f,1.0f,1.0f); //reset to white
    	
//    glPopMatrix();
}

//added by Mike, 20210827
void Text::drawTextBackgroundAsQuadWithTexture()
{
	//added by Mike, 20210826
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //removed by Mike, 20210829
		glLoadIdentity();
		
    //added by Mike, 20210907
    if (bHasReachedEndOfTextMessage) {
        if(bHasPressedKeyToCloseEndOfTextMessage) {
            return;
        }
    }		


/* //removed by Mike, 20211004
		printf(">>>>myXPos: %f; myYPos: %f;",myXPos,myYPos);
		printf(">>>>vertex myXPos: %f; vertex myYPos: %f;",myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos),myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos));
*/
	
		//edited by Mike, 20211004        
    //openGLDrawTexture(myXPos, myYPos, myWidth, myHeight);
    openGLDrawTexture(myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos), 
    									myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos), 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth), 
    									myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight));
    
    //TO-DO: -update: this due to myWidth, myHeight NOT used
    //edited by Mike, 20211004
//   	drawTextFontAsQuadWithTexture(myXPos, myYPos, myWidth, myHeight);
		//TO-DO: -reverify: this
   	drawTextFontAsQuadWithTexture(0, 0);

 //removed by Mike, 20211004   	
   	//added by Mike, 20210907
		if (isAtMaxTextCharRow) {
		
        if ((idrawPressNextSymbolCount)%2==0) {
        		//added by Mike, 20211004
        		//TO-DO: -reverify: this
            drawPressNextSymbol();
        }
        idrawPressNextSymbolCount=idrawPressNextSymbolCount+1;
	 //removed by Mike, 20211004   	
    }              	


//	drawPressNextSymbol();	

}

//added by Mike, 20210827
void Text::drawTextBackgroundAsQuadWithTextureWith2DLevelOK()
{
	//added by Mike, 20210826
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //removed by Mike, 20210829
		glLoadIdentity();
		
    //added by Mike, 20210907
    if (bHasReachedEndOfTextMessage) {
        if(bHasPressedKeyToCloseEndOfTextMessage) {
            return;
        }
    }		
    
    //removed by Mike, 20211004
//    openGLDrawTexture(myXPos, myYPos, myWidth, myHeight);
    
    //TO-DO: -update: this due to myWidth, myHeight NOT used
    //edited by Mike, 20211004
   	openGLDrawTextureWith2DLevelOK(myXPos, myYPos, myWidth, myHeight);
   	
   	//added by Mike, 20210907
		if (isAtMaxTextCharRow) {
        if ((idrawPressNextSymbolCount)%2==0) {
            drawPressNextSymbol();
        }
        idrawPressNextSymbolCount=idrawPressNextSymbolCount+1;
    }          	
}

    
//TO-DO: -reverify: this
void Text::drawTextFontAsQuadWithTexture2DLevelOK(float x, float y, float textw, float texth)
{
		char tempText[MAX_TEXT_CHAR_ROW_RAM][MAX_TEXT_CHAR_COLUMN];
        
    int iRowCount;
    
    
    glPushMatrix();    

/*    
    
    float fMyWindowWidthAsVertexOffsetInput=(240.0f-iMyWindowWidthAsPixelOffset)*(-1);
    float fMyWindowWidthAsVertexOffset=0.20f+(fMyWindowWidthAsVertexOffsetInput/59.0f*0.04);
    
    //    printf(">>>>>>>>>> fMyWindowWidthAsVertexOffset: %f\n",fMyWindowWidthAsVertexOffset);
    
    glTranslatef(fMyWindowWidthAsVertexOffset, -myUsbongUtils->autoConvertFromPixelToVertexPointY(fMyWindowHeight*0.75f), 0.0f);

    //auto-scale Window Width to Height
    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    //edited by Mike, 20210626
    glScalef(0.26f,0.26f,1.0f);
*/    
  
  //TO-DO: -update: this  
//    glScalef(2.0f,2.0f,2.0f);
    
    
for (iRowCount=0; iRowCount<iTextCurrentMaxRowCount;) {

  for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW_RAM; iRowCountToSetDefault++) {
    for (int iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
        tempText[iRowCountToSetDefault][iColumnCount]='\0'; //verified: in macOS, with Japanese keyboard ro-maji input, "¥0", backspace is "¥"
    }
	}
        
  for (int iColumnCount=0; iColumnCount<iCurrentMaxColumnCountPerRowContainer[iRowCount]; iColumnCount++) {
            tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount]=cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount];
  }
  
//  printf(">>tempText: %s\n",tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  
  //edited by Mike, 20210903      
//  draw_string(glIFontTexture, 0.05f, 1.2f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
//  draw_string(glIFontTexture, x, y, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
//  draw_string(glIFontTexture, x, y+0.1f*iRowCount, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  //edited by Mike, 20210905
//  draw_string(glIFontTexture, x, y+iRowCount*20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
	//edited by Mike, 20210907
//  draw_string(glIFontTexture, x+(20.0f*2.0f), y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
	
	//edited by Mike, 20211005
	//centered; to remove excess margin to the right
//  draw_string(glIFontTexture, x+(20.0f*2.0f)+20.0f, y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  
  myFont->draw_string(glIFontTexture, x+(20.0f*2.0f)+20.0f, y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  
//  glTranslatef(0.0f+0.05f,0.0f+1.2f+0.1f+0.05f,0.0f);

  iTextAnimationCountDelay=0;
  
  if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
      iCurrentMaxColumnCountPerRowContainer[iRowCount]++;
      
      //added by Mike, 20210617
      if (iCurrentMaxColumnCountPerRowContainer[iRowCount]>=MAX_TEXT_CHAR_COLUMN) {
          iCurrentMaxColumnCountPerRowContainer[iRowCount]=MAX_TEXT_CHAR_COLUMN;
      }
  }
        
        
  //'\n'){ //new line; "\0" empty character
  if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iRowCount]-1]=='\n') {
      
  //TO-DO: -add: instructions to auto-identify end row by removing empty rows after reading input file
  //if next row is already empty
  //row, column
  if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iTextCurrentMaxRowCount]]=='\0') {
      //removed by Mike, 20210905
//      iTextCurrentMaxRowCount=iTextCurrentMaxRowCount;
  }
  else {
      
      if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
          iTextCurrentMaxRowCount++;
      }
      
      //added by Mike, 20210618
      //if has reached end of rows, no need to execute this
      //TO-DO: -add: auto-identify if at MAX row
      if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW+1][0]=='\0') {
          printf(">>>>>>>>>>>>>>> WAKAS!\n");
          
          bHasReachedEndOfTextMessage=true;
          
          break;
      }
      else {
/*
          printf(">>> iRowCount: %i\n",iRowCount);
          printf(">>> iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
*/          
          
          if (iRowCount>=MAX_TEXT_CHAR_ROW) {
              iRowCountPageNumber++;
              iTextCurrentMaxRowCount=1;
              iRowCount=-1; //note: we add 1 near the closing of the for loop
          }
      }
    }
  
      //edited by Mike, 20210618
      //re-set isAtMaxTextCharRow to FALSE after button press
      //edited by Mike, 20210905
      if ((iRowCount+1)>=MAX_TEXT_CHAR_ROW) {
//      if ((iRowCount)>=MAX_TEXT_CHAR_ROW) {
      iRowCount=3;
          //                			iRowCountPageNumber=0; //removed by Mike, 20210618
          iTextCurrentMaxRowCount=4;
          isAtMaxTextCharRow=true;
      }
            
            //printf("iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
            
            //added by Mike, 20210617
            //TO-DO: fix: next row only iTextCurrentMaxColumnCount=1       
  		}
  		else {
      		break;
  		}
              
      iTextAnimationCountDelay+=1;
      
        //added by Mike, 20210618
        iRowCount=iRowCount+1;       
    }
    
    //glScalef(1.0f,1.2f,1.0f);
				
	   glDisable(GL_TEXTURE_2D);
	   
	   //removed by Mike, 20211004
//	   glBindTexture(GL_TEXTURE_2D, 0);
	       
    //added by Mike, 20210802
		glPopMatrix();
}


void Text::drawTextFontAsQuadWithTexture(float x, float y)
{
		char tempText[MAX_TEXT_CHAR_ROW_RAM][MAX_TEXT_CHAR_COLUMN];
        
    int iRowCount;
    
    
    glPushMatrix();    
    
			//added by Mike, 20211005
			glLoadIdentity();    
		
/*    
    
    float fMyWindowWidthAsVertexOffsetInput=(240.0f-iMyWindowWidthAsPixelOffset)*(-1);
    float fMyWindowWidthAsVertexOffset=0.20f+(fMyWindowWidthAsVertexOffsetInput/59.0f*0.04);
    
    //    printf(">>>>>>>>>> fMyWindowWidthAsVertexOffset: %f\n",fMyWindowWidthAsVertexOffset);
    
    glTranslatef(fMyWindowWidthAsVertexOffset, -myUsbongUtils->autoConvertFromPixelToVertexPointY(fMyWindowHeight*0.75f), 0.0f);

    //auto-scale Window Width to Height
    glScalef(fMyWindowWidthAsPixelRatioToHeightPixel,1.0f,1.0f);
    
    //edited by Mike, 20210626
    glScalef(0.26f,0.26f,1.0f);
*/    
  
  //TO-DO: -update: this  
    glScalef(2.0f,2.0f,2.0f);
    
    
for (iRowCount=0; iRowCount<iTextCurrentMaxRowCount;) {

  for (int iRowCountToSetDefault=0; iRowCountToSetDefault<MAX_TEXT_CHAR_ROW_RAM; iRowCountToSetDefault++) {
    for (int iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
        tempText[iRowCountToSetDefault][iColumnCount]='\0'; //verified: in macOS, with Japanese keyboard ro-maji input, "¥0", backspace is "¥"
    }
	}
        
  for (int iColumnCount=0; iColumnCount<iCurrentMaxColumnCountPerRowContainer[iRowCount]; iColumnCount++) {
            tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount]=cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iColumnCount];
  }
  
  printf(">>tempText: %s\n",tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  
  //edited by Mike, 20210903      
//  draw_string(glIFontTexture, 0.05f, 1.2f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
//  draw_string(glIFontTexture, x, y, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
//  draw_string(glIFontTexture, x, y+0.1f*iRowCount, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
  //edited by Mike, 20210905
//  draw_string(glIFontTexture, x, y+iRowCount*20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
	//edited by Mike, 20210907
//  draw_string(glIFontTexture, x+(20.0f*2.0f), y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
	//centered; to remove excess margin to the right
/*
//edited by Mike, 20211004	
  draw_string(glIFontTexture, x+(20.0f*2.0f)+20.0f, y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
*/

	//edited by Mike, 20211005
//  draw_string(glIFontTexture, myUsbongUtils->autoConvertFromPixelToVertexPointX(x+(20.0f*2.0f)+20.0f), myUsbongUtils->autoConvertFromPixelToVertexPointY(y+iRowCount*(20.0f*2.0f)+20.0f), 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);

	//edited by Mike, 20211005
	//centered; to remove excess margin to the right
  //draw_string(glIFontTexture, x+(20.0f*2.0f)+20.0f, y+iRowCount*(20.0f*2.0f)+20.0f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
/*  
  myFont->draw_string(glIFontTexture, myXPos, myYPos, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
*/
	//edited by Mike, 20211005
//  myFont->draw_string(glIFontTexture, x, y, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
//  myFont->draw_string(glIFontTexture, x, y-iRowCount*0.1f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);
    myFont->draw_string(glIFontTexture, myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos+75.0f), myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos)-iRowCount*0.1f, 0.0f, tempText[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW]);

//  glTranslatef(0.0f+0.05f,0.0f+1.2f+0.1f+0.05f,0.0f);

  iTextAnimationCountDelay=0;
  
  if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
      iCurrentMaxColumnCountPerRowContainer[iRowCount]++;
      
      //added by Mike, 20210617
      if (iCurrentMaxColumnCountPerRowContainer[iRowCount]>=MAX_TEXT_CHAR_COLUMN) {
          iCurrentMaxColumnCountPerRowContainer[iRowCount]=MAX_TEXT_CHAR_COLUMN;
      }
  }
        
        
  //'\n'){ //new line; "\0" empty character
  if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iRowCount]-1]=='\n') {
      
  //TO-DO: -add: instructions to auto-identify end row by removing empty rows after reading input file
  //if next row is already empty
  //row, column
  if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW][iCurrentMaxColumnCountPerRowContainer[iTextCurrentMaxRowCount]]=='\0') {
      //removed by Mike, 20210905
//      iTextCurrentMaxRowCount=iTextCurrentMaxRowCount;
  }
  else {
      
      if ((iRowCount)==(iTextCurrentMaxRowCount-1)) {
          iTextCurrentMaxRowCount++;
      }
      
      //added by Mike, 20210618
      //if has reached end of rows, no need to execute this
      //TO-DO: -add: auto-identify if at MAX row
      if (cCurrentTextContainer[iRowCount+iRowCountPageNumber*MAX_TEXT_CHAR_ROW+1][0]=='\0') {
          printf(">>>>>>>>>>>>>>> WAKAS!\n");
          
          bHasReachedEndOfTextMessage=true;
          
          break;
      }
      else {
/*
          printf(">>> iRowCount: %i\n",iRowCount);
          printf(">>> iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
*/          
          
          if (iRowCount>=MAX_TEXT_CHAR_ROW) {
              iRowCountPageNumber++;
              iTextCurrentMaxRowCount=1;
              iRowCount=-1; //note: we add 1 near the closing of the for loop
          }
      }
    }
  
      //edited by Mike, 20210618
      //re-set isAtMaxTextCharRow to FALSE after button press
      //edited by Mike, 20210905
      if ((iRowCount+1)>=MAX_TEXT_CHAR_ROW) {
//      if ((iRowCount)>=MAX_TEXT_CHAR_ROW) {
      iRowCount=3;
          //                			iRowCountPageNumber=0; //removed by Mike, 20210618
          iTextCurrentMaxRowCount=4;
          isAtMaxTextCharRow=true;
      }
            
            //printf("iTextCurrentMaxRowCount: %i\n",iTextCurrentMaxRowCount);
            
            //added by Mike, 20210617
            //TO-DO: fix: next row only iTextCurrentMaxColumnCount=1       
  		}
  		else {
      		break;
  		}
              
      iTextAnimationCountDelay+=1;
      
        //added by Mike, 20210618
        iRowCount=iRowCount+1;       
    }
    
    //glScalef(1.0f,1.2f,1.0f);
				
	   glDisable(GL_TEXTURE_2D);
	   
	   //removed by Mike, 20211004
//	   glBindTexture(GL_TEXTURE_2D, 0);
	       
    //added by Mike, 20210802
		glPopMatrix();
}

//added: by Mike, 20210423
//TO-DO: -add: in PolygonPool
//added by Mike, 20210516
//note: origin/anchor is TOP-LEFT
void Text::drawTextBackgroundObject()
{
    /*
     glBegin(GL_LINES);
     //edited by Mike, 20210514
     //		glColor3f(1.0f,0.0f,0.0f); //red
     //sky blue color; brighter
     glColor3f(0.69f, 0.84f, 1.0f);
     //diagonal line left to right
     glVertex2f(-1.0f, 1.0f);
     glVertex2f(1.0f, -1.0f);
     glEnd();
     */
    
    //added by Mike, 20210422
    glPushMatrix();
    //added by Mike, 20210420
    glColor3f(1.0f, 1.0f, 1.0f); // white
    //		glColor3f(1.0f, 0.0f, 0.0f); // red
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TEXT_TEXTURE_A);
    
    //edited by Mike, 20210515
    fButtonAnimationFrameOffset=0;
    
    //added by Mike, 20210516; removed to after glScale(...) by Mike, 20210516
    //due to instructions to auto-draw quad using triangles
    //		glTranslatef(0.2f, 0.2f, 0.0f);
    
    //TO-DO: -verify: scaled texture object if equal with pixel width and height size
    //use autoConvertFromPixelToVertexPointX, et cetera if exact
    
/*
    printf(">>iMyWindowWidthAsPixelOffset: %i",iMyWindowWidthAsPixelOffset); //example: 240
    printf(">>myWindowWidth: %i",myWindowWidth); //example 800; //offset already subtracted
*/
    
    //TO-DO: -reverify: in another machine
//     glTranslatef(0.23f, 0.0f, 0.0f); //note: no need to execute glTranslatef(...)
//     glScalef(1.0f-0.23f, 1.0f, 1.0f); //OK in macOS machine
//    glScalef(1.0f-(iMyWindowWidthAsPixelOffset*1.0f)/(myWindowWidth+iMyWindowWidthAsPixelOffset), 1.0f, 1.0f); //OK also in macOS machine\
	//edited by Mike, 20210815
//    glScalef(1.0f-(iMyWindowWidthAsPixelOffset*1.0f)/(myWindowWidth), 1.0f, 1.0f); //OK
    glScalef(1.0f-(iMyWindowWidthAsPixelOffset*1.0f)/(fMyWindowWidth), 1.0f, 1.0f); //OK

    //added by Mike, 20210725
    //TO-DO: -reverify: set of instructions with another machine's screen/monitor
    //glTranslatef(0.12f, 0.2f, 0.0f);
//    glTranslatef(0.2f, 0.2f, 0.0f);
//    glTranslatef(0.2f, 0.4f, 0.0f);
    
    //window width and height; 640x640pixels
    //whole texture image sheet 512x256pixels
    //button size: 64x16pixels
    //		glScalef(0.25f, 0.4f, 1.0f);
    //    glScalef(0.20f, 0.4f, 1.0f);
    //    glScalef(0.25f, 0.4f, 1.0f);
//    glScalef(0.28f, 0.4f, 1.0f);
    
    /* //edited by Mike, 20210725
     //added by Mike, 20210516
     //due to instructions to auto-draw quad using triangles
     //    glTranslatef(1.0f, 0.5f, 0.0f);
     glTranslatef(0.4f, 0.5f, 0.0f);
     */
    
    //added by Mike, 20210724
    //note: adding background texture causes delay in execution
    //verified: using GL_TRIANGLES and GL_QUAD
    //note: we can stop Pilot movement due to input Command
    //when display text is executing
    
    //edited by Mike, 20210724
    glBegin(GL_TRIANGLES);
    //counter-clockwise sequence to auto-draw front face
    //triangle#6 //front face left part
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A1; face left
    glVertex3f(-1.000000,1.000000,0.000000); //A1
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,1.0);	//C1; face left
    glVertex3f(-1.000000,-1.000000,0.000000); //C1
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,1.0); //B1; face left
    glVertex3f(1.000000,-1.000000,0.000000); //B1
    
    //triangle#12 //front face right part
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A2; face lefT
    glVertex3f(-1.000000,1.000000,0.000000); //A2
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,1.0); //C2; face left
    glVertex3f(1.000000,-1.000000,0.000000); //C2
    
    glNormal3f(0.0000,0.0000,0.0000);
    glTexCoord2f(0.25+fButtonAnimationFrameOffset,0.0); //B2; face left
    glVertex3f(1.000000,1.000000,0.000000); //B2
    glEnd();
    
    
    /*  //added by Mike, 20210724
     glDisable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, 0);
     glColor3f(1.0f, 1.0f, 1.0f); //set to default, i.e. white
     
     float fGridTileWidthVertexPosition=1.0f;
     float fGridTileHeightVertexPosition=1.0f;
     //counter-clockwise sequence to auto-draw front face
     glBegin(GL_QUADS); // Each set of 4 vertices form a quad
     //    	glTexCoord2f(fTx, fTy);
     //	    glTexCoord2f(0.0+fButtonAnimationFrameOffset,0.0);	//A1; face left
     //    	glVertex3f(0.0f, 0.0f, 0.0f);
     //    	glVertex3f(-1.0f, 1.0f, 0.0f);
    	glVertex3f(-1.0f, 0.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx + fTileSideXAxis, fTy);
    	glVertex3f(-1.0f, -1.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx + fTileSideXAxis, fTy + fTileSideYAxis);
    	glVertex3f(1.0f, -1.0f, 0.0f);
    	
     //    	glTexCoord2f(fTx, fTy + fTileSideYAxis);
     //    	glVertex3f(1.0f, 1.0f, 0.0f);
    	glVertex3f(1.0f, 0.0f, 0.0f);
     glEnd();
     */
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix(); //added by Mike, 20210422
}

//added by Mike, 20210423
void Text::update(float dt)
{
    switch (currentState)
    {
        case INITIALIZING_STATE:
        case MOVING_STATE:
            switch(currentMovingState) {
                case WALKING_MOVING_STATE:
                    break;
                case ATTACKING_MOVING_STATE:
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
            
            rotationAngle=0; //TO-DO: -update: this
            
            /* //removed by Mike, 20210424
             //Note: Use these with update to OpenGLCanvas
           		//wrap the world
           		if (myXPos <= 0.0f) myXPos = myWindowWidth/100-myWidth/8; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side
           		if (myZPos >= myWindowHeight/100) myZPos = 0.0f+myHeight/8; //if bottom side
           		else if (myZPos <= 0.0f) myZPos = myWindowHeight/100-myHeight/8; //if top side
             */
            
            
            //added by Mike, 20210503
            //Note: Use these with update to OpenGLCanvas
            //max world
            /*
           		if (myXPos <= 15.0f) myXPos = 0-15.0f; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side
             */
            /*	//removed by Mike, 20210504
             printf(">> myXPos: %f\n",myXPos);
             printf(">> stepX: %f\n",stepX);
             //TO-DO: -update: instructions to do not execute step if already at border
             */
            /*
           		if ((myXPos-stepX) <= -20.70f) myXPos = 0.0f-21.0f+stepX; //+myWidth+stepX; //if left side
           		else if (myXPos >= myWindowWidth/100) myXPos = 0.0f+myWidth/8; //if right side
             */
            /*	//removed by Mike, 20210504
           		if ((myXPos-stepX) <= -21.0f) myXPos = 0.0f-21.0f+stepX; //if left side
           		else if (myXPos+stepX >= 4.0f) myXPos = 0.0f+4.0f-stepX; //if right side
             */
            break;
        case IN_TITLE_STATE:
            rotationAngle+=5;//rotationStep;
            break;
        default: //STANDING STATE
            break;//do nothing
    }
}

void Text::changeState(int s)
{
    currentState=s;
}

//added by Mike, 20201226
void Text::keyDown(int keyCode) {
    myKeysDown[keyCode] = TRUE;
    
//    printf(">>keyDown: %i",keyCode);
    //added by Mike, 20210619
    //TO-DO: -reverify: output of over 6 rows in input file
    if (myKeysDown[KEY_K]==TRUE) {
        //edited by Mike, 20210723
        if (bHasReachedEndOfTextMessage) {
            bHasPressedKeyToCloseEndOfTextMessage=true;
        }
        else {
            if (isAtMaxTextCharRow) {
                isAtMaxTextCharRow=false;
                
                iRowCountPageNumber++;
                iTextCurrentMaxRowCount=1;
                
                //next row; reminder: MAX_TEXT_CHAR_ROW=4
                for(int iCount=0; iCount<MAX_TEXT_CHAR_ROW; iCount++) {
                    iCurrentMaxColumnCountPerRowContainer[iCount]=1;
                }
            }
        }
    }
    
    //removed by Mike, 20210619
    //added by Mike, 20210127; edited by Mike, 20210128
    //    autoVerifyDashStateWithKeyDown();//keyCode);
}

/* //removed by Mike, 20210905
//added by Mike, 20201227; edited by Mike, 20210128
//void Text::setDashStateWithKeyDown() {
void Text::setDashStateWithKeyDown(int keyCode) {
    if (bIsDashReady==true) {
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
*/

/* //removed by Mike, 20210905
//added by Mike, 20201226; edited by Mike, 20210128
//void Text::autoVerifyDashStateWithKeyDown(int keyCode) {
void Text::autoVerifyDashStateWithKeyDown() { //int keyCode) {
    //edited by Mike, 20210128
    //if (myKeysDown[KEY_RIGHT]==TRUE) {
    //edited by Mike, 20210130
    //	if ((myKeysDown[KEY_RIGHT]==TRUE) || (myKeysDown[KEY_D]==TRUE)) {
    if (myKeysDown[KEY_D]==TRUE) {
        setDashStateWithKeyDown(KEY_D);
    }
    //edited by Mike, 20210130
    //	else if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
    else if (myKeysDown[KEY_W]==TRUE) {
        setDashStateWithKeyDown(KEY_W);//KEY_UP);
    }
    else if (myKeysDown[KEY_A]==TRUE) {
        setDashStateWithKeyDown(KEY_A);
    }
    else if (myKeysDown[KEY_S]==TRUE) {
        setDashStateWithKeyDown(KEY_S);
    }
}
*/

void Text::keyUp(int keyCode) {
    //added by Mike, 20210127
    autoVerifyDashStateWithKeyUp(keyCode);
    
    myKeysDown[keyCode] = FALSE;
}

//added by Mike, 20210127; edited by Mike, 20210126
/*
 void Text::setDashStateWithKeyUp() {
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
//void Text::setDashStateWithKeyUp() {
void Text::setDashStateWithKeyUp(int keyCode) {
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
        if (bIsExecutingDashArray[KEY_W]) {
            bIsExecutingDashArray[KEY_W]=false;//KEY_UP);
            bIsDashReady=false;
        }
        else if (bIsExecutingDashArray[KEY_A]) {
            bIsExecutingDashArray[KEY_A]=false;
            bIsDashReady=false;
        }
        else if (bIsExecutingDashArray[KEY_S]) {
            bIsExecutingDashArray[KEY_S]=false;
            bIsDashReady=false;
        }
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
void Text::autoVerifyDashStateWithKeyUp(int keyCode) {
    //added by Mike, 20210126; edited by Mike, 20210128
    //	if (keyCode==KEY_RIGHT) {
    //edited by Mike, 20210130
    //	if ((keyCode==KEY_RIGHT) || (keyCode==KEY_D)) {
    if (keyCode==KEY_D) {
        //edited by Mike, 20210128
        //		if (myKeysDown[KEY_RIGHT]==TRUE) {
        if (myKeysDown[KEY_D]==TRUE) {
            //edited by Mike, 20210129
            //			setDashStateWithKeyUp();
            setDashStateWithKeyUp(KEY_D);
        }
    }
    //edited by Mike, 20210130
    //	else if ((keyCode==KEY_UP) || (keyCode==KEY_W)) {
    else if (keyCode==KEY_W) {
        //edited by Mike, 20210130
        //		if ((myKeysDown[KEY_UP]==TRUE) || (myKeysDown[KEY_W]==TRUE)) {
        if (myKeysDown[KEY_W]==TRUE) {
            //edited by Mike, 20210129
            //			setDashStateWithKeyUp();
            setDashStateWithKeyUp(KEY_W);
        }
    }
    else if (keyCode==KEY_A) {
        if (myKeysDown[KEY_A]==TRUE) {
            //edited by Mike, 20210129
            //			setDashStateWithKeyUp();
            setDashStateWithKeyUp(KEY_A);
        }
    }
    else if (keyCode==KEY_S) {
        if (myKeysDown[KEY_S]==TRUE) {
            //edited by Mike, 20210129
            //			setDashStateWithKeyUp();
            setDashStateWithKeyUp(KEY_S);
        }
    }
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


void Text::move(int key)
{
    //Note: Unit member as Pilot has to release hold of directional keys,
    //so that RobotShip faces in the correct direction;
    //holding the fire beam key, e.g. KEY_I, causes RobotShip
    //to move without changing the direction that it faces
    
    //added by Mike, 20201201; removed by Mike, 20201226
    //   currentMovingState=WALKING_MOVING_STATE;
    
    //added by Mike, 20201225; removed by Mike, 20201225
    //bIsFiringBeam=false;
    
    //TO-DO: -add: bIsMovingLeft, etc, while facing up, etc
    
    //added by Mike, 20201226; removed by Mike, 20201226
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
            //TO-DO: +reverified: Robotship does not execute correctly
            //when down and left buttons are pressed while firing beam down
            //AND when up and left buttons are pressed while firing beam up
            //in Windows Machine
            //problem did not occur on Linux Machine (with external USB keyboard)
            //added by Mike, 20210131
            //note: add use of external USB keyboard solves the problem
            
            //          for (int iCount=0; iCount<10; iCount++) {
            //edited by Mike, 20210130
            //note: in RobotShip.h set PILOT_MAX_DIRECTIONAL_KEY_DASH_COUNT=4, not 6,
            //causes RobotShip movement error
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
            //isMovingForward=1;
            //myZPos-=1.0f;
            /*          if (thrust<thrustMax)
             thrust+=0.1f;
             */
            
            //added by Mike, 20210111
            if (bIsExecutingPunch) {
            }
            //added by Mike, 20210121
            else if (bIsExecutingDefend) {
            }
            else {
                /*	//removed by Mike, 20210502
                 //added by Mike, 20201001; edited by Mike, 20201116
                 //	      myYPos+=-stepY;
                 myZPos+=-stepZ;
                 //added by Mike, 20210127; edited by Mike, 20210128
                 //			if (bIsExecutingDash) {
                 //edited by Mike, 20210130
                 if (bIsExecutingDashArray[KEY_W]) {
                 
                 
                 //			if ((bIsExecutingDashArray[KEY_UP]) || (bIsExecutingDashArray[KEY_W])) {
                 //			if ((bIsExecutingDashArray[KEY_W])) {
                 myZPos+=-stepZ;
                 }
                 */
            }
            
            //added by Mike, 20201201; edited by Mike, 20201225
            //currentFacingState=FACING_UP;
            if (bIsFiringBeam) {
            }
            else {
                //removed by Mike, 20210425
                //TO-DO: -add: this
                //      		currentFacingState=FACING_UP;
            }
            
            //added by Mike, 20201226; edited by Mike, 20210502
            //   		  currentMovingState=WALKING_MOVING_STATE;
            currentMovingState=IDLE_MOVING_STATE;
            break;
            
            //     case KEY_DOWN:  //removed by Mike, 20210130
        case KEY_S: //added by Mike, 20210128
            /*     	  //added by Mike, 20201001
             if (thrust<thrustMax)
             thrust+=-0.1f;
             */
            
            //added by Mike, 20210111
            if (bIsExecutingPunch) {
            }
            //added by Mike, 20210121
            else if (bIsExecutingDefend) {
            }
            else {
                /* //removed by Mike, 20210502
                 //added by Mike, 20201001; edited by Mike, 20201116
                 //	      myYPos+=stepY;
                 myZPos+=stepZ;
                 //added by Mike, 20210127; edited by Mike, 20210128
                 //			if (bIsExecutingDash) {
                 if ((bIsExecutingDashArray[KEY_S])) {
                 myZPos+=stepZ;
                 }
                 */
            }
            //added by Mike, 20201201; edited by Mike, 20201225
            //currentFacingState=FACING_DOWN;
            if (bIsFiringBeam) {
            }
            else {
                //removed by Mike, 20210425
                //TO-DO: -add: this
                //currentFacingState=FACING_DOWN;
            }
            
            //added by Mike, 20201226; edited by Mike, 20210502
            //   		currentMovingState=WALKING_MOVING_STATE;
            currentMovingState=IDLE_MOVING_STATE;
            break;
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
            else {
                //added by Mike, 20210504
                /*			printf(">> myXPos: %f\n",myXPos);
                 printf(">> stepX: %f\n",stepX);
                 */
                
                //max world left border
                /*		//edited  by Mike, 20210504
                 //added by Mike, 20201001
                 myXPos+=-stepX;
                 //added by Mike, 20210127; edited by Mike, 20210128
                 //			if (bIsExecutingDash) {
                 if ((bIsExecutingDashArray[KEY_A])) {
                 myXPos+=-stepX;
                 }
                 */
                //do not execute step if already at border
                if ((myXPos) <= -21.0f) {
                }
                else {
                    //added by Mike, 20201001
                    myXPos+=-stepX;
                    
                    //added by Mike, 20210127; edited by Mike, 20210128
                    //			if (bIsExecutingDash) {
                    if ((bIsExecutingDashArray[KEY_A])) {
                        myXPos+=-stepX;
                    }
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
                //removed by Mike, 20210502
                /*          	currentFacingState=FACING_LEFT;
                 */
            }
            
            //added by Mike, 20201226
            currentMovingState=WALKING_MOVING_STATE;
            break;
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
            else {
                /* //edited by Mike, 20210504
                 //added by Mike, 20201001
                 myXPos+=stepX;
                 //added by Mike, 20210126; edited by Mike, 20210128
                 //			if (bIsExecutingDash) {
                 if ((bIsExecutingDashArray[KEY_D])) {
                 myXPos+=stepX;
                 }openGLDrawTexture
                 */
                //		printf("myXPos: %f",myXPos);
                //do not execute step if already at border
                if ((myXPos) >= 4.0f) {
                }
                else {
                    myXPos+=stepX;
                    
                    if ((bIsExecutingDashArray[KEY_D])) {
                        myXPos+=stepX;
                    }
                }
            }
            
            
            
            //added by Mike, 20201201; edited by Mike, 20201225
            //currentFacingState=FACING_RIGHT;
            if (bIsFiringBeam) {
            }
            else {
                currentFacingState=FACING_RIGHT;
            }
            
            
            //added by Mike, 20201226
            currentMovingState=WALKING_MOVING_STATE;
            break;
            //added by Mike, 20201201
        default:
            currentMovingState=IDLE_MOVING_STATE;
            bIsFiringBeam=false; //added by Mike, 20201226
            //removed by Mike, 20210123
            //bIsExecutingPunch=false; //added by Mike, 20210111
            
            bIsExecutingDefend=false; //added by Mike, 20210121
            break;
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
    
    //added by Mike, 20210502
    //TO-DO: -add: FACING_LEFT based on opponent position, e.g. left of pilot
    
    
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
}
void Text::hitBy(MyDynamicObject* mdo)
{
    //changeState(DEATH_STATE);
    //setCollidable(false);
    myOpenGLCanvas->loseLife();
    
    //removed by Mike, 20201001
    /*
     zing = sound->load_sound_clip(RESETSOUND);
     sound->play_sound_clip(zing);
     */
    reset();
}

/*	//removed by Mike, 20210522
 void Text::setOpenGLCanvas(OpenGLCanvas* c)
 {
 myOpenGLCanvas = c;
 }
 */

//edited by Mike, 20211004
void Text::openGLDrawTextureWith2DLevelOK(float x, float y, float textw, float texth)
{
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);

	//removed by Mike, 20210903
//	textw=textw*2;

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, 0);
		
		glTexCoord2f(0.0f+0.5f, 0.0f);
		glVertex3f(x + textw, y, 0);
		
		glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
		glVertex3f(x + textw, y + texth, 0);
		
		glTexCoord2f(0.0f, 0.0f+0.5f);
		glVertex3f(x, y + texth, 0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void Text::openGLDrawTexture(float x, float y, float textw, float texth)
{
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);

	//removed by Mike, 20210903
//	textw=textw*2;

/* //edited by Mike, 20211004
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, 0);
		
		glTexCoord2f(0.0f+0.5f, 0.0f);
		glVertex3f(x + textw, y, 0);
		
		glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
		glVertex3f(x + textw, y + texth, 0);
		
		glTexCoord2f(0.0f, 0.0f+0.5f);
		glVertex3f(x, y + texth, 0);
	glEnd();
*/	
			
			//added by Mike, 20211004
			glTranslatef(x,y,0.0f);			

			x=0;
			y=0;

			//set position to be at bottom center
			glScalef(4.0f, 4.0f, 0.0f);
			glTranslatef(0.0f,-texth/2,0.0f);			
			
				//note: texture positions inverted
				//set vertex clock-wise
				//texture positions U shape, clock-wise			
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
					glVertex3f(x, y, 0);
					
					glTexCoord2f(0.0f+0.5f, 0.0f);
					glVertex3f(x, y + texth, 0);
	
					glTexCoord2f(0.0f, 0.0f);				
					glVertex3f(x + textw, y + texth, 0);
	
					glTexCoord2f(0.0f, 0.0f+0.5f);				
					glVertex3f(x + textw, y, 0);
				glEnd();

			//added by Mike, 20211004
			//reset
			glTranslatef(0.0f,texth/2,0.0f);			
			glScalef(1.0f, 1.0f, 0.0f);
			glTranslatef(-x,-y,0.0f);			
			
	glDisable(GL_TEXTURE_2D);
}




void Text::reset()
{
    changeState(INITIALIZING_STATE);
    myXPos=0;
    myYPos=0;
    rotationAngle=0.0f;
    thrust=0.0f;
    setCollidable(false);
    invincibleCounter=0;
}
int Text::getState()
{
    return currentState;
}

//added by Mike, 20201016
void Text::destroy()
{
    /*
     for(int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i) {
	    delete [] explosionParticle[i];
     }
     delete [] explosionParticle;
     */
}


//added by Mike, 20210614
//note: error occurs if excess rows > 1
//TO-DO: -fix: this
void Text::readInputText(char *inputFilename) {
    int c;
    FILE *file;
    
    //TO-DO: update: this
    //	char** iCurrentLevelMapContainer = new char[100][100];
    int iRowCount=0;
    int iColumnCount=0;
    
    //TO-DO: -update: this
    /*	int MAX_TEXT_CHAR_ROW=2;
     int MAX_TEXT_CHAR_COLUMN=8;
     */
    
    //edited by Mike, 20210618
    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW; iRowCount++) {
        //    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
            //edited by Mike, 20210616
            //sCurrentTextContainer[iRowCount][iColumnCount]=(char*)"-1";//'G';
            //edited by Mike, 20210617
            //            cCurrentTextContainer[iRowCount][iColumnCount]='\n';
            //            cCurrentTextContainer[iRowCount][iColumnCount]='\0';
            
            //added by Mike, 20210617
            iCurrentMaxColumnCountPerRowContainer[iRowCount]=1;
        }
    }
    
    //added by Mike, 20210618
    for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
        for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
            cCurrentTextContainer[iRowCount][iColumnCount]='\0';
        }
    }
    
    
    
    /*
     //added by Mike, 20210618
     for (iRowCount=0; iRowCount<MAX_TEXT_CHAR_ROW_RAM; iRowCount++) {
     for (iColumnCount=0; iColumnCount<MAX_TEXT_CHAR_COLUMN; iColumnCount++) {
     iCurrentMaxColumnCountPerRowContainer[iRowCount]=1;
     }
     }
     */
    
    iRowCount=0;
    iColumnCount=0;
    
    //added by Mike, 20210617
    iTextCurrentMaxRowCount=1;
    
    //added by Mike, 20210618
    iRowCountPageNumber=0; //start at zero
    
				
    //noted by Mike, 20201210
    //note: if concatenated string exceeds size, "stack smashing detected"; terminated; Aborted (core dumped)
    //I prefer to set a size, instead of dynamically allocate due to increasing likelihood of memory leaks
    //where memory leaks = not deallocated storage in memory, albeit not used by software application
    //identifying not deallocated storage in memory becomes more difficult with increasing use
    //edited by Mike, 20210615
    //	char input[MAX_TEXT_CHAR_COLUMN]; //max size
    //TO-DO: -update: this; if total size of the input filename > total size of container, in macOS abort trap 6 error
    char input[100]; //max size in Char of input filename
    
    char inputTextLine[MAX_TEXT_CHAR_COLUMN]; //max size
    char tempInputTextLine[MAX_TEXT_CHAR_COLUMN]; //max size
    
    //added by Mike, 20210618
    //TO-DO: -add: auto-notify Unit member if input over max
    
    strcpy(input, "input/");
    strcat(input, inputFilename); //already includes .txt
    //	strcat(input,".txt");
    
    //	printf("dito: %s",input);
    
    //	file = fopen("input/"+inputFilename, "r"); //.txt file
    //	file = fopen("input/inputHalimbawa.txt", "r"); //.txt file
    file = fopen(input, "r"); //.txt file
    
    //TO-DO: -reverify: tile positions
    int iCount=0;
    
    strcpy(tempInputTextLine,""); //added by Mike, 20210615
    
    if (file) {
        //edited by Mike, 20210210
        //		while ((c = getc(file)) != EOF) {
        //edited by Mike, 20210516
        //		while (fgets (input, MAX_TEXT_CHAR_COLUMN, file)) { /* read each line of input */
        while (fgets (inputTextLine, MAX_TEXT_CHAR_COLUMN, file)) { /* read each line of input */
            
            //	putchar(c);
            
            /*	//removed by Mike, 20210210
             char sRow[2] = {(char)c};
             */
            //delimited using new line
            /*			char *chRow = strtok(sRow, "\n");
             */
            
            //            printf(">>> input: %s\n",input);
            //			sscanf (input, "%s", inputTextLine);
            
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
            //edited by Mike, 20210616
            //TO-DO: -verify: add tool for computer to notify Unit member if max characters per row already exceeded
            //at present, 19 characters including space
            //TO-DO: -add: auto-put excess characters in next row
            //TO-DO: -fix: text background image texture file after scale up
            //TO-DO: -add: remaining Font characters
            //TO-DO: -update: Font characters to use handwritting
            //TO-DO: -reverify: max rows due to text length long in Filipino language
            //TO-DO: -add: auto-write text per character
            //TO-DO: -add: auto-pause for text with several rows
            strcpy(tempInputTextLine,inputTextLine);
            
/* //removed by Mike, 20211004            
            printf(">>> inputTextLine: %s\n",inputTextLine);
*/            
            
            /* 	//edited by Mike, 20210616
             //note: add "-1" for empty
             //otherwise, comma as column is skipped
             //edited by Mike, 20210615
             //			char *ch = strtok(tempInputTextLine, ",");
             char *ch = strtok(tempInputTextLine, "\n");
             
             while (ch != NULL) {
             //				printf("%i,",iColumnCount);
             MAX_TEXT_CHAR_ROW_RAM
             //TO-DO: use String, instead of char
             //TO-DO: -reverify: output due to "G" not put in container
             //				sCurrentLevelMapContainer[iRowCount][iColumnCount]=&ch;
             sCurrentTextContainer[iRowCount][iColumnCount]=ch;
             //				printf("%i:",iColumnCount);
             //				printf("%s,",ch);
             
             iColumnCount=iColumnCount+1;
             
             //edited by Mike, 20210615
             //				  ch = strtok(NULL, ",");
             ch = strtok(NULL, "\n");
             }
             
             */
            
            //added by Mike, 20210617
            /*            if (cCurrentTextContainer[iRowCount][iCharCount]=='\0') {
             break;
             }
             */
            
            //added by Mike, 20210617
            //TO-DO: -add: trim to input text line
            
            int iCharCount;
            for (iCharCount=0; iCharCount<strlen(tempInputTextLine); iCharCount++) {
                cCurrentTextContainer[iRowCount][iCharCount]=tempInputTextLine[iCharCount];
                
                printf("cCurrentTextContainer[%i][%i]: %c",iRowCount, iCharCount, tempInputTextLine[iCharCount]);
            }
            //added by Mike, 20210617
            cCurrentTextContainer[iRowCount][iCharCount]='\n';
            
            
            
            iColumnCount=iColumnCount+1;
            
            //removed by Mike, 20210617
            //edited by Mike, 20210311
            //			if (iRowCount<100) {
            //edited by Mike, 20210321
            //			if (iRowCount<160) {
            //edited by Mike, 20210618
            //            if (iRowCount<MAX_TEXT_CHAR_ROW) {
            if (iRowCount<MAX_TEXT_CHAR_ROW_RAM) {
                iRowCount=iRowCount+1;
                
                //TO-DO: -notify: Unit member if over MAX_TEXT_CHAR_ROW_RAM
            }
            else {
                iRowCount=0;
            }
            
            //removed by Mike, 20210617
            //            iTextCurrentMaxRowCount=iTextCurrentMaxRowCount+1;
            
            printf("\n");
        }
        fclose(file);
        
        //added by Mike, 20210615
        //        free(tempInputTextLine);
    }
}

//--------------------------------------------
