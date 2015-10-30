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
	linked_list.h

	Build custom linked list, push new element, clear and print items

	@author Bertrand Martel
	@version 0.1
*/
#ifndef LLIST
#define LLIST

#ifdef __ANDROID__
#	include "SDL.h"
#else
#	include "SDL2/SDL.h"
#endif

//--------------CAPTION STRUCT --------------
typedef struct caption_item caption_item;

struct caption_item
{
	char * caption_txt;
	int    caption_id;
	int    caption_color;
	struct caption_item *nxt;
};

typedef caption_item* captionlist;
//----------------------------------------

//-------------COORDINATE STRUCT ------------
typedef struct coordinate_item coordinate_item;

struct coordinate_item
{
	float  x;
	float  y;
	int    caption_id;
	struct coordinate_item *nxt;
};

typedef coordinate_item* coordlist;
//----------------------------------------

//-------------SDL_Surface STRUCT ------------
typedef struct surface_item surface_item;

struct surface_item
{
	SDL_Surface * surface;
	struct surface_item *nxt;
};

typedef surface_item* surfacelist;
//----------------------------------------

/**
 * @brief push_back_coord
 *      push a new item to the end of coordinate table
 * @param list
 *      list of coordinate items
 * @param x
 *      x coordinate
 * @param y
 *      y coordinate
 */
coordlist push_back_coord(coordlist list,int caption_id, float x,float y);

/**
 * @brief print_list_coord
 *      print coordinate table
 * @param list
 *      list of coordinate items
 */
void print_list_coord(coordlist list);

/**
 * @brief clear_coord
 *      clear coordinate table
 * @param list
 *      list of coordinate items
 */
coordlist clear_coord(coordlist list);

/**
 * @brief push_back_caption
 *      push a new item to the end of caption table
 * @param list
 *      list of caption items
 * @param caption_txt
 *      caption text
 * @param caption_id
 *      caption id
 * @param color
 *      caption color
 */
captionlist push_back_caption(captionlist list, char* caption_txt,int caption_id,int color);

/**
 * @brief print_list_caption
 *      print caption table
 * @param list
 *      list of caption items
 */
void print_list_caption(captionlist list);

/**
 * @brief clear_caption
 *      clear caption table
 * @param list
 *      list of caption items
 */
captionlist clear_caption(captionlist list);

/**
 * @brief push_back_surface
 *      push a new item to the end of surface table
 * @param list
 *      list of surface items
 * @param surface
 *      SDL surface ptr
 */
surfacelist push_back_surface(surfacelist list, SDL_Surface* surface);

/**
 * @brief clear_surface
 *      clear surface table
 * @param list
 *      list of surface items
 */
surfacelist clear_surface(surfacelist list);

#endif