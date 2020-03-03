package com.example.laserinstrument_mobileapp;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

public class ConnectRecordActivity extends AppCompatActivity {

    Button ConnectButton, RecordButton, BackButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connectrecord);

        ConnectButton = (Button) findViewById(R.id.button_Connect);
        RecordButton = (Button) findViewById(R.id.button_Record);
        BackButton = (Button) findViewById(R.id.button_Back_ConnectRecord);

        ConnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doConnect();
            }
        });
        RecordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openRecordActivity();
            }
        });
        BackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openMainActivity();
            }
        });
    }

    public void doConnect(){
        // BT connect stuff
    }

    public void openRecordActivity(){
        Intent intent = new Intent(this, RecordActivity.class);
        startActivity(intent);
    }

    public void openMainActivity(){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }
}
