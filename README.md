# plot-sdl : plot graphic lib using SDL2 #


<i>Last update 30/10/2015</i>

Small library using only SDL2 and SDL_TTF libraries to plot points into a 2D graph

Current features available:

* add points(x,y) to graph with a specific caption identified by id
* add a new caption identified by id,a specified color and a text
* set x label and y label
* set font text used for the graph (ttf font text file must be included)
* set font-size
* set scale value and max value for X axis and Y axis (graduation are shown on the graph)
* set window size (width/height)


<i>All points for a specific caption identifier will be joined by a line</i>
<i>For now, only positive part of x,y axis is shown</i>

![screenshot](https://raw.github.com/akinaru/plot-sdl/master/plot.png)

* launch included test project :

```
cd plot-sdl-lib
make
./release/plot ./plot-sdl-test/opensans.ttf
```

<i>test program require font text file (ttf) in argument, you can change it at will</i>

<hr/>

<h4>Using from C/C++</h4>

* headers :

```
#include "plot.h"
#include "llist.h"
```

* create a list of caption for all your graph you want to plot : 

```
captionlist caption_list = NULL;

caption_list=push_back_caption(caption_list,"Download",0,0x0000FF);
caption_list=push_back_caption(caption_list,"Upload"  ,1,0xFF0000);
```

* Function ``push_back_caption(captionlist catopn_list, char * caption_txt,int caption_id,int color) `` description :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``caption_list`` | captionlist    | pointer to caption list  |
|``caption_txt`` | char*    | caption label     |
|``caption_id`` | int    | caption identifier      |
|``caption_color`` | int    | caption color (hexa format)      |


* create a list of coordinate for all the points to draw :

```
coordlist coordinate_list=NULL;

coordinate_list=push_back_coord(coordinate_list,0,0,0);
coordinate_list=push_back_coord(coordinate_list,0,1,90);
coordinate_list=push_back_coord(coordinate_list,0,2,84);
coordinate_list=push_back_coord(coordinate_list,0,3,98);
coordinate_list=push_back_coord(coordinate_list,0,4,94);
coordinate_list=push_back_coord(coordinate_list,0,5,85);
```

* Function ``push_back_coord(coordlist coord_list,int caption_id, float x,float y) `` description :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``coord_list`` | coordlist    | pointer to coordinate list  |
|``caption_id`` | int    | caption identifier      |
|``x`` | float    | x coordinate      |
|``y`` | float    | y coordinate      |

<i>One point is bound to a specific caption with ``caption_id`` property</i>

* instanciate plot parameter structure to set all your specific parameters :

```
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
```

* ``plot_params`` structure definition :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``screen_width`` | int    | window screen width  |
|``screen_heigth`` | int    | window screen heigth      |
|``plot_window_title`` | char*    | window title      |
|``font_text_path`` | char*    | ttf font text file path      |
|``font_text_size`` | int    | font size used      |
|``caption_text_x`` | char*    | caption label fot axis X     |
|``caption_text_y`` | char*    | caption label fot axis Y       |
|``caption_list`` | captionlist    | caption list       |
|``coordinate_list`` | coordlist    | coordinate list      |
|``scale_x`` | float    | scale graduation used for axis X      |
|``scale_y`` | float    | scale graduation used for axis Y      |
|``max_x`` | float    | max value for axis X      |
|``max_y`` | float    | max value for axis Y      |

* Plot graph with ``int plot_graph(plot_params *params)`` :

```
int ret = plot_graph(&params);
```

* This will display graph in a new window and will block until a ``SDL_QUIT`` event is received.

<hr/>

<h4>Using from Python</h4>

Build python wrapper extension :

```
cd python-wrapper

./build-wrapper.sh
```

``plot.so`` is localed in python-wrapper/plotsdl directory.

* import :

```
from plotsdl.plot import CaptionList
from plotsdl.plot import CoordinateList
from plotsdl.plot import PlotParams
```

* create a list of caption for all your graph you want to plot : 

```
caption = CaptionList()


caption.push_back("Download",0,0x0000FF)
caption.push_back("Upload"  ,1,0xFF0000)
```

* Function ``push_back(char * caption_txt,int caption_id,int color) `` description :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``caption_txt`` | char*    | caption label     |
|``caption_id`` | int    | caption identifier      |
|``caption_color`` | int    | caption color (hexa format)      |

* create a list of coordinate for all the points to draw :

```
coordinate = CoordinateList()

coordinate.push_back(0,0,0)
coordinate.push_back(0,1,90)
coordinate.push_back(0,2,84)
coordinate.push_back(0,3,98)
coordinate.push_back(1,0,0)
coordinate.push_back(1,1,92)
coordinate.push_back(1,2,90)
```

* Function ``push_back(int caption_id, float x,float y) `` description :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``caption_id`` | int    | caption identifier      |
|``x`` | float    | x coordinate      |
|``y`` | float    | y coordinate      |


<i>One point is bound to a specific caption with ``caption_id`` property</i>

* instanciate plot parameter structure to set all your specific parameters :

```
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
	120)
```

* ``PlotParams`` definition :

| property             | Type              | comment
|--------------|--------------|------------------------|
|``screen_width`` | int    | window screen width  |
|``screen_heigth`` | int    | window screen heigth      |
|``plot_window_title`` | char*    | window title      |
|``font_text_path`` | char*    | ttf font text file path      |
|``font_text_size`` | int    | font size used      |
|``caption_text_x`` | char*    | caption label fot axis X     |
|``caption_text_y`` | char*    | caption label fot axis Y       |
|``caption_list`` | CaptionList    | caption list       |
|``coordinate_list`` | CoordinateList    | coordinate list      |
|``scale_x`` | float    | scale graduation used for axis X      |
|``scale_y`` | float    | scale graduation used for axis Y      |
|``max_x`` | float    | max value for axis X      |
|``max_y`` | float    | max value for axis Y      |

* Plot graph with ``int plot_graph(plot_params *params)`` :

```
int ret = plot_params.plot_graph()
```

<b>Python test</b>

```
cd python-wrapper
python test.py

```

<b>Clean python wrapper</b>

```
./build-wrapper.sh clean
```

<i>font file wont be found if launched from a different directory</i>

<b>Cython install</b>

* https://github.com/cython/cython/wiki/Installing

More info about Cython usage : 

* http://docs.cython.org/src/tutorial/cython_tutorial.html
* http://docs.cython.org/src/userguide/external_C_code.html

<br/>

<h4>Using Android</h4>

<b>Android wrapper under construction</b>

<h4>External link</h4>

* algorithm for drawing / filling circle : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
* midpoint circle algorithm : https://en.wikipedia.org/wiki/Midpoint_circle_algorithm

<hr/>