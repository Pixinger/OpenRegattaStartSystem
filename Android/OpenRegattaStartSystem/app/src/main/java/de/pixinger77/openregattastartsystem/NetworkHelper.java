package de.pixinger77.openregattastartsystem;

import android.content.Context;
import android.os.AsyncTask;
import android.os.StrictMode;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.HttpResponse;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.JsonRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.ExecutionException;

public class NetworkHelper {

    static final String SERVER_NAME = "192.168.4.1";

    // region public interface IRequestResultStatus
    public interface IRequestResultStatus {
        void OnRequestSucceeded(Status status);

        void OnRequestFailed(String message);
    }

    // endregion
    // region public interface IRequestResult
    public interface IRequestResult {
        void OnRequestSucceeded();

        void OnRequestFailed(String message);
    }
    // endregion

    // region public abstract class Status
    public abstract class Status {
        private StateMachine.States state;

        public Status(StateMachine.States state) {
            this.state = state;
        }

        public StateMachine.States getState() {
            return state;
        }
    }
    // endregion
    // region public class StatusPrepare extends Status
    public abstract class StatusTime extends Status {
        private double _time;

        public StatusTime(StateMachine.States state, double time) {
            super(state);
            this._time = time;
        }

        public double getTime() { return _time; }
    }
    // endregion
    // region public class StatusPrepare extends Status
    public abstract class StatusTimeClassFlag extends StatusTime {
        private int _classFlagId;

        public StatusTimeClassFlag(StateMachine.States state, double time, int classFlagId) {
            super(state, time);
            this._classFlagId = classFlagId;
        }

        public int getClassFlagId() { return _classFlagId; }
    }
    // endregion

    // region public class StatusSetup extends Status
    public class StatusSetup extends Status {

        public int classFlagId;
        public int prepareMinutes;
        public int countdownMinutes;

        public StatusSetup(int classFlagId, int prepareMinutes, int countdownMinutes) {
            super(StateMachine.States.State_00_Config);

            this.classFlagId = classFlagId;
            this.prepareMinutes = prepareMinutes;
            this.countdownMinutes = countdownMinutes;
        }
    }

    // endregion
    // region public class StatusDisconnected extends Status
    public class StatusDisconnected extends Status {
        public StatusDisconnected() {
            super(StateMachine.States.Disconnected);
        }
    }

    // endregion
    // region public class StatusPrepare extends StatusTime
    public class StatusPrepare extends StatusTimeClassFlag {
        public StatusPrepare(double time, int classFlagId) {
            super(StateMachine.States.State_01_Prepare, time, classFlagId);
        }
    }
    // endregion
    // region public class StatusCountdown4 extends StatusTime
    public class StatusCountdown4 extends StatusTimeClassFlag {
        public StatusCountdown4(double time, int classFlagId) {
            super(StateMachine.States.State_02_Countdown4, time, classFlagId);
        }
    }
    // endregion
    // region public class StatusCountdown1 extends StatusTime
    public class StatusCountdown1 extends StatusTimeClassFlag {
        public StatusCountdown1(double time, int classFlagId) {
            super(StateMachine.States.State_03_Countdown1, time, classFlagId);
        }
    }
    // endregion
    // region public class StatusStarted extends StatusTime
    public class StatusStarted extends StatusTime {
        public StatusStarted(double time) {
            super(StateMachine.States.State_04_Started, time);
        }
    }
    // endregion
    // region public class StatusAbortedAll extends StatusTime
    public class StatusAbortedAll extends StatusTime {
        public StatusAbortedAll(double time) {
            super(StateMachine.States.State_05_AbortedAll, time);
        }
    }
    // endregion
    // region public class StatusAbortedSingle extends StatusTime
    public class StatusAbortedSingle extends StatusTime {
        public StatusAbortedSingle(double time) {
            super(StateMachine.States.State_05_AbortedSingle, time);
        }
    }
    // endregion
    // region public class StatusAborted extends StatusTime
    public class StatusAborted extends StatusTime {
        public StatusAborted(double time) {
            super(StateMachine.States.State_05_Aborted, time);
        }
    }
    // endregion

    private Context context;
    private RequestQueue queue;

    public NetworkHelper(Context context) {
        this.context = context;
        this.queue = Volley.newRequestQueue(context);
    }

    public void dispose() {
        queue.stop();
    }

    public void RequestStatus(final IRequestResultStatus iRequestResult) {
        String url = "http://" + SERVER_NAME + "/";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        try {
                            Log.d("PIX", jsonObject.toString());
                            int version = jsonObject.getInt("ver");
                            if (version == 2) {
                                String state = jsonObject.getString("state");
                                if (state.equals("Setup")) {
                                    int classFlagId = jsonObject.getInt("cfid");
                                    int prepareMinutes = jsonObject.getInt("pmin");
                                    int countdownMinutes = jsonObject.getInt("cmin");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusSetup(classFlagId, prepareMinutes, countdownMinutes));
                                } else if (state.equals("Prepare")) {
                                    double time = jsonObject.getDouble("time");
                                    int classFlagId = jsonObject.getInt("cfid");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusPrepare(time, classFlagId));
                                } else if (state.equals("Countdown4")) {
                                    double time = jsonObject.getDouble("time");
                                    int classFlagId = jsonObject.getInt("cfid");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusCountdown4(time, classFlagId));
                                } else if (state.equals("Countdown1")) {
                                    double time = jsonObject.getDouble("time");
                                    int classFlagId = jsonObject.getInt("cfid");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusCountdown1(time, classFlagId));
                                } else if (state.equals("Started")) {
                                    double time = jsonObject.getDouble("time");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusStarted(time));
                                } else if (state.equals("AbortedAll")) {
                                    double time = jsonObject.getDouble("time");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusAbortedAll(time));
                                } else if (state.equals("AbortedSingle")) {
                                    double time = jsonObject.getDouble("time");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusAbortedSingle(time));
                                } else if (state.equals("Aborted")) {
                                    double time = jsonObject.getDouble("time");
                                    iRequestResult.OnRequestSucceeded(new NetworkHelper.StatusAborted(time));
                                } else {
                                    iRequestResult.OnRequestFailed("Unknown state: " + state);
                                }
                            }
                            else
                            {
                                iRequestResult.OnRequestFailed("Invalid network protocol version: " + version);
                            }
                        } catch (JSONException e) {
                            iRequestResult.OnRequestFailed("JSON error: " + e.getMessage());
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }

    public void RequestStart(int classFlagId, int prepareMinutes, int countdownMinutes, final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/Start?cf=" + classFlagId + "&pm=" + prepareMinutes + "&cm=" + countdownMinutes;

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }

    public void RequestReset(final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/Reset";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }

    public void RequestEmergency(final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/Emergency";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }

    public void RequestAbortSingle(final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/AbortSingle";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }

    public void RequestAbortAll(final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/AbortAll";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }
    public void RequestAbort(final IRequestResult iRequestResult) {
        String url = "http://" + SERVER_NAME + "/Abort";

        JsonObjectRequest jsonRequest = new JsonObjectRequest(
                Request.Method.GET,
                url,
                null, // because we use GET here
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject jsonObject) {
                        iRequestResult.OnRequestSucceeded();
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        iRequestResult.OnRequestFailed("JSON error: " + error.getMessage());
                    }
                });

        this.queue.add(jsonRequest);
    }
}
