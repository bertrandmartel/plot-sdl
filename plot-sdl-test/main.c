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
	main.c

	testing plot-sdl-lib : create a new SDL window, plot points in a created graph featuring a cartesian coordinate system

	@author Bertrand Martel
	@version 0.1
*/
#include "stdio.h"
#include "plotsdl/plot.h"
#include "plotsdl/llist.h"

int main(int argc, char* argv[])
{
	if ( argc != 2 )
	{
		printf( "usage: %s font file name (.ttf)\n", argv[0] );
		return EXIT_FAILURE;
	}

	//populate caption list
	captionlist caption_list = NULL;

	caption_list=push_back_caption(caption_list,"Download",0,0x0000FF);
	caption_list=push_back_caption(caption_list,"Upload"  ,1,0xFF0000);

	//print_list_caption(caption_list);

	//populate coordinate list
	coordlist coordinate_list=NULL;

	coordinate_list=push_back_coord(coordinate_list,0,0,0);
	coordinate_list=push_back_coord(coordinate_list,0,1,90);
	coordinate_list=push_back_coord(coordinate_list,0,2,84);
	coordinate_list=push_back_coord(coordinate_list,0,3,98);
	coordinate_list=push_back_coord(coordinate_list,0,4,94);
	coordinate_list=push_back_coord(coordinate_list,0,5,85);
	coordinate_list=push_back_coord(coordinate_list,0,6,90);
	coordinate_list=push_back_coord(coordinate_list,0,7,99);
	coordinate_list=push_back_coord(coordinate_list,0,8,94);
	coordinate_list=push_back_coord(coordinate_list,1,0,0);
	coordinate_list=push_back_coord(coordinate_list,1,1,92);
	coordinate_list=push_back_coord(coordinate_list,1,2,90);
	coordinate_list=push_back_coord(coordinate_list,1,3,98);
	coordinate_list=push_back_coord(coordinate_list,1,4,92);
	coordinate_list=push_back_coord(coordinate_list,1,5,82);
	coordinate_list=push_back_coord(coordinate_list,1,6,98);
	coordinate_list=push_back_coord(coordinate_list,1,7,94);
	coordinate_list=push_back_coord(coordinate_list,1,8,90);

	//print_list_coord(coordinate_list);

	//populate plot parameter object
	plot_params params;

	params.screen_width=800;
	params.screen_heigth=640;
	params.plot_window_title="plot-sdl";
	params.font_text_path=argv[1];
	params.font_text_size=18;
	params.caption_text_x="Temps (s)";
	params.caption_text_y="Debit (Mbit/s)";
	params.caption_list = caption_list;
	params.coordinate_list = coordinate_list;
	params.scale_x = 1;
	params.scale_y = 10;
	params.max_x = 8.5;
	params.max_y = 120;

	int ret = plot_graph(&params);

	if (ret == EXIT_FAILURE)
	{
		printf("plot_graph return with status %d\n",ret);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}