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
 * @date updated: 20210903
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

/*****************************************************************************/

GLboolean test_pow2(GLushort i)
{
    while (i % 2 == 0)
        i /= 2;
    if (i == 1)
        return GL_TRUE;
    else
        return GL_FALSE;
}


/*****************************************************************************/

/* text drawing */

//added by Mike, 20210903
//TO-DO: -reverify: this due to font char NOT drawn
void draw_char(GLuint glIFontTexture, float x, float y, float z, char c)
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
    ty = 0.875f - (c / 12 * 0.125f);
    
    //added by Mike, 20210904
    //note: each character in the font texture image file has a width-height ratio of 10:16
    float textw = 20; //0.078125f;
    float texth = 32; //0.125f;
    
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

//edited by Mike, 2020117
//void draw_string(GLfloat x, GLfloat y, char *string)
void draw_string(GLuint glIFontTexture, GLfloat x, GLfloat y, GLfloat z, char *string)
{
    GLfloat origX=x;
    while (string[0] != 0)
    {

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
            draw_char(glIFontTexture, x, y, z, string[0]);
    	glPopMatrix();

        
        /* advance 10 pixels after each character */
//TO-DO: -update: this
        x += 10.0f;
//        x += 0.1f;

        /* go to the next character in the string */
        string++;
    }
}

//added by Mike, 20210903
GLuint openGLLoadTexture(char *filename, float fMyWidth, float fMyHeight)
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
GLuint setupFont(char *filename, float fMyWidth, float fMyHeight)
{
	return openGLLoadTexture(filename, fMyWidth, fMyHeight);
}
