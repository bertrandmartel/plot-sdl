cimport cllist

cdef class Caption:
	cdef cllist.captionlist _caption_list
	def __init__(self):
		self._caption_list = NULL
