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
	native public boolean		InternalLoad		( byte[] data, int size );
	public boolean				Load				( InputStream is )
	{
		ByteArrayOutputStream os = new ByteArrayOutputStream();
		byte buffer [] = new byte [ 4096 ];
		int bytes;
		
		do
		{
			try {
				bytes = is.read(buffer);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return false;
			}
			if ( bytes > 0 )
				os.write(buffer, 0, bytes);
		} while ( bytes > -1 );
		return InternalLoad ( os.toByteArray(), os.size() );
	}
	
	native public boolean		InternalSave		( ByteArrayOutputStream os );
	public boolean				Save				( OutputStream os )
	{
		ByteArrayOutputStream internal_os = new ByteArrayOutputStream ();
		boolean state = InternalSave ( internal_os );
		try {
			os.write(internal_os.toByteArray());
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	native public String		type				()
	native public String		error				();
	
	protected void				DeclareMetadata		( String name );
	protected boolean			SaveMetadata		( String )
}
