package es.lautech.slackgine.test;

import es.lautech.slackgine.l3m;
import java.io.*;

import android.util.Log;

public class l3mWithDescription extends l3m
{
	private String m_description;
	
	public l3mWithDescription ( String desc )
	{
		super ( "descriptable" );
		m_description = desc;
		
		DeclareMetadata ( "description" );
		DeclareMetadata ( "other stuff" );
	}
	
	protected boolean LoadMetadata ( String name, InputStream stream )
	{
		if ( name.compareTo("description") == 0 )
		{
			m_description = ReadStr ( stream );
			return true;
		}
		else if ( name.compareTo("other stuff") == 0 )
		{ 
			
			int[] magicNumber = new int[1];
			if ( Read32 ( magicNumber, 1, stream ) > 0 )
			{
				if ( magicNumber[0] == 0xFABADA )
					return true;
				else
				{
					Log.e ( "l3mWithDescription", "Invalid magic number!" );
					return false;
				}
			}
			else
			{
				Log.e ( "l3mWithDescription", "Unable to read the magic number" );
				return false;
			}
		}
		
		return false;
	}
	
	protected boolean SaveMetadata ( String name, OutputStream stream )
	{
		if ( name.compareTo("description") == 0 )
			return WriteStr ( m_description, stream );
		else if ( name.compareTo("other stuff") == 0 )
		{
			int[] magicNumber = new int[1];
			magicNumber[0] = 0xFABADA;
			return Write32 ( magicNumber, 1, stream );
		}
		return true;
	}
	
	public String description () { return m_description; }
}
