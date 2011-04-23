package es.lautech.slackgine;

import java.*;

public class Slackgine
{
	public Slackgine ()
	{}
	
	native public String PruebaString ();
	native public String Render ();

	private static Slackgine ms_instance = null;
	public static Slackgine Instance ()
	{
		if ( ms_instance == null )
			ms_instance = new Slackgine ();
		return ms_instance;
	}
	
	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
}
