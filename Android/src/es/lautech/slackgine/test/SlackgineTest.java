package es.lautech.slackgine.test;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import es.lautech.slackgine.*;

public class SlackgineTest extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Slackgine sg = new Slackgine ();
        Log.w("Probando L3M", sg.PruebaString() );
    }
}