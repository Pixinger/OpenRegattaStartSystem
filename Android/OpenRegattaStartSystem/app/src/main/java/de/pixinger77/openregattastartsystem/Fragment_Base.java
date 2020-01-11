package de.pixinger77.openregattastartsystem;

import android.content.Context;

import androidx.fragment.app.Fragment;

public abstract class Fragment_Base extends Fragment {

    public interface OnFragmentBaseListener {
        StateMachine getStateMachine();
    }
    protected OnFragmentBaseListener _FragmentBaseListener = null;

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);

        if (context instanceof OnFragmentBaseListener) {
            _FragmentBaseListener = (OnFragmentBaseListener)context;
        } else {
            throw new RuntimeException(context.toString() + " must implement OnFragmentBaseListener");
        }
    }
    @Override
    public void onDetach() {
        _FragmentBaseListener = null;
        super.onDetach();
    }

    public void UpdateStatus(NetworkHelper.Status status) {
        OnUpdateStatus(status);
    }
    protected abstract void OnUpdateStatus(NetworkHelper.Status status);
}
