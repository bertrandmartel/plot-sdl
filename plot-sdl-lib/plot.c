/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/**
	plot.c

	create a new SDL window, plot points in a created graph featuring a cartesian coordinate system

	@author Bertrand Martel
	@version 0.1
*/
#include "stdio.h"
#include "plotsdl/plot.h"

#ifdef __ANDROID__
#	include "SDL.h"
#	include "SDL_ttf.h"
#	include "android/log.h"
#else
#	include "SDL2/SDL.h"
#	include "SDL2/SDL_ttf.h"
#endif

#include "plotsdl/llist.h"
#include "math.h"

/**
 * @brief plot_graph
 *      create a new SDL window and plot grap with given parameters
 * @param params
 *      plot parameters (cf plot_params struct)
 */
int plot_graph(plot_params *params)
{

	setvbuf (stdout, NULL, _IONBF, 0);

	surfacelist surface_list = NULL;

	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Error SDL TTF_Init error : %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	if (SDL_Init(SDL_INIT_VIDEO)==-1){

		fprintf(stderr, "Error SDL init failure : %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	splot plot;
	plot.font                 = NULL;
	plot.screen               = NULL;
	plot.plot_surface         = NULL;
	plot.plot_mask_surface    = NULL;
	plot.caption_surface      = NULL;
	plot.caption_mask_surface = NULL;
	plot.captionX             = NULL;
	plot.captionY             = NULL;
	plot.textureX             = NULL;
	plot.textureY             = NULL;
	plot.renderer             = NULL;

	//font specs
	plot.font = TTF_OpenFont(params->font_text_path, params->font_text_size);

	if(plot.font==NULL)
	{
		printf("Error font file read failure, check your font file\n");
		clean_plot(&plot,params,&surface_list);

		return EXIT_FAILURE;
	}

	plot.screen = SDL_CreateWindow(
		params->plot_window_title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		params->screen_width, 
		params->screen_heigth,
		SDL_WINDOW_SHOWN);

	//SDL_SetWindowFullscreen(plot.screen,SDL_WINDOW_FULLSCREEN);

	draw_plot(&plot,params,&surface_list);

	clean_plot(&plot,params,&surface_list);

	return EXIT_SUCCESS;
}		

/**
 * @brief clean_plot
 *      full clean of SDL pointers and linked list clear
 * @param splot
 *      plot containing SDL items
 * @param params
 *      plot parameters (cf plot_params struct)
  * @param surface_list
 *      list of surfaces stored to be freed later
 */
void clean_plot(splot *plot, plot_params *params, surfacelist *surface_list)
{
	SDL_DestroyTexture(plot->textureX);
	SDL_DestroyTexture(plot->textureY);
	SDL_FreeSurface   (plot->plot_mask_surface);
	SDL_FreeSurface   (plot->plot_surface);
	SDL_FreeSurface   (plot->caption_mask_surface);
	SDL_FreeSurface   (plot->caption_surface);
	SDL_FreeSurface   (plot->captionX);
	SDL_FreeSurface   (plot->captionY);

	params->caption_list    = clear_caption(params->caption_list);
	params->coordinate_list = clear_coord(params->coordinate_list);

	*surface_list=clear_surface(*surface_list);

	SDL_DestroyRenderer(plot->renderer);
	SDL_DestroyWindow  (plot->screen);

	SDL_Quit();
	TTF_CloseFont(plot->font);
	TTF_Quit();
}

/**
 * @brief draw_plot
 *      create a new SDL window and plot grap with given parameters
 * @param splot
 *      plot containing SDL objects
 * @param params
 *      plot parameters (cf plot_params struct)
  * @param surface_list
 *      list of surfaces stored to be freed later
 */
void draw_plot(splot *plot, plot_params *params, surfacelist *surface_list)
{
	plot->renderer = SDL_CreateRenderer(plot->screen, 0, 0);

	if (plot->screen != NULL) {

		int stroke_width=2;

		SDL_Color font_color = {0, 0, 0};
		plot->captionX = TTF_RenderText_Blended(plot->font, params->caption_text_x, font_color);
		plot->captionY = TTF_RenderText_Blended(plot->font, params->caption_text_y, font_color);

		//------------ background-----------------------
		SDL_SetRenderDrawColor(plot->renderer,255,255,255,255);
		SDL_Rect screen;
		screen.x=0;
		screen.y=0;
		screen.w=params->screen_width;
		screen.h=params->screen_heigth;

		SDL_RenderFillRect( plot->renderer, &screen );
		//---------------------------------------------

		float plot_width=params->screen_width*0.8;
		float plot_heigth=params->screen_heigth*0.8;
		float plot_caption_heigth=params->screen_heigth*0.05;

		SDL_Rect plot_position;
		plot_position.x=(params->screen_width/2)-(plot_width*0.47);
		plot_position.y=(params->screen_heigth*0.50)-(plot_heigth/2);
		plot_position.w=plot_width;
		plot_position.h=plot_heigth;

		SDL_Rect plot_mask_position;
		plot_mask_position.x=plot_position.x-stroke_width;
		plot_mask_position.y=plot_position.y-stroke_width;
		plot_mask_position.w=plot_width+stroke_width*2;
		plot_mask_position.h=plot_heigth+stroke_width*2;

		SDL_Rect plot_caption_position;
		plot_caption_position.x=plot_position.x;
		plot_caption_position.y=plot_position.y-20-plot_caption_heigth;
		plot_caption_position.w=plot_width;
		plot_caption_position.h=plot_caption_heigth;

		SDL_Rect plot_caption_mask_position;
		plot_caption_mask_position.x=plot_caption_position.x-stroke_width;
		plot_caption_mask_position.y=plot_caption_position.y-stroke_width;
		plot_caption_mask_position.w=plot_width+stroke_width*2;
		plot_caption_mask_position.h=plot_caption_heigth+stroke_width*2;

		SDL_SetRenderDrawColor(plot->renderer,0, 0, 0,255);
		SDL_RenderFillRect(plot->renderer,&plot_mask_position);

		SDL_SetRenderDrawColor(plot->renderer,255, 255, 255,255);
		SDL_RenderFillRect(plot->renderer,&plot_position);

		SDL_SetRenderDrawColor(plot->renderer,0, 0, 0,255);
		SDL_RenderFillRect(plot->renderer,&plot_caption_mask_position);

		SDL_SetRenderDrawColor(plot->renderer,255, 255, 255,255);
		SDL_RenderFillRect(plot->renderer,&plot_caption_position);


		SDL_Rect caption_y_position;
		caption_y_position.x=plot_position.x;
		caption_y_position.y=plot_position.y;

		draw_scale_graduation(plot->renderer,
			params,
			plot,
			plot_width,
			plot_heigth,
			plot_mask_position,
			plot->font,
			font_color,
			surface_list,
			plot_position.x,
			plot_position.y);

		if (params->caption_list!=NULL)
		{
			caption_item *tmp = params->caption_list;

			if (tmp!=NULL)
			{
				int caption_offset=CAPTION_MARGIN;

				while(tmp != NULL)
				{
					//plot cercle1
					int circle_x1=plot_caption_mask_position.x+caption_offset;
					int circle_y1=plot_caption_mask_position.y+plot_caption_heigth/2+stroke_width;

					SDL_SetRenderDrawColor(plot->renderer,0,0,0,255);
					fill_circle(plot->renderer,circle_x1,circle_y1,DOT_RADIUS);

					SDL_SetRenderDrawColor(plot->renderer,(tmp->caption_color & 0xFF0000)>>16, 
						(tmp->caption_color & 0x00FF00)>>8,
						tmp->caption_color & 0x0000FF,255);

					fill_circle(plot->renderer,circle_x1,circle_y1,DOT_RADIUS-2);

					//plot cercle2
					caption_offset+=40;

					int circle_x2=plot_caption_mask_position.x+caption_offset;
					int circle_y2=circle_y1;

					SDL_SetRenderDrawColor(plot->renderer,0,0,0,255);
					fill_circle(plot->renderer,circle_x2,circle_y2,DOT_RADIUS);

					SDL_SetRenderDrawColor(plot->renderer,(tmp->caption_color & 0xFF0000)>>16, 
						(tmp->caption_color & 0x00FF00)>>8,
						tmp->caption_color & 0x0000FF,255);

					fill_circle(plot->renderer,circle_x2,circle_y2,DOT_RADIUS-2);

					//draw line between two circles
					SDL_RenderDrawLine(plot->renderer,circle_x1+DOT_RADIUS+1,circle_y1,circle_x2-DOT_RADIUS-1,circle_y2);

					//display caption
					SDL_Surface *caption_text_surface = TTF_RenderText_Blended(plot->font, tmp->caption_txt, font_color);
					SDL_Rect caption_text;
					SDL_Texture * texture_text = SDL_CreateTextureFromSurface(plot->renderer, caption_text_surface);
					SDL_QueryTexture(texture_text, NULL, NULL, &caption_text.w, &caption_text.h);
					caption_text.x=circle_x2+DOT_RADIUS+CAPTION_OFFSET_CIRCLE_TO_TEXT;
					caption_text.y=circle_y2-caption_text.h/2;
					SDL_RenderCopy(plot->renderer, texture_text, NULL, &caption_text);

					*surface_list=push_back_surface(*surface_list,caption_text_surface);

					caption_offset+=caption_text.w+DOT_RADIUS+CAPTION_OFFSET_CIRCLE_TO_TEXT+CAPTION_OFFSET_DELIMITER;

					draw_points(plot->renderer,
						tmp,
						params,
						plot_width,
						plot_heigth,
						plot_mask_position);

					tmp = tmp->nxt;
				}
			}
		}

		SDL_RenderPresent(plot->renderer);

		#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_VERBOSE,APP_NAME,"plot finished");
		#endif

		wait_for_sdl_event();
	}
	else{
		fprintf(stderr, "Error cant allocate memory for screen : %s\n", SDL_GetError());
	}
}

/**
 * @brief draw_points
 *      draw each coordinate points on the graph featuring cartesian coordinate system
 * @param renderers
 *      SDL renderer object
 * @param caption_item
 *      caption item object structure
 * @param params
 *      plot parameters (cf plot_params struct)
 * @param plot_width
 *      plot base width (with proportion to screen width)
 * @param plot_heigth
 *      plot bas heigth (with proportion to screen heigth)
 * @param plot_mask_position
 *      SDL rectangle giving position of plot base (x,y) from max x or max y due to stroke width
 */
void draw_points(
	SDL_Renderer* renderer,
	caption_item* caption_item,
	plot_params *params,
	float plot_width,
	float plot_heigth,
	SDL_Rect plot_mask_position)
{
	coordinate_item* tmp=params->coordinate_list;

	float scale_x_num=plot_width/(params->max_x/params->scale_x);
	float scale_y_num=plot_heigth/(params->max_y/params->scale_y);

	unsigned char isFirst=1;

	float previous_x=0;
	float previous_y=0;

	while (tmp!=NULL)
	{
		if (tmp->caption_id==caption_item->caption_id)
		{
			float circle_x1=plot_mask_position.x+1+(tmp->x/params->scale_x)*scale_x_num;
			float circle_y1=plot_mask_position.y+plot_heigth-(tmp->y/params->scale_y)*scale_y_num;

			SDL_SetRenderDrawColor(renderer,0,0,0,255);

			fill_circle(renderer,circle_x1,circle_y1,DOT_RADIUS);

			SDL_SetRenderDrawColor(renderer,(caption_item->caption_color & 0xFF0000)>>16, 
				(caption_item->caption_color & 0x00FF00)>>8,
				caption_item->caption_color & 0x0000FF,255);

			fill_circle(renderer,circle_x1,circle_y1,DOT_RADIUS-2);

			if (!isFirst)
			{
				SDL_RenderDrawLine(renderer,previous_x,previous_y,circle_x1,circle_y1);
			}

			previous_x=circle_x1;
			previous_y=circle_y1;

			if (isFirst)
			{
				isFirst=0;
			}
		}
		tmp=tmp->nxt;
	}
}

/**
 * @brief draw_scale_graduation
 *      draw graduation for the graph
 * @param renderer
 *      SDL renderer object
 * @param params
 *      plot parameters (cf plot_params struct)
 * @param plot
 *      structure containing textures and surfaces
 * @param plot_width
 *      plot base width (with proportion to screen width)
 * @param plot_heigth
 *      plot bas heigth (with proportion to screen heigth)
 * @param plot_mask_position
 *      SDL rectangle giving position of plot base (x,y) from max x or max y due to stroke width
 * @param font
 *      SDL font used for captions
 * @param font_color
 *      font color to be used
 * @param surface_list
 *      list of surfaces stored to be freed later
 * @param plot_position_x
 *      x position of plot
 * @param plot_position_y
 *      y position of plot
 */
void draw_scale_graduation(SDL_Renderer * renderer,
	plot_params *params,
	splot *plot,
	float plot_width,
	float plot_heigth,
	SDL_Rect plot_mask_position,
	TTF_Font *font,
	SDL_Color font_color,
	surfacelist *surface_list,
	int plot_position_x,
	int plot_position_y){

	int scale_x_num=plot_width/(params->max_x/params->scale_x);
	int scale_y_num=plot_heigth/(params->max_y/params->scale_y);

	int init_pos_x=plot_mask_position.x+1;
	int init_pos_y=plot_mask_position.y+plot_heigth+1;

	int current_scale=0;

	int point_number_x=(params->max_x/params->scale_x);

	int i = 0;

	int regular_caption_text_heigth=0;
	int regular_caption_text_width=0;

	for (i = 0; i< point_number_x+1;i++){

		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderDrawLine(renderer,init_pos_x,init_pos_y,init_pos_x,init_pos_y-GRADUATION_HEIGTH);

		char text[10];
		sprintf(text,"%d",current_scale);

		SDL_Surface *caption_text_surface = TTF_RenderText_Blended(font, text, font_color);
		SDL_Rect caption_text;
		SDL_Texture * texture_text = SDL_CreateTextureFromSurface(renderer, caption_text_surface);
		SDL_QueryTexture(texture_text, NULL, NULL, &caption_text.w, &caption_text.h);
		caption_text.x=init_pos_x-caption_text.w/2;
		caption_text.y=init_pos_y+5;
		SDL_RenderCopy(renderer, texture_text, NULL, &caption_text);

		*surface_list=push_back_surface(*surface_list,caption_text_surface);

		init_pos_x+=scale_x_num;
		current_scale+=params->scale_x;

		regular_caption_text_heigth=caption_text.h;
	}

	current_scale=0;
	init_pos_x=plot_mask_position.x+1;
	init_pos_y=plot_mask_position.y+plot_heigth+2;

	for (i = 0; i< (params->max_y/params->scale_y)+1;i++){

		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderDrawLine(renderer,init_pos_x,init_pos_y,init_pos_x+GRADUATION_HEIGTH,init_pos_y);

		char text[10];
		sprintf(text,"%d",current_scale);

		SDL_Surface *caption_text_surface = TTF_RenderText_Blended(font, text, font_color);
		SDL_Rect caption_text;
		SDL_Texture * texture_text = SDL_CreateTextureFromSurface(renderer, caption_text_surface);
		SDL_QueryTexture(texture_text, NULL, NULL, &caption_text.w, &caption_text.h);
		caption_text.x=init_pos_x-caption_text.w-10;
		caption_text.y=init_pos_y-caption_text.h/2;
		SDL_RenderCopy(renderer, texture_text, NULL, &caption_text);

		*surface_list=push_back_surface(*surface_list,caption_text_surface);

		init_pos_y-=scale_y_num;

		current_scale+=params->scale_y;

		regular_caption_text_width=caption_text.w;
	}

	//caption y
	SDL_Rect text_caption_y;
	plot->textureY = SDL_CreateTextureFromSurface(plot->renderer, plot->captionY);
	SDL_QueryTexture(plot->textureY, NULL, NULL, &text_caption_y.w, &text_caption_y.h);
	text_caption_y.x=-1*regular_caption_text_width;
	text_caption_y.y=plot_mask_position.y+plot_heigth/2+text_caption_y.w/4;

	//rotate caption y
	SDL_Point caption_center={plot_position_x-CAPTION_Y_LABEL_OFFSET,0};
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
	SDL_RenderCopyEx(plot->renderer,plot->textureY,NULL,&text_caption_y,90,&caption_center,flip);

	//caption x
	SDL_Rect text_caption_x;
	plot->textureX = SDL_CreateTextureFromSurface(plot->renderer, plot->captionX);
	SDL_QueryTexture(plot->textureX, NULL, NULL, &text_caption_x.w, &text_caption_x.h);
	text_caption_x.x=params->screen_width/2-text_caption_x.w/2;
	text_caption_x.y=plot_position_y+plot_heigth+1*regular_caption_text_heigth;
	SDL_RenderCopy(plot->renderer, plot->textureX, NULL, &text_caption_x);
}

/**
 * @brief wait_for_sdl_event
 *      wait for an event
 */
void wait_for_sdl_event(){

	int go=1;
	SDL_Event event;

	while (go){

		SDL_WaitEvent(&event);

		switch(event.type){
			case SDL_QUIT:
				go=0;
		}
	}
}

/*
 * from : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
 * 
 * This is an implementation of the Midpoint Circle Algorithm 
 * found on Wikipedia at the following link:
 *
 *   http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 * The algorithm elegantly draws a circle quickly, using a
 * set_pixel function for clarity.
 */
void draw_circle(SDL_Renderer *renderer, int n_cx, int n_cy, int radius)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:
 
	double error = (double)-radius;
	double x = (double)radius -0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;
 
	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy + y));
		SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy + x));
 
		if (x != 0)
		{
			SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy + y));
			SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy - x));
		}
 
		if (y != 0)
		{
			SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy - y));
			SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy + x));
		}
 
		if (x != 0 && y != 0)
		{
			SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy - y));
			SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy - x));
		}
 
		error += y;
		++y;
		error += y;
 
		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

/*
 * from : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
 *
 * SDL_Surface 32-bit circle-fill algorithm without using trig
 *
 * While I humbly call this "Celdecea's Method", odds are that the 
 * procedure has already been documented somewhere long ago.  All of
 * the circle-fill examples I came across utilized trig functions or
 * scanning neighbor pixels.  This algorithm identifies the width of
 * a semi-circle at each pixel height and draws a scan-line covering
 * that width.  
 *
 * The code is not optimized but very fast, owing to the fact that it
 * alters pixels in the provided surface directly rather than through
 * function calls.
 *
 * WARNING:  This function does not lock surfaces before altering, so
 * use SDL_LockSurface in any release situation.
 */
void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
	// Note that there is more to altering the bitrate of this 
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html
	static const int BPP = 4;

	double r = (double)radius;

	double dy = 0;

	for (dy = 1; dy <= r; dy += 1.0)
	{
		// This loop is unrolled a bit, only iterating through half of the
		// height of the circle.  The result is used to draw a scan line and
		// its mirror image below it.

		// The following formula has been simplified from our original.  We
		// are using half of the width of the circle because we are provided
		// with a center and we need left/right coordinates.

		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;

		for (; x <= cx + dx; x++)
		{
			SDL_RenderDrawPoint(renderer,x,((int)(cy + r - dy)));
			SDL_RenderDrawPoint(renderer,x,((int)(cy - r + dy)));
		}
	}
}