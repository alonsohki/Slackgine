package es.lautech.slackgine;

import java.io.*;

import android.util.Log;

public class l3m
{
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
	
	// Pointer to the C++ instance
	private long m_jniInstance;
	
	// Construction / Destruction
	native private long Createl3mInstance ();
	native private long Createl3mInstance_type ( String type );
	native private void Destroyl3mInstance ( long instance );
	
	public l3m ()
	{
		m_jniInstance = Createl3mInstance ();
	}
	
	protected l3m ( String type )
	{
		m_jniInstance = Createl3mInstance_type ( type );
	}
	
	protected void finalize() throws Throwable
	{
	    Destroyl3mInstance ( m_jniInstance );
	    super.finalize();
	}
	
	// Functions
	native public boolean		Load				( InputStream is );	
	native public boolean		Save				( OutputStream os );
	
	native public String		type				();
	native public String		error				();
	
	native protected void		DeclareMetadata		( String name );
	// Override these
	protected boolean			SaveMetadata		( String name, OutputStream os )
	{
		return true;
	}
	protected boolean			LoadMetadata		( String name, InputStream os )
	{
		return true;
	}
	
	native protected boolean	Write16				( short[] data, int nmemb, OutputStream os );
	native protected boolean	Write32				( int[] data, int nmemb, OutputStream os );
	native protected boolean	Write64				( long [] data, int nmemb, OutputStream os );
	native protected boolean	WriteFloat			( float[] data, int nmemb, OutputStream os );
	native protected boolean	WriteStr			( String str, OutputStream os );
	native protected boolean	WriteData			( byte[] data, int size, int nmemb, OutputStream os );
	
	native protected int		Read16				( short[] dest, int nmemb, InputStream is );
	native protected int		Read32				( int[] dest, int nmemb, InputStream is );
	native protected int		Read64				( long [] dest, int nmemb, InputStream is );
	native protected int		ReadFloat			( float[] dest, int nmemb, InputStream is );
	native protected String		ReadStr				( InputStream is );
	native protected int		ReadData			( byte[] dest, int size, int nmemb, InputStream is );
}
