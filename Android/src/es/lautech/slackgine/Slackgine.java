package es.lautech.slackgine;

public class Slackgine
{
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }

	// Pointer to the C++ instance
	private long m_jniInstance;

	// Construction / Destruction
	native private long CreateSlackgineInstance ();
	public Slackgine ()
	{
		m_jniInstance = CreateSlackgineInstance ();
	}
	
	native private void DestroySlackgineInstance ( long instance );
	protected void finalize() throws Throwable
	{
	    DestroySlackgineInstance ( m_jniInstance );
	    super.finalize();
	}
	
	// Bound functions
	native public boolean		Initialize		();
	native public Renderer		renderer 		();
}
