package es.lautech.slackgine;

public class Slackgine {
	native public String PruebaString ();

	static {
        System.loadLibrary("Slackgine-jni-bind");
    }
}
