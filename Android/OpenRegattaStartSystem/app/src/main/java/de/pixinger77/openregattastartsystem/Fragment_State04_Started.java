package de.pixinger77.openregattastartsystem;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;


public class Fragment_State04_Started extends Fragment_Base {

    private TextView txtTimer = null;

    public Fragment_State04_Started() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_state04__started, container, false);
    }

    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        txtTimer = (TextView)view.findViewById(R.id.txtTimer);
        txtTimer.setText("");

        Button btnAbortSingle = view.findViewById(R.id.btnAbortSingle);
        btnAbortSingle.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestAbortSingle();
            }
        });
        Button btnAbortAll = view.findViewById(R.id.btnAbortAll);
        btnAbortAll.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestAbortAll();
            }
        });
        Button btnAbort = view.findViewById(R.id.btnAbort);
        btnAbort.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestAbort();
            }
        });
        Button btnReset = view.findViewById(R.id.btnReset);
        btnReset.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestReset();
            }
        });
        Button btnUpdate = view.findViewById(R.id.btnUpdate);
        btnUpdate.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestStatus();
            }
        });

        super.onViewCreated(view, savedInstanceState);
    }

    @Override
    protected void OnUpdateStatus(NetworkHelper.Status status) {
        NetworkHelper.StatusStarted castedStatus = (NetworkHelper.StatusStarted)status; // Casted to underlying type.
    }
}
