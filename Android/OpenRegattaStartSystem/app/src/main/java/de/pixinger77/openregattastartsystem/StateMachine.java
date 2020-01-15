package de.pixinger77.openregattastartsystem;

import android.content.Context;
import android.util.Log;

import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

public class StateMachine {

    // region public enum States
    public enum States
    {
        Unknown,
        Disconnected,
        State_00_Config,
        State_01_Prepare,
        State_02_Countdown4,
        State_03_Countdown1,
        State_04_Started,
        State_05_AbortedAll,
        State_05_AbortedSingle,
        State_05_Aborted,
    }
    // endregion

    private States currentState = States.Unknown;
    private Fragment_Base currentFragment = null;
    private FragmentManager fragmentManager;
    private NetworkHelper networkHelper;
    private Context context;
    private Boolean _RequestStatusInProgress = false;


    public StateMachine(FragmentManager fragmentManager, Context context) {
        Log.d("PIX", "StateMachine CTOR");
        this.context = context;
        this.networkHelper = new NetworkHelper(context);
        this.fragmentManager = fragmentManager;
        this.SetDisconnected();
    }
    public void dispose() {
        this.networkHelper.dispose();
        this.networkHelper = null;
        SetState(States.Unknown); // This will remove the fragment.
    }

    private void SetState(States state)
    {
        if (this.currentState == state)
            return;

        // Shall we remove the current fragment?
        if (this.currentFragment != null)
        {
            Log.d("PIX", "Removed Fragment");
            FragmentTransaction transaction = this.fragmentManager.beginTransaction();
            transaction.remove(this.currentFragment);
            transaction.commit();
            this.currentFragment = null;
            this.currentState = States.Unknown;
        }

        this.currentState = state;

        Fragment_Base fragment = null;
        switch (state)
        {
            case Unknown:
                fragment = null;
                break;
            case Disconnected:
                fragment = new Fragment_Disconnected();
                ((Fragment_Disconnected)fragment).setErrorMessage("");
                break;
            case State_00_Config:
                fragment = new Fragment_State00_Setup();
                break;
            case State_01_Prepare:
                fragment = new Fragment_State01_Prepare();
                break;
            case State_02_Countdown4:
                fragment = new Fragment_State02_Countdown4();
                break;
            case State_03_Countdown1:
                fragment = new Fragment_State03_Countdown1();
                break;
            case State_04_Started:
                fragment = new Fragment_State04_Started();
                break;
            case State_05_AbortedAll:
                fragment = new Fragment_State05_AbortedAll();
                break;
            case State_05_AbortedSingle:
                fragment = new Fragment_State05_AbortedSingle();
                break;
            case State_05_Aborted:
                fragment = new Fragment_State05_Aborted();
                break;
            default:
                Log.e("PIX", "Unknown State in NetworkResponse: " + state.toString());
                return;
        }

        // Prepare new fragment
        if (fragment != null)
        {
            Log.d("PIX", "Added Fragment");
            FragmentTransaction transaction = this.fragmentManager.beginTransaction();
            transaction.add(R.id.fragmentHolder, fragment);
            this.currentFragment = fragment;
            transaction.commit();
        }
    }
    private void SetDisconnected()
    {
        SetState(States.Disconnected);
    }

    public void requestStatus() {
        if (_RequestStatusInProgress)
            return;

        _RequestStatusInProgress = true;

        //networkHelper.requestStatus();
        Log.d("PIX","this.networkHelper.requestStatus");
        this.networkHelper.RequestStatus(new NetworkHelper.IRequestResultStatus() {
            @Override
            public void OnRequestSucceeded(NetworkHelper.Status status) {
                SetState(status.getState());
                if (currentFragment != null) {
                    currentFragment.UpdateStatus(status);
                }
                _RequestStatusInProgress = false;
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
                _RequestStatusInProgress = false;
            }
        });
    }
    public void requestStart(final ClassFlags classFlag, final int prepareMinutes, final int countdownMinutes) {
        Log.d("PIX","this.networkHelper.requestStart");
        this.networkHelper.RequestStart(classFlag.getValue(), prepareMinutes, countdownMinutes, new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetState(States.State_01_Prepare);
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
    public void requestReset() {
        Log.d("PIX","this.networkHelper.requestReset");
        this.networkHelper.RequestReset(new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetDisconnected();
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
    public void requestEmergency() {
        Log.d("PIX","this.networkHelper.requestEmergency");
        this.networkHelper.RequestEmergency(new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetDisconnected();
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
    public void requestAbort() {
        Log.d("PIX","this.networkHelper.requestAbort");
        this.networkHelper.RequestAbort(new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetDisconnected();
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
    public void requestAbortAll() {
        Log.d("PIX","this.networkHelper.requestAbortAll");
        this.networkHelper.RequestAbortAll(new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetDisconnected();
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
    public void requestAbortSingle() {
        Log.d("PIX","this.networkHelper.requestAbortSingle");
        this.networkHelper.RequestAbortSingle(new NetworkHelper.IRequestResult() {
            @Override
            public void OnRequestSucceeded() {
                SetDisconnected();
            }

            @Override
            public void OnRequestFailed(String message) {
                SetDisconnected();
                if (currentFragment != null) {
                    ((Fragment_Disconnected)currentFragment).setErrorMessage(message);
                }
            }
        });
    }
}
