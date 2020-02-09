package com.yancy.zbarscanutils

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.yancy.zbarqrscanner.QRDecoder
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        PermissionUtil.addPermission(
            this, Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.CAMERA
        ).request(this)
        setContentView(R.layout.activity_main)
    }

    override fun onResume() {
        super.onResume()
        cameraPreview.setPreviewFrameCallback(object : CameraPreview.PreviewFrameCallback {
            override fun onPreviewFrame(data: ByteArray, width: Int, height: Int) {
                QRDecoder.run {
                    val res = scanImage(data, width, height)
                    Log.w("Yancy","res = $res")
                    if (res > 0) {
                        val obtainResult = obtainResult()
                        obtainResult?.forEach {
                            Log.e("Yancy","code = $it")
                        }
                    }

                }
            }
        })
    }
}
