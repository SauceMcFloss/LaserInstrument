package com.example.bdskv2.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.example.bdskv2.R;

public class MainActivity extends AppCompatActivity {

    Button playbackButton, bluetoothButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        playbackButton = (Button) findViewById(R.id.btnPlayback);
        bluetoothButton = (Button) findViewById(R.id.btnBluetooth);
        playbackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openOfflineActivity();
            }
        });
        bluetoothButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openOnlineActivity();
            }
        });
    }

    public void openOfflineActivity(){
        // change this when recording works
        Intent intent = new Intent(this, ConnectionActivity.class);
        startActivity(intent);
    }

    public void openOnlineActivity(){
        Intent intent = new Intent(this, ConnectionActivity.class);
        startActivity(intent);
    }
}
