package com.example.streamvideo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private static final String TAG = "VideoStreaming";
    private static final int PERMISSION_REQUEST_CODE = 100;

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;
    private MediaRecorder mediaRecorder;
    private Camera camera;
    private boolean isStreaming = false;

    // Replace with your server URL
    private static final String SERVER_URL = "http://your-server-ip:port/upload";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        surfaceView = findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        // Request permissions
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED ||
                ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED
//                ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.CAMERA, Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    PERMISSION_REQUEST_CODE);
        }
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        startStreaming();
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        // Handle surface changes if needed
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        stopStreaming();
    }

    private void startStreaming() {
        if (isStreaming) {
            return;
        }

        camera = Camera.open();
        camera.setDisplayOrientation(90);
        camera.unlock();

        mediaRecorder = new MediaRecorder();
        mediaRecorder.setCamera(camera);
        mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);
        mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
        mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);
        mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        mediaRecorder.setVideoSize(640, 480);
        mediaRecorder.setVideoFrameRate(30);

        // Use a pipe to stream data to the server
        ExecutorService executorService = Executors.newSingleThreadExecutor();
        executorService.execute(() -> {
            try {
                URL url = new URL(SERVER_URL);
                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                urlConnection.setDoOutput(true);
                urlConnection.setRequestMethod("POST");
                urlConnection.setRequestProperty("Content-Type", "application/octet-stream");

                mediaRecorder.setOutputFile(String.valueOf(urlConnection.getOutputStream()));
                mediaRecorder.setPreviewDisplay(surfaceHolder.getSurface());
                mediaRecorder.prepare();
                mediaRecorder.start();

                isStreaming = true;
                Log.d(TAG, "Streaming started");

            } catch (IOException e) {
                Log.e(TAG, "Error streaming video: " + e.getMessage());
                stopStreaming();
            }
        });
    }

    private void stopStreaming() {
        if (!isStreaming) {
            return;
        }

        try {
            mediaRecorder.stop();
            mediaRecorder.reset();
            mediaRecorder.release();
            camera.lock();
            camera.release();
        } catch (Exception e) {
            Log.e(TAG, "Error stopping streaming: " + e.getMessage());
        }

        isStreaming = false;
        Log.d(TAG, "Streaming stopped");
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQUEST_CODE) {
            boolean allGranted = true;
            for (int result : grantResults) {
                if (result != PackageManager.PERMISSION_GRANTED) {
                    allGranted = false;
                    break;
                }
            }

            if (!allGranted) {
                Toast.makeText(this, "Permissions are required for the app to work", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}