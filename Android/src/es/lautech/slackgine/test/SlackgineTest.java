package es.lautech.slackgine.test;

import java.io.*;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.TextView;
import es.lautech.slackgine.*;

public class SlackgineTest extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LayoutInflater inflater = (LayoutInflater)this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = inflater.inflate(R.layout.main, null);
        
        GLSurfaceView canvas = (GLSurfaceView)view.findViewById(R.id.GLCanvas);
        canvas.setEGLContextClientVersion(2);
        canvas.setRenderer(new Renderer(this));
        
        setContentView(view);
        
        l3mWithDescription model = new l3mWithDescription ( "" );
        try {
        	FileInputStream is = new FileInputStream("/sdcard/chromatic_tri.l3m");
			if ( !model.Load( is ) )
				Log.e ( "SlackgineTest", "Unable to load the model: " + model.error() );
			else
			{
		        TextView titulo = (TextView)view.findViewById(R.id.Titulo);
		        titulo.setText(model.description());
			}
			
			FileOutputStream os = new FileOutputStream("/sdcard/copia.l3m");
			if ( !model.Save(os))
				Log.e ( "SlackgineTest", "Unable to save the model :" + model.error() );
        }
        catch ( IOException e )
        {
		}
    }
}