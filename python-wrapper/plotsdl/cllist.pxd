cdef extern from "plotsdl/llist.h":

	struct caption_item:
		char * caption_txt
		int    caption_id
		int    caption_color
		caption_item *nxt

	ctypedef caption_item* captionlist

	struct coordinate_item:
		float  x
		float  y
		int    caption_id
		coordinate_item *nxt

	ctypedef coordinate_item* coordlist

	captionlist clear_caption(captionlist list)

	coordlist clear_coord(coordlist list)

	captionlist push_back_caption(captionlist list, char* caption_txt,int caption_id,int color)

	coordlist push_back_coord(coordlist list,int caption_id, float x,float y)

	void print_list_caption(captionlist list)

	void print_list_coord(coordlist list);