package es.lautech.slackgine;

import java.*;

public class Slackgine
{
	public Slackgine ()
	{
		
	}
	
	native public String PruebaString ();

	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
}
