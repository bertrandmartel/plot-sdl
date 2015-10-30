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
/*	main.c

	create a new SDL window, plot points in a created graph featuring a cartesian coordinate system
	
	@author Bertrand Martel
	@version 0.1
*/
#include "SDL.h"

#include "stdio.h"
#include "plotsdl/plot.h"
#include "plotsdl/llist.h"
#include "unistd.h"
#include "wrapper.h"

#ifdef __ANDROID__
#	include "android/log.h"
#	define APP_NAME "plot_sdl"
#endif /* __ANDROID__ */

captionlist caption_list  = NULL;
coordlist coordinate_list = NULL;
plot_params params;

int main(int argc, char* argv[]) {

	#ifdef __ANDROID__
	__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"screen_width  : %d\n",params.screen_width);
	__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"screen_heigth : %d\n",params.screen_heigth);
	#endif /* __ANDROID__ */
	params.caption_list = caption_list;
	params.coordinate_list = coordinate_list;

	int ret = plot_graph(&params);

	/*no need to free linked list, it has already been done in plot_graph()*/
	caption_list = NULL;
	coordinate_list = NULL;

	if (ret == EXIT_FAILURE)
	{

		printf("plot_graph return with status %d\n",ret);
		#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"plot_graph return with status %d\n",ret);
		#endif /* __ANDROID__ */

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
} 

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
void pushCoordinate(int caption_id,int coord_x,int coord_y){
	
	#ifdef __ANDROID__
	__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"push coordinate : %d-%d",coord_x,coord_y);
	#endif /* __ANDROID__ */
	coordinate_list=push_back_coord(coordinate_list,caption_id,coord_x,coord_y);
}

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
void pushCaption(char * caption_text,int caption_id,int color)
{
	#ifdef __ANDROID__
	__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"push caption : %s",caption_text);
	#endif /* __ANDROID__ */
	caption_list=push_back_caption(caption_list,caption_text,caption_id,color);
}

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
	float max_y)
{
	params.screen_width = screen_width;
	params.screen_heigth = screen_heigth;
	params.plot_window_title = title;
	params.font_text_path = fontFileName;
	params.font_text_size = textSize;
	params.caption_text_x = xcaptionText;
	params.caption_text_y = ycaptionText;
	params.scale_x = scale_x;
	params.scale_y = scale_y;
	params.max_x = max_x;
	params.max_y = max_y;
}