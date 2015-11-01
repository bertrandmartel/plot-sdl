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
	plot.h

	create a new SDL window, plot points in a created graph featuring a cartesian coordinate system

	@author Bertrand Martel
	@version 0.1
*/
#ifndef PLOT
#define PLOT

#ifdef __ANDROID__
#	include "SDL.h"
#	include "SDL_ttf.h"
#	define APP_NAME "plot_sdl_impl"
#else
#	include "SDL2/SDL.h"
#	include "SDL2/SDL_ttf.h"
#endif

#include "llist.h"

#define DOT_RADIUS                    6
#define CAPTION_OFFSET_CIRCLE_TO_TEXT 10
#define CAPTION_OFFSET_DELIMITER      40
#define GRADUATION_HEIGTH             10
#define CAPTION_Y_LABEL_OFFSET        10
#define CAPTION_MARGIN                30

//-------------Plot parameter structure  ------------
typedef struct plot_params_struct{

	int         screen_width      ;
	int         screen_heigth     ;
	char *      plot_window_title ;
	char *      font_text_path    ;
	int         font_text_size    ;
	char *      caption_text_x    ;
	char *      caption_text_y    ;
	captionlist caption_list      ;
	coordlist   coordinate_list   ;
	float       scale_x           ;
	float       scale_y           ;
	float       max_x             ;
	float       max_y             ;

} plot_params;

//-------------Plot SDL objects structure  ------------
typedef struct plot_struct{

	TTF_Font     *font                 ;
	SDL_Window   *screen               ;
	SDL_Surface  *plot_surface         ;
	SDL_Surface  *plot_mask_surface    ;
	SDL_Surface  *caption_surface      ;
	SDL_Surface  *caption_mask_surface ;
	SDL_Surface  *captionX             ;
	SDL_Surface  *captionY             ;
	SDL_Texture  *textureX             ;
	SDL_Texture  *textureY             ;
	SDL_Renderer *renderer             ;

} splot;

/**
 * @brief plot_graph
 *      call draw_plot function, when exit event received clean everything
 * @param params
 *      plot parameters (cf plot_params struct)
 */
int plot_graph(plot_params *params);

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
	int plot_position_y);

/**
 * @brief draw_points
 *      draw each coordinate points on the graph featuring cartesian coordinate system
 * @param renderers
 *      SDL renderer
 * @param caption_item
 *      caption item structure
 * @param params
 *      plot parameters (cf plot_params struct)
 * @param plot_width
 *      plot base width (with proportion to screen width)
 * @param plot_heigth
 *      plot bas heigth (with proportion to screen heigth)
 * @param plot_mask_position
 *      SDL rectangle giving position of plot base (x,y) from max x or max y due to stroke width
 */
void draw_points(SDL_Renderer* renderer,caption_item* caption_item,plot_params *params,float plot_width,float plot_heigth,SDL_Rect plot_mask_position);

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
void draw_plot(splot *plot, plot_params *params, surfacelist *surface_list);

/**
 * @brief wait_for_sdl_event
 *      wait for an event
 */
void wait_for_sdl_event();

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
void clean_plot(splot *plot, plot_params *params, surfacelist *surface_list);

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
void draw_circle(SDL_Renderer *renderer, int n_cx, int n_cy, int radius);

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
void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius);

#endif