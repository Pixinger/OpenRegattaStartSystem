package de.pixinger77.openregattastartsystem;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.material.snackbar.Snackbar;


/**
 * A simple {@link Fragment} subclass.
 */
public class Fragment_Disconnected extends Fragment_Base {

    private TextView txtDebug;
    private TextView txtStatus;
    private Button btnConnect;

    public Fragment_Disconnected() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_disconnected, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        txtDebug = view.findViewById(R.id.txtDebug);
        txtStatus = view.findViewById(R.id.txtStatus);

        btnConnect = view.findViewById(R.id.btnConnect);
        btnConnect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                TryConnection();
            }
        });

        super.onViewCreated(view, savedInstanceState);
    }
    public void setErrorMessage(String message) {
        if (this.txtDebug != null)
            this.txtDebug.setText(message);
    }

    @Override
    public void onResume() {
        super.onResume();

        TryConnection();
    }

    @Override
    protected void OnUpdateStatus(NetworkHelper.Status status) {
        NetworkHelper.StatusDisconnected castedStatus = (NetworkHelper.StatusDisconnected)status; // Casted to underlying type.
        if (this.txtDebug != null)
            this.txtDebug.setText("UpdateStatus called");
    }

    private void TryConnection()
    {
        txtStatus.setVisibility(View.VISIBLE);
        btnConnect.setEnabled(false);

        _FragmentBaseListener.getStateMachine().requestStatus();
        Snackbar.make(getView(), "Trying to connect...", Snackbar.LENGTH_LONG).setAction("Action", null).show();

        getView().postDelayed(new Runnable() {
            @Override
            public void run() {
                txtStatus.setVisibility(View.GONE);
                btnConnect.setEnabled(true);
            }
        }, 6000);
    }
}
