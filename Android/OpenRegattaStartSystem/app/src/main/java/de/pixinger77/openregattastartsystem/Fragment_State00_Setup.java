package de.pixinger77.openregattastartsystem;


import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Spinner;


public class Fragment_State00_Setup extends Fragment_Base {

    private int classFlagId = 0;
    private int prepareMinutes = 0;
    private int countdownMinutes = 0;

    private Spinner spinnerClassFlag;
    private Spinner spinnerPrepareMinutes;
    private Spinner spinnerCountdownMinutes;

    public Fragment_State00_Setup() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_state00__setup, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        this.spinnerClassFlag = view.findViewById(R.id.spinnerlassFlag);
        this.spinnerPrepareMinutes= view.findViewById(R.id.spinnerPrepareTime);
        this.spinnerCountdownMinutes = view.findViewById(R.id.spinnerCountdownTime);
        UpdateGUI();

        Button btnStart = view.findViewById(R.id.btnStart);
        btnStart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                int cf = spinnerClassFlag.getSelectedItemPosition();
                int pm = spinnerPrepareMinutes.getSelectedItemPosition() + 1;
                int cm = spinnerCountdownMinutes.getSelectedItemPosition() + 2;
                _FragmentBaseListener.getStateMachine().requestStart(cf, pm, cm);
            }
        });
        super.onViewCreated(view, savedInstanceState);
    }

    private void UpdateGUI() {
        if (this.spinnerClassFlag != null)
            this.spinnerClassFlag.setSelection(this.classFlagId);
        if (this.spinnerPrepareMinutes != null)
            this.spinnerPrepareMinutes.setSelection(this.prepareMinutes - 1);
        if (this.spinnerCountdownMinutes != null)
            this.spinnerCountdownMinutes.setSelection(this.countdownMinutes - 2);
    }
    private void UpdateGUI(int classFlagId, int prepareMinutes, int countdownMinutes) {
        this.classFlagId = classFlagId;
        this.prepareMinutes = prepareMinutes;
        this.countdownMinutes = countdownMinutes;
        UpdateGUI();
    }

    @Override
    protected void OnUpdateStatus(NetworkHelper.Status status) {
        NetworkHelper.StatusSetup castedStatus = (NetworkHelper.StatusSetup)status; // Casted to underlying type.
        UpdateGUI(castedStatus.classFlagId, castedStatus.prepareMinutes, castedStatus.countdownMinutes);
    }
}
