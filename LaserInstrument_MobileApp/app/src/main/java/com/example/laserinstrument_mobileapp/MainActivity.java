package com.example.laserinstrument_mobileapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button OfflineButton, OnlineButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        OfflineButton = (Button) findViewById(R.id.button_mainOffline);
        OnlineButton = (Button) findViewById(R.id.button_mainOnline);
        OfflineButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openOfflineActivity();
            }
        });
        OnlineButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openOnlineActivity();
            }
        });
    }

    public void openOfflineActivity(){
        Intent intent = new Intent(this, OfflineActivity.class);
        startActivity(intent);
    }

    public void openOnlineActivity(){
        Intent intent = new Intent(this, OnlineActivity.class);
        startActivity(intent);
    }
}
