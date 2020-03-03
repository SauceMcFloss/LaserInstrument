package com.example.laserinstrument_mobileapp;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

public class RecordActivity extends AppCompatActivity {

    Button BeginRecordingButton, BackButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);

        BeginRecordingButton = (Button) findViewById(R.id.button_BeginRecording);
        BackButton = (Button) findViewById(R.id.button_Back_Record);

        BeginRecordingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doBeginRecord();
            }
        });

        BeginRecordingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openConnectRecordActivity();
            }
        });
    }

    public void doBeginRecord(){
        // Begin recording stuff
    }

    public void openConnectRecordActivity(){
        Intent intent = new Intent(this, ConnectRecordActivity.class);
        startActivity(intent);
    }
}
