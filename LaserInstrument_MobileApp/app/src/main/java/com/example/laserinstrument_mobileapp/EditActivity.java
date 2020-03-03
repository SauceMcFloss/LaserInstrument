package com.example.laserinstrument_mobileapp;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

public class EditActivity extends AppCompatActivity {

    Button PlayButton, ModifyButton, SaveButton, BackButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit);

        PlayButton = (Button) findViewById(R.id.button_Play);
        ModifyButton = (Button) findViewById(R.id.button_Modify);
        SaveButton = (Button) findViewById(R.id.button_Save);
        BackButton = (Button) findViewById(R.id.button_Back_Edit);

        PlayButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doPlay();
            }
        });
        ModifyButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doModify();
            }
        });
        SaveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doSave();
            }
        });
        BackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openBrowseActivity();
            }
        });
    }

    public void doPlay(){
        // Play stuff
    }

    public void doModify(){
        // Modify stuff
    }

    public void doSave(){
        // Save stuff
    }

    public void openBrowseActivity(){
        Intent intent = new Intent(this, BrowseActivity.class);
        startActivity(intent);
    }
}
