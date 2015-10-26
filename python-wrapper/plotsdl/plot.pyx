cimport cllist
cimport plot

cdef class CaptionList:
	cdef cllist.captionlist _caption_list
	def __init__(self):
		self._caption_list = NULL
	def push_back(self, char* caption_txt,int caption_id,int color):
		self._caption_list = cllist.push_back_caption(self._caption_list, caption_txt,caption_id,color)
	def log(self):
		cllist.print_list_caption(self._caption_list)
	def clear(self):
		if self._caption_list is not NULL:
			self._caption_list = cllist.clear_caption(self._caption_list)

cdef class CoordinateList:
	cdef cllist.coordlist _coordinate_list
	def __init__(self):
		self._coordinate_list = NULL
	def push_back(self,int caption_id, float x,float y):
		self._coordinate_list = cllist.push_back_coord(self._coordinate_list,caption_id,x,y)
	def log(self):
		cllist.print_list_coord(self._coordinate_list)
	def clear(self):
		if self._coordinate_list is not NULL:
			self._coordinate_list = cllist.clear_coord(self._coordinate_list)

cdef class PlotParams:
	cdef plot.plot_params _plot_params
	def __init__(self,
		int screen_width,
		int screen_heigth,
		char * plot_window_title,
		char * font_text_path,
		int font_text_size,
		char * caption_text_x,
		char * caption_text_y,
		CaptionList caption_list,
		CoordinateList coordinate_list,
		float scale_x,
		float scale_y,
		float max_x,
		float max_y):

		self._plot_params.screen_width      = screen_width
		self._plot_params.screen_heigth     = screen_heigth
		self._plot_params.plot_window_title = plot_window_title
		self._plot_params.font_text_path    = font_text_path
		self._plot_params.font_text_size    = font_text_size
		self._plot_params.caption_text_x    = caption_text_x
		self._plot_params.caption_text_y    = caption_text_y
		self._plot_params.caption_list      = caption_list._caption_list
		self._plot_params.coordinate_list   = coordinate_list._coordinate_list
		self._plot_params.scale_x           = scale_x
		self._plot_params.scale_y           = scale_y
		self._plot_params.max_x             = max_x
		self._plot_params.max_y             = max_y

	cpdef int plot_graph(self):
		return plot.plot_graph(&self._plot_params)