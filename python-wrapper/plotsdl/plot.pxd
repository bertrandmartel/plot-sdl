cimport cllist

cdef extern from "plotsdl/plot.h":

	struct plot_params_struct:
		int                screen_width
		int                screen_heigth
		char *             plot_window_title
		char *             font_text_path
		int                font_text_size
		char *             caption_text_x
		char *             caption_text_y
		cllist.captionlist caption_list
		cllist.coordlist   coordinate_list
		float              scale_x
		float              scale_y
		float              max_x
		float              max_y

	ctypedef plot_params_struct plot_params

	int plot_graph(plot_params *params)