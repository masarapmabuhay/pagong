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
 * @date created: 20201010
 * @date updated: 20211009
 *
 * Acknowledgments:
 * 1) "Bulalakaw Wars" Team (2007): 
 * Syson, M., Camacho, R., Gonzales, D., Del Rosario, R., Vidal, E., et al.
 *
 */
 
/*
 * This program demonstrates how to render
 * a simple heads-up display.  It uses a
 * font texture to display the text.
 *
 * Eric Vidal
 */

//#include <SDL.h>

//TO-DO: -update: this

//added by Mike, 20201010
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

//added by Mike, 20210903
//note: reverifying: use of SDL Image + OpenGL, without GLUT
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

#include <stdio.h>
#include <stdlib.h>

//edited by Mike, 20201011
//#include <string.h>
#include <string>

//added by Mike, 20201011
#include <iostream>

//added by Mike, 20211005
#include "Font.h"
#include "UsbongUtils.h"

/*****************************************************************************/

GLboolean Font::test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}

//added by Mike, 20211005
Font::Font(float fMyWindowWidthInput, float fMyWindowHeightInput)
{    
		myUsbongUtils = new UsbongUtils();
    myUsbongUtils->setWindowWidthHeight(fMyWindowWidthInput, fMyWindowHeightInput);
    
    //added by Mike, 20211005
    myWidth=fMyWindowWidthInput/1.5f;
    myHeight=fMyWindowHeightInput/1.5f;
    
    //added by Mike, 20211008
    myXPos=0;
    myYPos=0;
    fMyWindowWidth=fMyWindowWidthInput;
    fMyWindowHeight=fMyWindowHeightInput;
}


Font::Font(float xPos, float yPos, float zPos, float fWindowWidth, float fWindowHeight): MyDynamicObject(xPos,yPos,0.0f, fWindowWidth, fWindowHeight)
{
}

/*****************************************************************************/

/* text drawing */

void Font::draw_char(GLuint glIFontTexture, float x, float y, float z, char c)
{
    //added by Mike, 20211005; removed by Mike, 20211008
	//glLoadIdentity();
	
 //removed by Mike, 20211004
	glBindTexture(GL_TEXTURE_2D, glIFontTexture); 
	glEnable(GL_TEXTURE_2D);

/*
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0f,0.0f,0.0f); //red	
*/

    GLfloat tx, ty, tz;

    // check if the character is valid
    if (c < ' ' || c > '~') {
        return;
    }

    //subtract 32, since the first character in the font texture
    //is the space (ascii value 32)
    c = c - 32;

    //each character in the font texture image file
    //has a width-height ratio of 10:16
    tx = c % 12 * 0.078125f;
    
//    printf(">>c: %i\n",c);

    ty = (c / 12 * 0.125f);    
       
      
/*    
    float fMySideLength = myWidth/20.0f/1.5f;        
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMySideLength);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMySideLength);
*/

/* //edited by Mike, 20211004
    x=myUsbongUtils->autoConvertFromPixelToVertexPointX(x);
    y=myUsbongUtils->autoConvertFromPixelToVertexPointY(y);
*/

/*
		x=0;
		y=0;
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
		glScalef(0.6f, 0.8f, 0.0f); //added by Mike, 20211005

/*
			//set position to be at bottom center
			glScalef(4.0f, 4.0f, 0.0f);
			glTranslatef(0.0f,-texth/2,0.0f);			
*/			
				//note: updated this set of instructions to fix NOT auto-drawn problem
				//note: texture positions inverted
				//set vertex clock-wise
				//texture positions U shape, clock-wise			
/* //edited by Mike, 20211005
				glBegin(GL_QUADS);
        	glTexCoord2f(tx, ty);
					glVertex3f(x, y, 0);
					
        	glTexCoord2f(tx + 0.078125f, ty);
					glVertex3f(x, y + texth, 0);
	
        	glTexCoord2f(tx + 0.078125f, ty + 0.125f);
					glVertex3f(x + textw, y + texth, 0);
	
        	glTexCoord2f(tx, ty + 0.125f);
					glVertex3f(x + textw, y, 0);      		
				glEnd();
*/
/*
//inverted texture; y-axis
				glBegin(GL_QUADS);
        	glTexCoord2f(tx + 0.078125f, ty + 0.125f);
					glVertex3f(x, y, 0);
					
        	glTexCoord2f(tx + 0.078125f, ty);
					glVertex3f(x, y + texth, 0);
	
        	glTexCoord2f(tx, ty);
					glVertex3f(x + textw, y + texth, 0);
	
        	glTexCoord2f(tx, ty + 0.125f);
					glVertex3f(x + textw, y, 0);      		
				glEnd();
*/
				glBegin(GL_QUADS);
        	glTexCoord2f(tx, ty);
					glVertex3f(x, y, 0);

        	glTexCoord2f(tx, ty + 0.125f);					
					glVertex3f(x, y + texth, 0);

        	glTexCoord2f(tx + 0.078125f, ty + 0.125f);	
					glVertex3f(x + textw, y + texth, 0);

        	glTexCoord2f(tx + 0.078125f, ty);	
					glVertex3f(x + textw, y, 0);      		
				glEnd();

			//added by Mike, 20211004
			//reset
//			glTranslatef(0.0f,texth/2,0.0f);			
			glScalef(1.0f, 1.0f, 1.0f);
			glTranslatef(-x,-y,0.0f);			
			
	glDisable(GL_TEXTURE_2D);
	
	//reset; removed by Mike, 20211008
//	glColor3f(1.0f,1.0f,1.0f); //white
}

void Font::draw_charRedSquareOK(GLuint glIFontTexture, float x, float y, float z, char c)
{
		//added by Mike, 20211005
	glLoadIdentity();
	
/* //removed by Mike, 20211004
	glBindTexture(GL_TEXTURE_2D, openGLITexture); //textureId);
	glEnable(GL_TEXTURE_2D);
*/
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0f,0.0f,0.0f); //red	


    x=myUsbongUtils->autoConvertFromPixelToVertexPointX(x);
    y=myUsbongUtils->autoConvertFromPixelToVertexPointY(y);
       
    
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

			x=0;
			y=0;

			
			//added by Mike, 20211004
//			glTranslatef(x,y,0.0f);			

			//edited by Mike, 20211004
//			glScalef(0.1f, 0.1f, 0.0f);
		//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
		//use correct width x height ratio; window 10x18; row x column
		glScalef(0.20f, 0.35f, 0.0f);



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


void Font::draw_charBuggy(GLuint glIFontTexture, float x, float y, float z, char c)
{
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, glIFontTexture);
	glEnable(GL_TEXTURE_2D);
/*
  glDisable(GL_TEXTURE_2D);	
*/
	//removed by Mike, 20210903
//	textw=textw*2;

    GLfloat tx, ty, tz;

    // check if the character is valid
    if (c < ' ' || c > '~')
        return;

    //subtract 32, since the first character in the font texture
    //is the space (ascii value 32)
    c = c - 32;

    //each character in the font texture image file
    //has a width-height ratio of 10:16
    tx = c % 12 * 0.078125f;
    
//    printf(">>c: %i\n",c);
    
    //edited by Mike, 20210905
//    ty = 0.875f - (c / 12 * 0.125f);
//    ty = 1.0f - (c / 7 * 0.125f);
//    ty = 0.875f - (c / 12 * 0.125f);
    ty = (c / 12 * 0.125f);

//    printf(">>ty: %f\n",ty);

/* //edited by Mike, 20211005    
    //added by Mike, 20210904
    //note: each character in the font texture image file has a width-height ratio of 10:16
    float textw = 20*1.2f; //20*1.5f; //20*2; //20; //0.078125f;
    float texth = 32*1.2f; //32*1.5f; //32*2; //32; //0.125f;
*/    
    //added by Mike, 20210903
    float textw = fMyWindowWidth/1.5f;
    float texth = fMyWindowHeight/1.5f;    
    
//    printf("tx: %f; ty: %f\n",tx,ty);

/* //removed by Mike, 20210903    
    //added by Mike, 20210903
    //note: each character in the font texture image file has a width-height ratio of 10:16
    float fTextWidth = 0.078125f;
    float fTextHeight = 0.125f;
*/


	x=0;
	y=0;

	//added by Mike, 20211005	
	x=myUsbongUtils->autoConvertFromPixelToVertexPointX(x);
  y=myUsbongUtils->autoConvertFromPixelToVertexPointY(y);  
  textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(textw);
  texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(texth);	


	//added by Mike, 20211004
//	glTranslatef(x,y,0.0f);			

	//add scale COMMAND after translate COMMAND for auto-computed positions to be correct
	//use correct width x height ratio; window 10x18; row x column
//	glScalef(0.20f, 0.35f, 0.0f);


glColor3f(1.0f,0.0f,0.0f); //red	

    printf("x: %f; y: %f\n",x,y);
    printf("textw: %f; texth: %f\n",textw,texth);

			//added by Mike, 20211005
			glTranslatef(x,y,0.0f);			

   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glTexCoord2f(tx, ty);
        glVertex3f(x, y, 0.0f);

        glTexCoord2f(tx + 0.078125f, ty);
      	glVertex3f(x+textw, y, 0.0f);      

        glTexCoord2f(tx + 0.078125f, ty + 0.125f);
      	glVertex3f(x+textw, y+texth, 0.0f);              

        glTexCoord2f(tx, ty + 0.125f);
      	glVertex3f(x, y+texth, 0.0f);      
   glEnd();    
   		
   		//reset
   		glColor3f(1.0f,1.0f,1.0f); //white

	
	glDisable(GL_TEXTURE_2D);
}

//added by Mike, 20210903
//TO-DO: -reverify: this due to font char NOT drawn
void Font::draw_charWith2DLevelOK(GLuint glIFontTexture, float x, float y, float z, char c)
{
	glBindTexture(GL_TEXTURE_2D, glIFontTexture);
	glEnable(GL_TEXTURE_2D);

	//removed by Mike, 20210903
//	textw=textw*2;

    GLfloat tx, ty, tz;

    // check if the character is valid
    if (c < ' ' || c > '~')
        return;

    //subtract 32, since the first character in the font texture
    //is the space (ascii value 32)
    c = c - 32;

    //each character in the font texture image file
    //has a width-height ratio of 10:16
    tx = c % 12 * 0.078125f;
    
//    printf(">>c: %i\n",c);
    
    //edited by Mike, 20210905
//    ty = 0.875f - (c / 12 * 0.125f);
//    ty = 1.0f - (c / 7 * 0.125f);
//    ty = 0.875f - (c / 12 * 0.125f);
    ty = (c / 12 * 0.125f);

//    printf(">>ty: %f\n",ty);

    
    //added by Mike, 20210904
    //note: each character in the font texture image file has a width-height ratio of 10:16
    float textw = 20*1.2f; //20*1.5f; //20*2; //20; //0.078125f;
    float texth = 32*1.2f; //32*1.5f; //32*2; //32; //0.125f;
    
//    printf("tx: %f; ty: %f\n",tx,ty);

/* //removed by Mike, 20210903    
    //added by Mike, 20210903
    //note: each character in the font texture image file has a width-height ratio of 10:16
    float fTextWidth = 0.078125f;
    float fTextHeight = 0.125f;

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, y, 0);
			
			glTexCoord2f(0.0f+0.5f, 0.0f);
			glVertex3f(x + fTextWidth, y, 0);
			
			glTexCoord2f(0.0f+0.5f, 0.0f+0.5f);
			glVertex3f(x + fTextWidth, y + fTextHeight, 0);
			
			glTexCoord2f(0.0f, 0.0f+0.5f);
			glVertex3f(x, y + fTextHeight, 0);
		glEnd();
*/		
/* //edited by Mike, 20210904		
		glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glTexCoord2f(tx, ty);
        glVertex3f(x, y, 0.0f);

        glTexCoord2f(tx + 0.078125f, ty);
      	glVertex3f(x+0.1f, y, 0.0f);      

        glTexCoord2f(tx + 0.078125f, ty + 0.125f);
      	glVertex3f(x+0.1f, y+0.16f, 0.0f);              

				glTexCoord2f(tx, ty + 0.125f);
      	glVertex3f(x, y+0.16f, 0.0f);      
   glEnd();    
*/
   //added by Mike, 20210905
//    x=x+(30.0f*2);
    
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glTexCoord2f(tx, ty);
        glVertex3f(x, y, 0.0f);

        glTexCoord2f(tx + 0.078125f, ty);
      	glVertex3f(x+textw, y, 0.0f);      

        glTexCoord2f(tx + 0.078125f, ty + 0.125f);
      	glVertex3f(x+textw, y+texth, 0.0f);              

        glTexCoord2f(tx, ty + 0.125f);
      	glVertex3f(x, y+texth, 0.0f);      
   glEnd();    
   		
	
	glDisable(GL_TEXTURE_2D);
}

//edited by Mike, 20210805
//TO-DO: -update: to be tool for Unit member to set font size, font spacing, et cetera?
//void draw_string(GLfloat x, GLfloat y, char *string)
void Font::draw_stringBuggy(GLuint glIFontTexture, GLfloat x, GLfloat y, GLfloat z, char *string)
{
    
//    printf(">>string: %s\n",string);
//    printf(">>string's c: %c\n",string[0]);

    GLfloat origX=x;
    
    //added by Mike, 20210905
    int iStringCharCount=0;
    
    //edited by Mike, 20210905
    while (string[0] != 0)
//    while (string[iStringCharCount] != 0)
    {
//        printf(">>string's c: %c\n",string[0]);


/* //removed by Mike, 20210904
		//TO-DO: -update: this
        //added by Mike, Feb14,2007
		if (string[0]=='\n') {
			y -= 0.1f;//15.0f;
			x=origX-0.1f;//-10.0f;			
    }
*/            
        glPushMatrix();
        	//removed by Mike, 20201010
            //make font larger, added by Mike, Feb28,2007
//            glScalef(2.0f, 2.0f, 2.0f);//1.5f, 1.5f, 1.5f);

//            glScalef(0.5f, 0.5f, 0.5f);
				//edited by Mike, 2020117
//            draw_char(x, y, string[0]);
//            draw_char(glIFontTexture, x, y, z, string[iStringCharCount]);
        draw_char(glIFontTexture, x, y, z, string[0]);

        glPopMatrix();

        
        /* advance 10 pixels after each character */
//TO-DO: -update: this
				//edited by Mike, 20211005
//        x += 20.0f; //(20.0f*1.2f); //(20.0f*1.5f); //40.0f;
//	        x += 0.1f;
	        x += 0.03f;

        /* go to the next character in the string */
        string++;
//        iStringCharCount++;
    }
}

//edited by Mike, 20211008
//auto-draws arrow, e.g. right-most of textbox's background, only at select times
//cause by myXPos, et cetera NOT yet set in memory to have value
void Font::draw_stringOKDrawsRedTriangle(GLuint glIFontTexture, GLfloat xInput, GLfloat yInput, GLfloat zInput, char *string)
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
//edited by Mike, 20211008
//	y=0+windowHeight-texth/4; //-texth/2;
//    y=0+windowHeight-texth/2; //-texth/4;
y=0+windowHeight-texth; //-texth/4;

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

//edited by Mike, 20211008
void Font::draw_stringCharOutputOK(GLuint glIFontTexture, GLfloat xInput, GLfloat yInput, GLfloat zInput, char *string)
{    
    //added by Mike, 20211005
    glLoadIdentity();
    
    //added by Mike, 20211009
    glBindTexture(GL_TEXTURE_2D, glIFontTexture);
    glEnable(GL_TEXTURE_2D);

/*
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f,0.0f,0.0f); //red
 */
    
    float x=myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos);
    float y=myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos);
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);
    
    float windowWidth=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMyWindowWidth);
    float windowHeight=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMyWindowHeight);
    
    //bottom-center
    x=x+windowWidth/2+textw/2;
    //note: /4, instead of 2 due to scale again by 0.5f
    //edited by Mike, 20211008
    //	y=0+windowHeight-texth/4; //-texth/2;
    //    y=0+windowHeight-texth/2; //-texth/4;
    y=0+windowHeight-texth; //-texth/4;
    
    //added by Mike, 20211004
    glTranslatef(x,y,0.0f);
    
    //add scale COMMAND after translate COMMAND for auto-computed positions to be correct
    //use correct width x height ratio; window 10x18; row x column
    glScalef(0.20f, 0.35f, 0.0f);
    glScalef(0.5f, 0.5f, 0.0f); //added by Mike, 20211005
    
    x=0;
    y=0;
  
/* //edited by Mike, 20211009
    //counter-clockwise sequence to auto-draw front face
    //note: origin TOP-LEFT
    glBegin(GL_TRIANGLES);
    glVertex3f(x-textw,y,0.0f); //LEFT vertex
    glVertex3f(x,y+texth,0.0f); //BOTTOM-CENTER vertex
    glVertex3f(x+textw,y,0.0f); //RIGHT vertex
    glEnd();
*/
/*
    //red square OK
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glVertex3f(x, y, 0.0f);
      glVertex3f(x+textw, y, 0.0f);
      glVertex3f(x+textw, y+texth, 0.0f);
      glVertex3f(x, y+texth, 0.0f);
    glEnd();
*/
    
    char c='A';
    
    GLfloat tx, ty, tz;
    
    // check if the character is valid
    if (c < ' ' || c > '~')
        return;
    
    //subtract 32, since the first character in the font texture
    //is the space (ascii value 32)
    c = c - 32;
    
    //each character in the font texture image file
    //has a width-height ratio of 10:16
    tx = c % 12 * 0.078125f;
    
    //    printf(">>c: %i\n",c);
    
    ty = (c / 12 * 0.125f);
    
    //    printf(">>ty: %f\n",ty);
    
  
/*
    //added by Mike, 20210904
    //note: each character in the font texture image file has a width-height ratio of 10:16
    textw = 20*1.2f; //20*1.5f; //20*2; //20; //0.078125f;
    texth = 32*1.2f; //32*1.5f; //32*2; //32; //0.125f;
*/
    
    
    //red square OK
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
    glTexCoord2f(tx, ty);
    glVertex3f(x, y, 0.0f);
    
    glTexCoord2f(tx + 0.078125f, ty);
    glVertex3f(x+textw, y, 0.0f);
    
    glTexCoord2f(tx + 0.078125f, ty + 0.125f);
    glVertex3f(x+textw, y+texth, 0.0f);
    
    glTexCoord2f(tx, ty + 0.125f);
    glVertex3f(x, y+texth, 0.0f);
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

//added by Mike, 20211009
void Font::draw_string(GLuint glIFontTexture, GLfloat xInput, GLfloat yInput, GLfloat zInput, char *string)
{
    //added by Mike, 20211005
    glLoadIdentity();
    
    //added by Mike, 20211009
    glBindTexture(GL_TEXTURE_2D, glIFontTexture);
    glEnable(GL_TEXTURE_2D);
    
    /*
     glDisable(GL_TEXTURE_2D);
     glColor3f(1.0f,0.0f,0.0f); //red
     */
    
    float x=myUsbongUtils->autoConvertFromPixelToVertexPointX(myXPos);
    float y=myUsbongUtils->autoConvertFromPixelToVertexPointY(myYPos);
    float textw=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(myWidth);
    float texth=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(myHeight);
    
    float windowWidth=myUsbongUtils->autoConvertFromPixelToVertexGridTileWidth(fMyWindowWidth);
    float windowHeight=myUsbongUtils->autoConvertFromPixelToVertexGridTileHeight(fMyWindowHeight);
    
    //bottom-center
    x=x+windowWidth/2+textw/2;
    //note: /4, instead of 2 due to scale again by 0.5f
    //edited by Mike, 20211008
    //	y=0+windowHeight-texth/4; //-texth/2;
    //    y=0+windowHeight-texth/2; //-texth/4;
    y=0+windowHeight-texth; //-texth/4;
    
    //added by Mike, 20211004
    glTranslatef(x,y,0.0f);
    
    //add scale COMMAND after translate COMMAND for auto-computed positions to be correct
    //use correct width x height ratio; window 10x18; row x column
    glScalef(0.20f, 0.35f, 0.0f);
    glScalef(0.5f, 0.5f, 0.0f); //added by Mike, 20211005
    
    x=0;
    y=0;
    
    /* //edited by Mike, 20211009
     //counter-clockwise sequence to auto-draw front face
     //note: origin TOP-LEFT
     glBegin(GL_TRIANGLES);
     glVertex3f(x-textw,y,0.0f); //LEFT vertex
     glVertex3f(x,y+texth,0.0f); //BOTTOM-CENTER vertex
     glVertex3f(x+textw,y,0.0f); //RIGHT vertex
     glEnd();
     */
    /*
     //red square OK
     glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
     glVertex3f(x, y, 0.0f);
     glVertex3f(x+textw, y, 0.0f);
     glVertex3f(x+textw, y+texth, 0.0f);
     glVertex3f(x, y+texth, 0.0f);
     glEnd();
     */
    
    char c='A';
    
    GLfloat tx, ty, tz;
    
    // check if the character is valid
    if (c < ' ' || c > '~')
        return;
    
    //subtract 32, since the first character in the font texture
    //is the space (ascii value 32)
    c = c - 32;
    
    //each character in the font texture image file
    //has a width-height ratio of 10:16
    tx = c % 12 * 0.078125f;
    
    //    printf(">>c: %i\n",c);
    
    ty = (c / 12 * 0.125f);
    
    //    printf(">>ty: %f\n",ty);
    
    
    /*
     //added by Mike, 20210904
     //note: each character in the font texture image file has a width-height ratio of 10:16
     textw = 20*1.2f; //20*1.5f; //20*2; //20; //0.078125f;
     texth = 32*1.2f; //32*1.5f; //32*2; //32; //0.125f;
     */
    
    
    //red square OK
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
    glTexCoord2f(tx, ty);
    glVertex3f(x, y, 0.0f);
    
    glTexCoord2f(tx + 0.078125f, ty);
    glVertex3f(x+textw, y, 0.0f);
    
    glTexCoord2f(tx + 0.078125f, ty + 0.125f);
    glVertex3f(x+textw, y+texth, 0.0f);
    
    glTexCoord2f(tx, ty + 0.125f);
    glVertex3f(x, y+texth, 0.0f);
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


//added by Mike, 20210903
GLuint Font::openGLLoadTexture(char *filename, float fMyWidth, float fMyHeight)
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
//added by Mike, 20210905; added again by Mike, 20210906
#elif defined(_WIN32)
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
    
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    
/* //edited by Mike, 20210830
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
/*	//edited by Mike, 20210722; this is due to displayed image is blurred
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
     GL_LINEAR_MIPMAP_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // unselect texture myFontTextureObject
//    glBindTexture(GL_TEXTURE_2D, 0);
    
    // setup alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w,
	surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	
	SDL_FreeSurface(surface);
	
	return texture;	
}

//added by Mike, 20210903
GLuint Font::setupFont(char *filename, float fMyWidth, float fMyHeight)
{
	return openGLLoadTexture(filename, fMyWidth, fMyHeight);
}
