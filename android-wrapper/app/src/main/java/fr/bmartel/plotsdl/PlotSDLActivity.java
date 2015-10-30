/**
 * The MIT License (MIT)
 * <p/>
 * Copyright (c) 2015 Bertrand Martel
 * <p/>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p/>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package fr.bmartel.plotsdl;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

/**
 * Plot SDL Activity
 * <p/>
 * <ul>
 * <li>set plot parameters</li>
 * <li>draw plot on a surfaceView defined as main layout</li>
 * <li>plot is being redrawn each time onResume() is called</li>
 * <li>SDL thread is interrupting when onPause() is called resulting in cleaning pointers, lists etc...</li>
 * </ul>
 *
 * @author Bertrand Martel
 */
public class PlotSDLActivity extends Activity implements View.OnKeyListener {

    private static final String TAG = "SDL";

    // Keep track of the paused state
    public static boolean mIsPaused, mIsSurfaceReady, mHasFocus;
    public static boolean mExitCalledFromJava;

    public static int screenWidth = 0;
    public static int screenHeight = 0;

    // Main components
    protected static PlotSDLActivity mSingleton;
    protected static SDLSurface mSurface;
    protected static ViewGroup mLayout;

    // This is what SDL runs in. It invokes SDL_main(), eventually
    protected static Thread mSDLThread;

    // Load the .so
    static {
        System.loadLibrary("SDL2");
        System.loadLibrary("SDL2_ttf");
        System.loadLibrary("plotsdl");
        System.loadLibrary("plotsdl_wrapper");
    }


    public static void initialize() {
        // The static nature of the singleton and Android quirkyness force us to initialize everything here
        // Otherwise, when exiting the app and returning to it, these variables *keep* their pre exit values
        mSingleton = null;
        mSurface = null;
        mLayout = null;
        mSDLThread = null;
        mExitCalledFromJava = false;
        mIsPaused = false;
        mIsSurfaceReady = false;
        mHasFocus = true;
    }

    public static void setFontText(int fileId, String fileName) {
        try {
            InputStream in = getContext().getResources().openRawResource(fileId);
            FileOutputStream out = new FileOutputStream(fileName);
            byte[] buff = new byte[1024];
            int read = 0;

            try {
                while ((read = in.read(buff)) > 0) {
                    out.write(buff, 0, read);
                }
            } finally {
                in.close();
                out.close();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @SuppressLint("NewApi")
    private void getScreenSize() {
        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        screenWidth = size.x;
        screenHeight = size.y;
    }

    // Setup
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.v("SDL", "onCreate():" + mSingleton);
        super.onCreate(savedInstanceState);

        getScreenSize();

        PlotSDLActivity.initialize();
        // So we can call stuff from static callbacks
        mSingleton = this;

        // Set up the surface
        mSurface = new SDLSurface(getApplication());

        mSurface.setOnKeyListener(new View.OnKeyListener() {

            // Key events
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {

                if ((event.getSource() & InputDevice.SOURCE_KEYBOARD) != 0) {
                    if (event.getAction() == KeyEvent.ACTION_DOWN) {
                        if ((event.getSource() & InputDevice.SOURCE_KEYBOARD) != 0) {
                            if (event.getAction() == KeyEvent.ACTION_DOWN && keyCode == KeyEvent.KEYCODE_BACK) {
                                PlotSDLActivity.this.onBackPressed();
                                return false;
                            }
                        }
                        //Log.v("SDL", "key down: " + keyCode);
                        //PlotSDLActivity.onNativeKeyDown(keyCode);
                        return true;
                    } else if (event.getAction() == KeyEvent.ACTION_UP) {
                        //Log.v("SDL", "key up: " + keyCode);
                        //PlotSDLActivity.onNativeKeyUp(keyCode);
                        return true;
                    }
                }

                return false;
            }
        });

        mLayout = new AbsoluteLayout(this);
        mLayout.addView(mSurface);

        setContentView(mLayout);
    }

    /**
     * initialize graph with custom parameters
     */
    public static void initGraph() {

        setFontText(R.raw.opensans, "/sdcard/opensans.ttf");

        nativePushCoordinate(0, 0, 0);
        nativePushCoordinate(0, 1, 90);
        nativePushCoordinate(0, 2, 84);
        nativePushCoordinate(0, 3, 98);
        nativePushCoordinate(0, 4, 94);
        nativePushCoordinate(0, 5, 85);
        nativePushCoordinate(0, 6, 90);
        nativePushCoordinate(0, 6, 90);
        nativePushCoordinate(0, 7, 99);
        nativePushCoordinate(0, 8, 94);
        nativePushCoordinate(1, 0, 0);
        nativePushCoordinate(1, 1, 92);
        nativePushCoordinate(1, 2, 90);
        nativePushCoordinate(1, 3, 98);
        nativePushCoordinate(1, 4, 92);
        nativePushCoordinate(1, 5, 82);
        nativePushCoordinate(1, 6, 98);
        nativePushCoordinate(1, 7, 94);
        nativePushCoordinate(1, 8, 90);

        nativePushCaption("Download", 0, 0x0000FF);
        nativePushCaption("Upload", 1, 0xFF0000);

        nativePushPlotParams(screenWidth, screenHeight, "plot-sdl", "/sdcard/opensans.ttf", 35, "Time (s)", "Speed (Mbit/s)", 1f, 10f, 8.5f, 120f);
    }

    @Override
    public void onBackPressed() {
        Log.v("SDL", "onBackPressed()");
        super.onBackPressed();
    }

    // Events
    @Override
    protected void onPause() {
        Log.v("SDL", "onPause()");
        super.onPause();
        PlotSDLActivity.nativePlotsdlQuit();
    }

    @Override
    protected void onResume() {
        Log.v("SDL", "onResume()");
        super.onResume();
        Log.i(TAG, "is null => " + (PlotSDLActivity.mSDLThread == null));
    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Log.v("SDL", "onWindowFocusChanged(): " + hasFocus);

        PlotSDLActivity.mHasFocus = hasFocus;
    }

    @Override
    public void onLowMemory() {
        Log.v("SDL", "onLowMemory()");
        super.onLowMemory();
    }

    @Override
    protected void onDestroy() {
        Log.v("SDL", "onDestroy()");
        // Send a quit message to the application
        PlotSDLActivity.mExitCalledFromJava = true;

        if (PlotSDLActivity.mSDLThread != null) {
            try {
                PlotSDLActivity.mSDLThread.join();
            } catch (Exception e) {
                Log.v("SDL", "Problem stopping thread: " + e);
            }
            PlotSDLActivity.mSDLThread = null;
        }

        super.onDestroy();
        // Reset everything in case the user re opens the app
        PlotSDLActivity.initialize();
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();
        // Ignore certain special keys so they're handled by Android
        if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN ||
                keyCode == KeyEvent.KEYCODE_VOLUME_UP ||
                keyCode == KeyEvent.KEYCODE_CAMERA ||
                keyCode == 168 || /* API 11: KeyEvent.KEYCODE_ZOOM_IN */
                keyCode == 169 /* API 11: KeyEvent.KEYCODE_ZOOM_OUT */
                ) {
            return false;
        }
        return super.dispatchKeyEvent(event);
    }

    /* The native thread has finished */
    public static void handleNativeExit() {
        Log.i("nativeExit", "exit");
        PlotSDLActivity.mSDLThread = null;
    }


    // Messages from the SDLMain thread
    static final int COMMAND_CHANGE_TITLE = 1;

    /**
     * This method is called by SDL if SDL did not handle a message itself.
     * This happens if a received message contains an unsupported command.
     * Method can be overwritten to handle Messages in a different class.
     *
     * @param command the command of the message.
     * @param param   the parameter of the message. May be null.
     * @return if the message was handled in overridden method.
     */
    protected boolean onUnhandledMessage(int command, Object param) {
        return false;
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {

        if ((event.getSource() & InputDevice.SOURCE_KEYBOARD) != 0) {
            if (event.getAction() == KeyEvent.ACTION_DOWN && keyCode == KeyEvent.KEYCODE_BACK) {
                super.onBackPressed();
                return true;
            }
        }
        return false;
    }

    /**
     * A Handler class for Messages from native SDL applications.
     * It uses current Activities as target (e.g. for the title).
     * static to prevent implicit references to enclosing object.
     */
    protected static class SDLCommandHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            Context context = getContext();
            if (context == null) {
                Log.e(TAG, "error handling message, getContext() returned null");
                return;
            }
            switch (msg.arg1) {
                case COMMAND_CHANGE_TITLE:
                    if (context instanceof Activity) {
                        ((Activity) context).setTitle((String) msg.obj);
                    } else {
                        Log.e(TAG, "error handling message, getContext() returned no Activity");
                    }
                    break;
                default:
                    if ((context instanceof PlotSDLActivity) && !((PlotSDLActivity) context).onUnhandledMessage(msg.arg1, msg.obj)) {
                        Log.e(TAG, "error handling message, command is " + msg.arg1);
                    }
            }
        }
    }

    // Handler for the messages
    Handler commandHandler = new SDLCommandHandler();

    // Send a message from the SDLMain thread
    boolean sendCommand(int command, Object data) {
        Message msg = commandHandler.obtainMessage();
        msg.arg1 = command;
        msg.obj = data;
        return commandHandler.sendMessage(msg);
    }

    /**
     * Set plot parameters
     *
     * @param screen_width  screen width
     * @param screen_heigth screen heigth
     * @param title         sdl window title
     * @param fontFileName  font file absolute path
     * @param textSize      text size (depends on font)
     * @param xcaptionText  caption for x axis
     * @param ycaptionText  caption for y axis
     * @param scale_x       scale value on x axis
     * @param scale_y       scale value on y axis
     * @param max_x         max value for x axis
     * @param max_y         max value for y axis
     */
    public static native void nativePushPlotParams(int screen_width,
                                                   int screen_heigth,
                                                   String title,
                                                   String fontFileName,
                                                   int textSize,
                                                   String xcaptionText,
                                                   String ycaptionText,
                                                   float scale_x,
                                                   float scale_y,
                                                   float max_x,
                                                   float max_y);

    /**
     * Add a coordinate point to coordinate point list
     *
     * @param caption_id caption identifier linked to this point
     * @param xcoord     point x coordinate
     * @param ycoord     point y coordinate
     */
    public static native void nativePushCoordinate(int caption_id, int xcoord, int ycoord);

    /**
     * Add a new graph caption to plot
     *
     * @param caption_txt   caption label
     * @param caption_id    caption unique identifier
     * @param caption_color caption color
     */
    public static native void nativePushCaption(String caption_txt, int caption_id, int caption_color);

    /**
     * will call an SDL_Quit event
     */
    public static native void nativePlotsdlQuit();

    /**
     * initialize SDL
     */
    public static native void nativeInit();

    /*
     * set screen resolution for SDL window
     */
    public static native void onNativeResize(int x, int y, int format);

    public static void flipBuffers() {
    }

    public static boolean setActivityTitle(String title) {
        // Called from SDLMain() thread and can't directly affect the view
        return mSingleton.sendCommand(COMMAND_CHANGE_TITLE, title);
    }

    public static boolean sendMessage(int command, int param) {
        return mSingleton.sendCommand(command, Integer.valueOf(param));
    }

    public static Context getContext() {
        return mSingleton;
    }

    /**
     * @return result of getSystemService(name) but executed on UI thread.
     */
    public Object getSystemServiceFromUiThread(final String name) {
        final Object lock = new Object();
        final Object[] results = new Object[2]; // array for writable variables
        synchronized (lock) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    synchronized (lock) {
                        results[0] = getSystemService(name);
                        results[1] = Boolean.TRUE;
                        lock.notify();
                    }
                }
            });
            if (results[1] == null) {
                try {
                    lock.wait();
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
        }
        return results[0];
    }

    public static Surface getNativeSurface() {
        return PlotSDLActivity.mSurface.getNativeSurface();
    }

    // Audio
    public static int audioInit(int sampleRate, boolean is16Bit, boolean isStereo, int desiredFrames) {
        return 0;
    }

    public static void audioWriteShortBuffer(short[] buffer) {
    }

    public static void audioWriteByteBuffer(byte[] buffer) {
    }

    public static void audioQuit() {
    }

    // Input

    /**
     * @return an array which may be empty but is never null.
     */
    public static int[] inputGetInputDeviceIds(int sources) {
        int[] ids = InputDevice.getDeviceIds();
        int[] filtered = new int[ids.length];
        int used = 0;
        for (int i = 0; i < ids.length; ++i) {
            InputDevice device = InputDevice.getDevice(ids[i]);
            if ((device != null) && ((device.getSources() & sources) != 0)) {
                filtered[used++] = device.getId();
            }
        }
        return Arrays.copyOf(filtered, used);
    }

    // Joystick glue code, just a series of stubs that redirect to the SDLJoystickHandler instance
    public static boolean handleJoystickMotionEvent(MotionEvent event) {
        return false;
    }

    public static void pollInputDevices() {

    }

}

/**
 * Initialize SDL
 */
class SDLMain implements Runnable {
    @Override
    public void run() {
        Log.v("SDL", "nativeInit");
        PlotSDLActivity.initGraph();
        PlotSDLActivity.nativeInit();
        Log.v("SDL", "SDL thread terminated");
    }
}


/**
 * SDLSurface. This is what we draw on, so we need to know when it's created
 * in order to do anything useful.
 * <p/>
 * Because of this, that's where we set up the SDL thread
 */
class SDLSurface extends SurfaceView implements SurfaceHolder.Callback, View.OnTouchListener, SensorEventListener {

    // Sensors
    protected static SensorManager mSensorManager;
    protected static Display mDisplay;

    // Keep track of the surface size to normalize touch events
    protected static float mWidth, mHeight;

    // Startup    
    public SDLSurface(Context context) {
        super(context);
        getHolder().addCallback(this);

        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
        setOnTouchListener(this);

        mDisplay = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);

        // Some arbitrary defaults to avoid a potential division by zero
        mWidth = 1.0f;
        mHeight = 1.0f;
    }

    public Surface getNativeSurface() {
        return getHolder().getSurface();
    }

    // Called when we have a valid drawing surface
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v("SDL", "surfaceCreated()");
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
    }

    // Called when we lose the surface
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v("SDL", "surfaceDestroyed()");
        // Call this *before* setting mIsSurfaceReady to 'false'
        PlotSDLActivity.mIsSurfaceReady = false;
        Log.v("SDL", "nativePlotSdlQuit");
    }

    // Called when the surface is resized
    @Override
    public void surfaceChanged(SurfaceHolder holder,
                               int format, int width, int height) {
        Log.v("SDL", "surfaceChanged()");

        int sdlFormat = 0x15151002; // SDL_PIXELFORMAT_RGB565 by default
        switch (format) {
            case PixelFormat.A_8:
                Log.v("SDL", "pixel format A_8");
                break;
            case PixelFormat.LA_88:
                Log.v("SDL", "pixel format LA_88");
                break;
            case PixelFormat.L_8:
                Log.v("SDL", "pixel format L_8");
                break;
            case PixelFormat.RGBA_4444:
                Log.v("SDL", "pixel format RGBA_4444");
                sdlFormat = 0x15421002; // SDL_PIXELFORMAT_RGBA4444
                break;
            case PixelFormat.RGBA_5551:
                Log.v("SDL", "pixel format RGBA_5551");
                sdlFormat = 0x15441002; // SDL_PIXELFORMAT_RGBA5551
                break;
            case PixelFormat.RGBA_8888:
                Log.v("SDL", "pixel format RGBA_8888");
                sdlFormat = 0x16462004; // SDL_PIXELFORMAT_RGBA8888
                break;
            case PixelFormat.RGBX_8888:
                Log.v("SDL", "pixel format RGBX_8888");
                sdlFormat = 0x16261804; // SDL_PIXELFORMAT_RGBX8888
                break;
            case PixelFormat.RGB_332:
                Log.v("SDL", "pixel format RGB_332");
                sdlFormat = 0x14110801; // SDL_PIXELFORMAT_RGB332
                break;
            case PixelFormat.RGB_565:
                Log.v("SDL", "pixel format RGB_565");
                sdlFormat = 0x15151002; // SDL_PIXELFORMAT_RGB565
                break;
            case PixelFormat.RGB_888:
                Log.v("SDL", "pixel format RGB_888");
                // Not sure this is right, maybe SDL_PIXELFORMAT_RGB24 instead?
                sdlFormat = 0x16161804; // SDL_PIXELFORMAT_RGB888
                break;
            default:
                Log.v("SDL", "pixel format unknown " + format);
                break;
        }

        mWidth = width;
        mHeight = height;
        PlotSDLActivity.onNativeResize(width, height, sdlFormat);
        Log.v("SDL", "Window size:" + width + "x" + height);

        // Set mIsSurfaceReady to 'true' *before* making a call to handleResume
        PlotSDLActivity.mIsSurfaceReady = true;

        if (PlotSDLActivity.mSDLThread == null) {
            // This is the entry point to the C app.
            // Start up the C app thread and enable sensor input for the first time

            PlotSDLActivity.mSDLThread = new Thread(new SDLMain(), "SDLThread");
            PlotSDLActivity.mSDLThread.start();

            // Set up a listener thread to catch when the native thread ends
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        PlotSDLActivity.mSDLThread.join();
                        Log.i("SDL", "after join");
                        PlotSDLActivity.mSDLThread = null;
                    } catch (Exception e) {
                        e.printStackTrace();
                    } finally {
                        // Native thread has finished
                        if (!PlotSDLActivity.mExitCalledFromJava) {
                            PlotSDLActivity.handleNativeExit();
                        }
                    }
                }
            }).start();
        }
    }

    @Override
    public void onDraw(Canvas canvas) {
    }

    // Touch events
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        return false;
    }

    // Sensor events
    public void enableSensor(int sensortype, boolean enabled) {
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
    }
}
