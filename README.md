# plot-sdl #

<i>Last update 01/11/2015</i>

Small library using SDL2 and SDL_TTF used to plot points into a 2D graph

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

plot-sdl is written in [C](#c) and has some wrappers for other languages/platforms :

* [Python](#python)
* [Android](#android)
* [Java](#java)

![screenshot](https://raw.github.com/bertrandmartel/plot-sdl/master/plot.png)

* launch included test project :

```
cd plot-sdl-lib
make
./release/plot ./plot-sdl-test/opensans.ttf
```

<i>test program require font text file (ttf) in argument, you can change it at will</i>

<hr/>

## C

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

## Python

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

<hr/>

## Android

<b>Setup</b>

* Add android-ndk directory to your path before setup

* In android-wrapper directory you'll find init-script.sh which will retrieve SDL and SDL_TTF mercurial repository 

```
cd android-wrapper
./init-script.sh
```

* By default, API_ABI is set to ``all`` which takes quite a long time to build SDL and SDL_TTF for all your ndk arch. You can modify ``./app/src/main/jni/Application.mk`` with a specific abi instead of ``APP_ABI := all``

* Then you can build with ``build`` parameter

```
./init-script.sh build
```

* When you are done, open Android-studio project located in ``android-wrapper`` directory and run test app or build directly with gradle wrapper : ``./gradlew clean build``

<b>JNI API</b>

Located in ``./app/src/main/jni/wrapper``, ``SDL_android_main.c`` contains all JNI api used :

* ``void nativePushCoordinate(int caption_id, int xcoord, int ycoord)`` : push a point with give coordinate (x,y)

* ``void nativePushCaption(String caption_txt, int caption_id, int caption_color)`` : push a caption label identifying a graph

* ``void nativePlotsdlQuit()`` : call SDL quit event to terminate process

* ``void onNativeResize(int x, int y, int format)`` : resize SDL window to x width, y heigth with given pixel format

* ``void nativePushPlotParams(int screen_width, int screen_heigth, String title, String fontFileName, int textSize, String xcaptionText, String ycaptionText, float scale_x, float scale_y, float max_x, float max_y)`` : push plot parameters used to plot points and setup graphs

* ``void nativeInit()`` : initialize SDL and plot graph

<b>Clean android wrapper</b>

* To make an ndk clean :

```
./init-script.sh clean
```

* reset android wrapper (this will remove SDL and SDL_TTF repo) :

```
./init-script.sh distclean
```

<hr/>

## Java

These are the same api as Android wrapper

* First, build wrapper : 

```
cd java-wrapper
make
```

* Open Eclipse and open-project located in ``java-wrapper/java`` directory

* Build project with ant : ``build.xml``

* in command line : 

```
cd java-wrapper
WORKING_DIR=`pwd`
java -Djava.library.path=$WORKING_DIR -jar ./java/release/plotsdl-1.0.jar ../plot-sdl-test/opensans.ttf
```

<hr/>

## External links

* tutorial on SDL : http://www.willusher.io/pages/sdl2/
* installing cython : https://github.com/cython/cython/wiki/Installing
* tutorial for cython : http://docs.cython.org/src/tutorial/cython_tutorial.html
* cython Interfacing with External C Code : http://docs.cython.org/src/userguide/external_C_code.html
* algorithm for drawing / filling circle : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
* midpoint circle algorithm : https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
* JNI : https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html#zz-2.1
* guide for building SDL for Android : https://wiki.libsdl.org/Android
* additional explanation for SDL on Android : https://amos.me/blog/2013/android-sdl-and-the-ouya/

<hr/>
