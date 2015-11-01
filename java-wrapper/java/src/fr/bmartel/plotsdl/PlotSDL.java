package fr.bmartel.plotsdl;

public class PlotSDL {

	static {
		System.loadLibrary("plotsdl_wrapper");
	}

	/**
	 * Set plot parameters
	 *
	 * @param screen_width
	 *            screen width
	 * @param screen_heigth
	 *            screen heigth
	 * @param title
	 *            sdl window title
	 * @param fontFileName
	 *            font file absolute path
	 * @param textSize
	 *            text size (depends on font)
	 * @param xcaptionText
	 *            caption for x axis
	 * @param ycaptionText
	 *            caption for y axis
	 * @param scale_x
	 *            scale value on x axis
	 * @param scale_y
	 *            scale value on y axis
	 * @param max_x
	 *            max value for x axis
	 * @param max_y
	 *            max value for y axis
	 */
	public native void nativePushPlotParams(int screen_width, int screen_heigth, String title, String fontFileName, int textSize, String xcaptionText,
			String ycaptionText, float scale_x, float scale_y, float max_x, float max_y);

	/**
	 * Add a coordinate point to coordinate point list
	 *
	 * @param caption_id
	 *            caption identifier linked to this point
	 * @param xcoord
	 *            point x coordinate
	 * @param ycoord
	 *            point y coordinate
	 */
	public native void nativePushCoordinate(int caption_id, int xcoord, int ycoord);

	/**
	 * Add a new graph caption to plot
	 *
	 * @param caption_txt
	 *            caption label
	 * @param caption_id
	 *            caption unique identifier
	 * @param caption_color
	 *            caption color
	 */
	public native void nativePushCaption(String caption_txt, int caption_id, int caption_color);

	/**
	 * will call an SDL_Quit event
	 */
	public native void nativePlotsdlQuit();
	
	/**
	 * plot graph
	 */
	public native void nativePlotGraph();
	
	/**
	 * initialize SDL
	 */
	public native void nativeInit();

	public static void main(String[] args) {
		
		PlotSDL sdl = new PlotSDL();

		sdl.nativePushCoordinate(0, 0, 0);
		sdl.nativePushCoordinate(0, 1, 90);
		sdl.nativePushCoordinate(0, 2, 84);
		sdl.nativePushCoordinate(0, 3, 98);
		sdl.nativePushCoordinate(0, 4, 94);
		sdl.nativePushCoordinate(0, 5, 85);
		sdl.nativePushCoordinate(0, 6, 90);
		sdl.nativePushCoordinate(0, 6, 90);
		sdl.nativePushCoordinate(0, 7, 99);
		sdl.nativePushCoordinate(0, 8, 94);
		sdl.nativePushCoordinate(1, 0, 0);
		sdl.nativePushCoordinate(1, 1, 92);
		sdl.nativePushCoordinate(1, 2, 90);
		sdl.nativePushCoordinate(1, 3, 98);
		sdl.nativePushCoordinate(1, 4, 92);
		sdl.nativePushCoordinate(1, 5, 82);
		sdl.nativePushCoordinate(1, 6, 98);
		sdl.nativePushCoordinate(1, 7, 94);
		sdl.nativePushCoordinate(1, 8, 90);

		sdl.nativePushCaption("Download", 0, 0x0000FF);
		sdl.nativePushCaption("Upload", 1, 0xFF0000);

		sdl.nativePushPlotParams(1280, 720, "plot-sdl", "/home/abathur/Bureau/open_source/plot-sdl/plot-sdl-test/opensans.ttf", 25, "Time (s)",
				"Speed (Mbit/s)", 1f, 10f, 8.5f, 120f);
		
		sdl.nativePlotGraph();
	}
}
