package es.lautech.slackgine;

public class STLostream extends java.io.OutputStream
{
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
	
	// Pointer to the C++ instance
	private long m_jniInstance;
	
	private STLostream ()
	{
	}
	
	// Functions
	native public void write ( byte[] data, int start, int count );
	public void write ( byte[] data )
	{
		write(data, 0, data.length);
	}
	public void write ( int data )
	{
		byte[] stupidFunction = new byte[1];
		stupidFunction[0] = (byte)data;
		write(stupidFunction, 0, 1);
	}
}
