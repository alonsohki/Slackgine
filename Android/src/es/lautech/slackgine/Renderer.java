package es.lautech.slackgine;

import android.util.Log;

public class Renderer
{
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
	
	// Pointer to the C++ instance
	private long m_jniInstance;
	
	// Let only the JNI bind to instantiate objects of this class
	private Renderer ()
	{
	}
	
	// Functions
	public native boolean		Initialize		();
	public native boolean		BeginScene		();
	public native boolean		Render			( l3m model );
	public native boolean		EndScene		();
}
