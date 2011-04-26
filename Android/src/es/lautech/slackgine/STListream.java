package es.lautech.slackgine;

public class STListream extends java.io.InputStream
{
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
	
	// Pointer to the C++ instance
	private long m_jniInstance;
	
	private STListream ()
	{
	}
	
	// Functions
	native public int read ( byte[] data, int start, int count );
	public int read ( byte[] data )
	{
		return read ( data, 0, data.length );
	}
	public int read ()
	{
		byte[] stupidFunction = new byte[1];
		int size = read(stupidFunction, 0, 1);
		if ( size == 1 )
			return stupidFunction[0];
		return -1;
	}
}
