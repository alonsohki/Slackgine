package es.lautech.slackgine;

public class Slackgine
{
	private long m_jniInstance;

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
	
	native public Renderer renderer ();

	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
}
