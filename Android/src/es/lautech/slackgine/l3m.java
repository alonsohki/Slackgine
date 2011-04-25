package es.lautech.slackgine;

import java.io.*;

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
	native protected boolean	SaveMetadata		( String name, OutputStream os );
	native protected boolean	LoadMetadata		( String name, InputStream os );
}
