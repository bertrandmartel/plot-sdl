from plotsdl.plot import CaptionList
from plotsdl.plot import CoordinateList
from plotsdl.plot import PlotParams

caption = CaptionList()
coordinate = CoordinateList()

caption.push_back("Download",0,0x0000FF)
caption.push_back("Upload"  ,1,0xFF0000)

coordinate.push_back(0,0,0)
coordinate.push_back(0,1,90)
coordinate.push_back(0,2,84)
coordinate.push_back(0,3,98)
coordinate.push_back(0,4,94)
coordinate.push_back(0,5,85)
coordinate.push_back(0,6,90)
coordinate.push_back(0,7,99)
coordinate.push_back(0,8,94)
coordinate.push_back(1,0,0)
coordinate.push_back(1,1,92)
coordinate.push_back(1,2,90)
coordinate.push_back(1,3,98)
coordinate.push_back(1,4,92)
coordinate.push_back(1,5,82)
coordinate.push_back(1,6,98)
coordinate.push_back(1,7,94)
coordinate.push_back(1,8,90)

caption.log()
coordinate.log()

plot_params = PlotParams(800,
	640,
	"plot-sdl",
	"../plot-sdl-test/opensans.ttf",
	18,
	"Time (s)",
	"Speed (Mbit/s)",
	caption,
	coordinate,
	1,
	10,
	8.5,
	120);

ret = plot_params.plot_graph()