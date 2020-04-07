package com.example.bdskv2.ui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import com.example.bdskv2.Constants;
import com.example.bdskv2.R;
import com.example.bdskv2.bluetooth.BleAdapterService;
import java.util.Timer;

public class PeripheralControlActivity extends Activity
{
    public static final String EXTRA_NAME = "name";
    public static final String EXTRA_ID = "id";

    private String device_name;
    private String device_address;
    private Timer mTimer;
    private boolean sound_alarm_on_disconnect = false;
    private int alert_level;
    private boolean back_requested = false;
    private boolean share_with_server = false;
    private Switch share_switch;

    private BleAdapterService bluetooth_le_adapter;
    private final ServiceConnection service_connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            bluetooth_le_adapter = ((BleAdapterService.LocalBinder) service).getService();
            bluetooth_le_adapter.setActivityHandler(message_handler);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            bluetooth_le_adapter = null;
        }
    };

    @SuppressLint("HandlerLeak")
    private Handler message_handler = new Handler()
    {
        @Override
        public void handleMessage (Message msg)
        {
            Bundle bundle;
            String service_uuid = "";
            String characteristic_uuid = "";
            byte[] b = null;

            // message handling logic
            switch (msg.what) {
                case BleAdapterService.MESSAGE:
                    bundle = msg.getData();
                    String text = bundle.getString(BleAdapterService.PARCEL_TEXT);
                    showMsg(text);
                    break;

                case BleAdapterService.GATT_CONNECTED:
                    PeripheralControlActivity.this.findViewById(R.id.connectButton).setEnabled(false);
                    // we are connected
                    showMsg("CONNECTED");
                    break;

                case BleAdapterService.GATT_DISCONNECT:
                    PeripheralControlActivity.this.findViewById(R.id.connectButton).setEnabled(true);
                    // we are disconnected
                    showMsg("DISCONNECTED");

                    if (back_requested)
                    {
                        PeripheralControlActivity.this.finish();
                    }
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_peripheral_control);

        // read intent data
        final Intent intent = getIntent();
        device_name = intent.getStringExtra(EXTRA_NAME);
        device_address = intent.getStringExtra(EXTRA_ID);

        // show the device name
        ((TextView) this.findViewById(R.id.nameTextView)).setText("Device : " + device_name + " [" + device_address + "]");

        // disable the noise button
        ((Button) PeripheralControlActivity.this.findViewById(R.id.noiseButton)).setEnabled(false);

        // disable the LOW/MID/HIGH alert level selection buttons
        ((Button) this.findViewById(R.id.lowButton)).setEnabled(false);
        ((Button) this.findViewById(R.id.midButton)).setEnabled(false);
        ((Button) this.findViewById(R.id.highButton)).setEnabled(false);

        share_switch = (Switch) this.findViewById(R.id.switch1);
        share_switch.setEnabled(false);
        share_switch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO
            }
        });

        // connect to the Bluetooth adapter service
        Intent gattServiceIntent = new Intent(this, BleAdapterService.class);
        bindService(gattServiceIntent, service_connection, BIND_AUTO_CREATE);
        showMsg("READY");
    }

    private void showMsg(final String msg)
    {
        Log.d(Constants.TAG, msg);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ((TextView) findViewById(R.id.msgTextView)).setText(msg);
            }
        });
    }

    @Override
    protected  void onDestroy()
    {
        super.onDestroy();
        unbindService(service_connection);
        bluetooth_le_adapter = null;
    }

    public void onLow(View view) {
    }
    public void onMid(View view) {
    }
    public void onHigh(View view) {
    }
    public void onNoise(View view) {
    }

    public void onConnect(View view)
    {
        showMsg("OnConnect");

        if (bluetooth_le_adapter != null)
        {
            if (bluetooth_le_adapter.connect(device_address))
            {
                ((Button) PeripheralControlActivity.this.findViewById(R.id.connectButton)).setEnabled(false);

                // TODO
            }
            else {
                showMsg("OnConnect: failed to connect");
            }
        }
        else {
            showMsg("OnConnect: bluetooth_le_adapter = null");
        }
    }

    public void onBackPressed()
    {
        Log.d(Constants.TAG, "OnBackPressed");
        back_requested = true;

        if (bluetooth_le_adapter.isConnected())
        {
            try
            {
                bluetooth_le_adapter.disconnect();
            } catch (Exception e) {

            }
        }
        else
        {
            finish();
        }
    }

}
