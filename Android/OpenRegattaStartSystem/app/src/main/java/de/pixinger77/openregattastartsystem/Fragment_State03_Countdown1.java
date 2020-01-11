package de.pixinger77.openregattastartsystem;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;


public class Fragment_State03_Countdown1 extends Fragment_Base {

    private long startTick;
    private double startTime;
    private Timer timer = null;
    private TextView txtTimer = null;

    public Fragment_State03_Countdown1() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_state03__countdown1, container, false);
    }

    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        txtTimer = (TextView)view.findViewById(R.id.txtTimer);
        txtTimer.setText("");

        // ClassFlag
        int selectedClassFlag = _FragmentBaseListener.getStateMachine().getSelectedClassFlag();
        ImageView imgClassFlag = view.findViewById(R.id.imgClassFlag);
        if (selectedClassFlag == 0)
            imgClassFlag.setImageResource(R.mipmap.ic_flag_class_opti);
        else
            imgClassFlag.setImageResource(R.mipmap.ic_flag_class_opti);

        // btnAbort
        Button btnAbort = view.findViewById(R.id.btnAbort);
        btnAbort.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestAbort();
            }
        });
        // btnReset
        Button btnReset = view.findViewById(R.id.btnReset);
        btnReset.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                _FragmentBaseListener.getStateMachine().requestReset();
            }
        });
        // btnUpdate
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
        NetworkHelper.StatusCountdown1 castedStatus = (NetworkHelper.StatusCountdown1)status; // Casted to underlying type.


        startTime = castedStatus.getTime();
        startTick = SystemClock.uptimeMillis();

        if (timer != null)
            timer.cancel();

        //Declare the timer
        timer = new Timer();
        //Set the schedule function and rate
        timer.scheduleAtFixedRate(
                new TimerTask() {
                    @Override
                    public void run() {
                        if (getActivity() != null) {
                            getActivity().runOnUiThread(
                                    new Runnable() {
                                        @Override
                                        public void run() {
                                            long currentTick = SystemClock.uptimeMillis();
                                            long deltaTicks = currentTick - startTick;
                                            double remainingTime = startTime - (deltaTicks / 1000.0f);
                                            //remainingTime = Math.round(remainingTime * 10) / 10;
                                            remainingTime = Math.round(remainingTime);
                                            if (remainingTime > 0) {
                                                txtTimer.setText(String.valueOf(remainingTime));
                                            } else {
                                                txtTimer.setText("0");
                                                if ((_FragmentBaseListener != null) && (_FragmentBaseListener.getStateMachine() != null))
                                                    _FragmentBaseListener.getStateMachine().requestStatus();
                                            }

                                        }
                                    }
                            );
                        }
                    }
                },
                0,
                200);
    }
}
