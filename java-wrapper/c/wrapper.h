/**
 * The MIT License (MIT)
 * <p/>
 * Copyright (c) 2015 Bertrand Martel
 * <p/>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p/>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 /*	wrapper.h

	api for plot sdl input parameters 
	
	@author Bertrand Martel
	@version 0.1
*/
#ifndef WRAPPER
#define WRAPPER

#include "plotsdl/llist.h"
#include "plotsdl/plot.h"

/**
 * @brief pushCoordinate
 *      push a new item to the end of coordinate table
 * @param caption_id
 *      caption id used to identify for which caption this point refers to
 * @param coord_x
 *      x coordinate
 * @param coord_y
 *      y coordinate
 */
void pushCoordinate(int caption_id,int coord_x,int coord_y);

/**
 * @brief pushCaption
 *      push a new item to the end of caption table
 * @param caption_text
 *      caption text
 * @param caption_id
 *      caption id
 * @param color
 *      caption color
 */
void pushCaption(char * caption_text,int caption_id,int color);

/**
 * @brief pushPlotParams
 *      push plot parameters
 * @param screen_width
 *      caption width
 * @param screen_heigth
 *      screen height
 * @param title
 *      SDL window title
 * @param fontFileName
 *      font file path
 * @param textSize
 *      text size
 * @param xcaptionText
 *      caption label on x axis
 * @param ycaptionText
 *      caption label on y axis
  * @param scale_x
 *      scale value on x axis
  * @param scale_y
 *      scale value on y axis
  * @param max_x
 *      max value on x axis
  * @param max_y
 *      max value on y axis
 */
void pushPlotParams(int screen_width,
	int screen_heigth,
	char * title,
	char * fontFileName,
	int textSize,
	char * xcaptionText,
	char * ycaptionText,
	float scale_x,
	float scale_y,
	float max_x,
	float max_y);

/**
* plot graph
*/
int plotsdlgraph();

#endif /*WRAPPER*/