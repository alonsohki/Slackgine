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
        
        TextView titulo = (TextView)view.findViewById(R.id.Titulo);
        titulo.setText("Test de Slackgine");

        GLSurfaceView canvas = (GLSurfaceView)view.findViewById(R.id.GLCanvas);
        canvas.setEGLContextClientVersion(2);
        canvas.setRenderer(new Renderer(this));
        
        setContentView(view);
        
        l3m model = new l3m ();
        try {
			model.Load( new FileInputStream("/sdcard/chromatic_tri.l3m") );
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}