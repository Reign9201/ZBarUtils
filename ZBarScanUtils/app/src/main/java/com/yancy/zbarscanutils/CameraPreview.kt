package com.yancy.zbarscanutils

import android.content.Context
import android.graphics.PixelFormat
import android.hardware.Camera
import android.os.Build
import android.os.Handler
import android.util.AttributeSet
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import java.lang.reflect.Method

/**
 * 说明：
 * @author  Yancy
 * @date 2020/1/13
 */
class CameraPreview : SurfaceView, SurfaceHolder.Callback, Camera.PreviewCallback {
    /**
     * 相机预览宽
     */
    private var cameraWidth = 1280

    /**
     * 相机预览高
     */
    private var cameraHeight = 720

    /**
     * 相机
     */
    private var mCamera: Camera? = null

    /**
     * 相机参数
     */
    private var parameters: Camera.Parameters? = null

    /**
     * 用于处理相机对焦的Handler
     */
    private lateinit var focusHandler: Handler

    /**
     * 相机对焦回调
     */
    private val cb = Camera.AutoFocusCallback { _, _ ->
       /* if (!success) {
            // 对焦失败的时候重新触发对焦
            focusHandler.postDelayed(autoFocusCallback, 1000)
        }*/
        focusHandler.postDelayed(autoFocusCallback, 1000)
    }
    private val autoFocusCallback: Runnable = Runnable {
        mCamera?.autoFocus(cb)
    }


    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs) {
        initParameter(cameraWidth, cameraHeight)
    }


    constructor(context: Context?, width: Int, height: Int) : super(context) {
        initParameter(width, height)
    }

    /**
     * 初始化一些参数
     *
     * @param width 相机预览像素的宽
     * @param height 相机预览像素的高
     */
    private fun initParameter(width: Int, height: Int) {
        focusHandler = Handler()
        this.cameraWidth = width
        this.cameraHeight = height
        holder.apply {
            setFormat(PixelFormat.TRANSPARENT)
            addCallback(this@CameraPreview)
            setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS)
        }
    }

    /**
     * 对焦，外部调用
     */
    fun cameraFocus() {
        mCamera?.autoFocus(cb)
    }

    /**
     * 初始化相机参数
     */
    private fun initCamera() {
        if (parameters == null) {
            parameters = mCamera!!.parameters
        }


        parameters!!.focusMode = Camera.Parameters.FOCUS_MODE_AUTO
        parameters!!.setPreviewSize(cameraWidth, cameraHeight)
        setDisplay(parameters!!, mCamera!!)
        mCamera!!.parameters = parameters!!
        mCamera!!.addCallbackBuffer(ByteArray(cameraWidth * cameraHeight * 3 / 2))
        mCamera!!.setPreviewCallbackWithBuffer(this)
        mCamera!!.startPreview()
        mCamera!!.autoFocus(cb)

    }

    /**
     * 设置预览方法
     */
    private fun setDisplay(mParameters: Camera.Parameters, mCamera: Camera) {
        if (Integer.parseInt(Build.VERSION.SDK) >= 8) {
            setDisplayOrientation(mCamera, 90)
        } else {
            mParameters.setRotation(90)
        }
    }

    private fun setDisplayOrientation(mCamera: Camera, i: Int) {
        val downPolymorphic: Method
        try {
            downPolymorphic =
                mCamera.javaClass.getMethod("setDisplayOrientation", Int::class.javaPrimitiveType)
            downPolymorphic.invoke(mCamera, i)
        } catch (ignored: Exception) {

        }
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        try {
            mCamera = Camera.open(0)
            mCamera?.setDisplayOrientation(90)
            mCamera?.setPreviewDisplay(holder)
        } catch (e: Exception) {
            Log.e("Yancy", "open camera error.")
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        initCamera()
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        release()
    }

    /**
     * 不使用的时候需要释放相机相关参数
     */
    @Synchronized
    fun release() {
        mCamera?.apply {
            setPreviewCallback(null)
            stopPreview()
            release()
            mCamera = null
        }
        focusHandler.removeCallbacks(autoFocusCallback)
    }


    override fun onPreviewFrame(data: ByteArray?, camera: Camera?) {
        if (null == data) {
            camera?.addCallbackBuffer(ByteArray(width * height * 6))
        } else {
            camera?.addCallbackBuffer(data)
        }
        callback?.onPreviewFrame(data!!, cameraWidth, cameraHeight)
    }

    interface PreviewFrameCallback {
        /**
         * 实时预览数据回调
         *
         * @param data   nv21格式的预览数据
         * @param width 相机分辨率宽
         * @param height 相机分辨率高
         */
        fun onPreviewFrame(data: ByteArray, width: Int, height: Int)
    }

    private var callback: PreviewFrameCallback? = null

    fun setPreviewFrameCallback(callback: PreviewFrameCallback) {
        this.callback = callback
    }
}