package com.arthurpachachura.scanit;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import org.opencv.android.OpenCVLoader;

public class CameraTestActivity extends AppCompatActivity {

    static {
        System.loadLibrary("scanit-lib");
        System.loadLibrary("opencv_java3");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cameratest);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        if (!OpenCVLoader.initDebug()) {
            Log.e("ScanIt/OpenCVLoader", "Failed to load");
        } else {
            Log.d("ScanIt/OpenCVLoader", "Loaded successfully.");
            Log.d("ScanIt/OpenCVLoader", validate(0L, 0L));
        }
    }

    public native String stringFromJNI();
    public native String validate(long matAddrGr, long matAddrRgba);
}
