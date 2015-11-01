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
/*
	SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
	
	Expose JNI api for plotsdl library

	@author Bertrand Martel
	@version 0.1

*/
#include "wrapper.h"

/*******************************************************************************
				 Functions called by JNI
*******************************************************************************/
#include <jni.h>

/**
 * @brief Java_fr_bmartel_plotsdl_PlotSDLActivity_nativePushCoordinate
 *      push a new item to the end of coordinate table
 * @param env
 *      structure featuring interface to VM
 * @param cls
 *      class caller 
 * @param caption_id
 *      caption id used to identify for which caption this point refers to
 * @param coord_x
 *      x coordinate
 * @param coord_y
 *      y coordinate
 */
void Java_fr_bmartel_plotsdl_PlotSDL_nativePushCoordinate(JNIEnv* env, jclass cls,jint caption_id,jint xcoord,jint ycoord)
{
	pushCoordinate(caption_id,xcoord,ycoord);
}

/**
 * @brief Java_fr_bmartel_plotsdl_PlotSDLActivity_nativePushCaption
 *      push a new item to the end of caption table
 * @param env
 *      structure featuring interface to VM
 * @param cls
 *      class caller 
 * @param caption_txt
 *      caption text
 * @param caption_id
 *      caption id
 * @param caption_color
 *      caption color
 */
void Java_fr_bmartel_plotsdl_PlotSDL_nativePushCaption(JNIEnv* env, jclass cls,jstring caption_txt,jint caption_id,jint caption_color)
{
	const char *nativeString = (*env)->GetStringUTFChars(env, caption_txt, 0);

	pushCaption(strdup(nativeString),caption_id,caption_color);

	(*env)->ReleaseStringUTFChars(env, caption_txt, nativeString);
}

/**
 * @brief Java_fr_bmartel_plotsdl_PlotSDLActivity_nativePushPlotParams
 *      push plot parameters
 * @param env
 *      structure featuring interface to VM
 * @param cls
 *      class caller 
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
void Java_fr_bmartel_plotsdl_PlotSDL_nativePushPlotParams(
	JNIEnv* env, 
	jclass cls,
	jint screen_width,
	jint screen_heigth,
	jstring title,
	jstring fontFileName,
	jint textSize,
	jstring xcaptionText,
	jstring ycaptionText,
	jfloat scale_x,
	jfloat scale_y,
	jfloat max_x,
	jfloat max_y)
{
	const char *titleChar = (*env)->GetStringUTFChars(env, title, 0);
	const char *fontFileChar = (*env)->GetStringUTFChars(env, fontFileName, 0);
	const char *xcaptionChar = (*env)->GetStringUTFChars(env, xcaptionText, 0);
	const char *ycaptionChar = (*env)->GetStringUTFChars(env, ycaptionText, 0);
	
	pushPlotParams(screen_width,
		screen_heigth,
		strdup(titleChar),
		strdup(fontFileChar),
		textSize,
		strdup(xcaptionChar),
		strdup(ycaptionChar),
		scale_x,
		scale_y,
		max_x,
		max_y);

	(*env)->ReleaseStringUTFChars(env, title, titleChar);
	(*env)->ReleaseStringUTFChars(env, fontFileName, fontFileChar);
	(*env)->ReleaseStringUTFChars(env, xcaptionText, xcaptionChar);
	(*env)->ReleaseStringUTFChars(env, ycaptionText, ycaptionChar);
}

/**
 * @brief Java_fr_bmartel_plotsdl_PlotSDLActivity_nativePlotsdlQuit
 *      push a new item to the end of caption table
 * @param env
 *      structure featuring interface to VM
 * @param cls
 *      class caller 
 */
void Java_fr_bmartel_plotsdl_PlotSDL_nativePlotsdlQuit(JNIEnv* env, jclass cls)
{
	SDL_Event user_event;
	user_event.type = SDL_QUIT;
	SDL_PushEvent(&user_event);
}


void Java_fr_bmartel_plotsdl_PlotSDL_nativePlotGraph(JNIEnv *even, jclass cls)
{
	plotsdlgraph();
}