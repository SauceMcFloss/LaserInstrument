package com.example.bdskv2.bluetooth;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.os.Handler;
import android.content.Intent;
import android.content.Context;
import android.os.Handler;
import android.os.Binder;
import android.os.IBinder;
//import android.support.annotation.Nullable;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import com.example.bdskv2.Constants;

public class BleAdapterService extends Service
{
    private BluetoothAdapter bluetooth_adapter;
    private BluetoothGatt bluetooth_gatt;
    private BluetoothManager bluetooth_manager;
    private Handler activity_handler = null;
    private BluetoothDevice device;
    private BluetoothGattDescriptor descriptor;
    private boolean connected = false;
    public boolean alarm_playing = false;
    private final IBinder binder = new LocalBinder();

    // messages sent back to activity
    public static final int GATT_CONNECTED = 1;
    public static final int GATT_DISCONNECT = 2;
    public static final int GATT_SERVICES_DISCOVERED = 3;
    public static final int GATT_CHARACTERISTIC_READ = 4;
    public static final int GATT_CHARACTERISTIC_WRITTEN = 5;
    public static final int GATT_REMOTE_RSSI = 6;
    public static final int MESSAGE = 7;
    public static final int NOTIFICATION_OR_INDICATION_RECEIVED = 8;

    // message parms
    public static final String PARCEL_DESCRIPTOR_UUID = "DESCRIPTOR_UUID";
    public static final String PARCEL_CHARACTERISTIC_UUID = "CHARACTERISTIC_UUID";
    public static final String PARCEL_SERVICE_UUID = "SERVICE_UUID";
    public static final String PARCEL_VALUE = "VALUE";
    public static final String PARCEL_RSSI = "RSSI";
    public static final String PARCEL_TEXT = "TEXT";

    public class LocalBinder extends Binder
    {
        public BleAdapterService getService()
        {
            return BleAdapterService.this;
        }
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return binder;
    }

    @Override
    public boolean onUnbind(Intent intent)
    {
        return super.onUnbind(intent);
    }

    // set activity the will receive the messages
    public void setActivityHandler(Handler handler) {
        activity_handler = handler;
    }

    private void sendConsoleMessage(String text) {
        Message msg = Message.obtain(activity_handler, MESSAGE);
        Bundle data = new Bundle();
        data.putString(PARCEL_TEXT, text);
        msg.setData(data);
        msg.sendToTarget();
    }

    public boolean isConnected()
    {
        return connected;
    }

    @Override
    public void onCreate()
    {
        if (bluetooth_adapter == null)
        {
            bluetooth_manager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
            if (bluetooth_manager == null)
            {
                return;
            }
        }
        bluetooth_adapter = bluetooth_manager.getAdapter();

        if (bluetooth_adapter == null)
        {
            return;
        }
    }

    // connect to the device
    public boolean connect(final String address) {
        if (bluetooth_adapter == null || address == null)
        {
            sendConsoleMessage("connect: bluetooth_adapter = null");
            return false;
        }

        device = bluetooth_adapter.getRemoteDevice(address);
        if (device == null)
        {
            sendConsoleMessage("connect: device = null");
            return false;
        }

        bluetooth_gatt = device.connectGatt(this, false, gatt_callback);
        return true;
    }

    // disconnect from device
    public void disconnect()
    {
        sendConsoleMessage("disconnecting");

        if (bluetooth_adapter == null || bluetooth_gatt == null)
        {
            sendConsoleMessage("disconnect: bluetooth_adapter|bluetooth_gatt null");
            return;
        }

        if (bluetooth_gatt != null)
        {
            bluetooth_gatt.disconnect();
        }
    }

    private final BluetoothGattCallback gatt_callback = new BluetoothGattCallback()
    {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState)
        {
            if (newState == BluetoothProfile.STATE_CONNECTED)
            {
                Log.d(Constants.TAG, "OnConnectionStateChange: status = " + status);
                connected = true;
                Message msg = Message.obtain(activity_handler, GATT_CONNECTED);
                msg.sendToTarget();
            }
            else if (newState == BluetoothProfile.STATE_DISCONNECTED)
            {
                Log.d(Constants.TAG, "OnConnectionStateChange: DISCONNECTED");
                connected = false;
                Message msg = Message.obtain(activity_handler, GATT_DISCONNECT);
                msg.sendToTarget();

                if (bluetooth_gatt != null)
                {
                    Log.d(Constants.TAG, "Closing and destroying BluetoothGatt object");
                    bluetooth_gatt.close();
                    bluetooth_gatt = null;
                }
            }
        }
    };
}
