package de.pixinger77.openregattastartsystem;

import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends AppCompatActivity implements Fragment_Base.OnFragmentBaseListener {

    private StateMachine _StateMachine = null;

    public MainActivity() {
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // Init Emergency Shutdown
        FloatingActionButton fab = findViewById(R.id.fabEmergencyShutdown);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                _StateMachine.requestEmergency();
                Snackbar.make(view, "Emergency requested", Snackbar.LENGTH_LONG).setAction("Action", null).show();
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d("PIX", "OnResume");
        this._StateMachine = new StateMachine(this.getSupportFragmentManager(), this);
    }

    @Override
    protected void onPause() {
        Log.d("PIX", "OnPause");
        this._StateMachine.dispose();
        this._StateMachine = null;

        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }
        if (id == R.id.action_reset) {
            if (this._StateMachine != null)
                this._StateMachine.requestReset();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    @Override
    public StateMachine getStateMachine() {
        return this._StateMachine;
    }
}
