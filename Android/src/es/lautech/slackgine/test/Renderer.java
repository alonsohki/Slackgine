package es.lautech.slackgine.test;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import es.lautech.slackgine.Slackgine;
import es.lautech.slackgine.l3m;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class Renderer implements GLSurfaceView.Renderer
{
	public Renderer ( Context context )
	{
		m_slackgine = new Slackgine ();
	}
	
	public void onSurfaceCreated(GL10 glUnused, EGLConfig config)
	{
		m_slackgine.Initialize();
	}
	
	public void onDrawFrame ( GL10 glUnused )
	{
		GLES20.glViewport(0, 0, m_width, m_height);
		GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		
		if ( m_model != null )
		{
			m_slackgine.renderer().BeginScene ();
			m_slackgine.renderer().Render ( m_model );
			m_slackgine.renderer().EndScene ();
		}
	}
	
	public void onSurfaceChanged(GL10 glUnused, int width, int height)
	{
		m_width = width;
		m_height = height;
	}
	
	public void SetModel ( l3m model )
	{
		m_model = model;
	}
	
	private int m_width;
	private int m_height;
	private l3m m_model = null;
	private Slackgine m_slackgine;
}
