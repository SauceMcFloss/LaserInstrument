package com.example.laserinstrument_mobileapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button ConnectRecordButton, BrowseButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ConnectRecordButton = (Button) findViewById(R.id.button_ConnectRecord);
        BrowseButton = (Button) findViewById(R.id.button_Browse);

        ConnectRecordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openConnectRecordActivity();
            }
        });
        BrowseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openBrowseActivity();
            }
        });
    }

    public void openConnectRecordActivity(){
        Intent intent = new Intent(this, ConnectRecordActivity.class);
        startActivity(intent);
    }

    public void openBrowseActivity(){
        Intent intent = new Intent(this, BrowseActivity.class);
        startActivity(intent);
    }
}
