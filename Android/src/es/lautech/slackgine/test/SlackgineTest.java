package es.lautech.slackgine.test;

import android.app.Activity;
import android.content.Context;
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
        TextView valueTV = new TextView(this);
        Slackgine sg = new Slackgine ();
        valueTV.setText(sg.PruebaString());
        valueTV.setId(5);
        valueTV.setLayoutParams(new LayoutParams(
                LayoutParams.FILL_PARENT,
                LayoutParams.WRAP_CONTENT));

        ((LinearLayout) view).addView(valueTV);
        
        setContentView(view);
    }
}