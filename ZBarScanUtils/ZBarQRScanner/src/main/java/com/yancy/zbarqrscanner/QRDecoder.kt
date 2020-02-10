package com.yancy.zbarqrscanner

/**
 * 说明：ZBar识别
 * @author  Yancy
 * @date 2020/1/14
 */
object QRDecoder {
    init {
        System.loadLibrary("QRDecoder")
        init()
    }

    private external fun init(): Int

    /**
     * 扫描操作
     * @param data       图像数据
     * @param width      图像的宽
     * @param height     图像的高
     * @param cropX      裁剪区域起始横坐标
     * @param cropY      裁剪区域起始纵坐标
     * @param cropWidth  裁剪区域的宽
     * @param cropHeight 裁剪区域的高
     *
     * @return 返回值大于 0 则表示有识别结果
     */
    external fun scanImage(
        data: ByteArray,
        width: Int,
        height: Int,
        cropX: Int,
        cropY: Int,
        cropWidth: Int,
        cropHeight: Int
    ): Int


    fun scanImage(data: ByteArray, width: Int, height: Int): Int {
        return scanImage(data, width, height, 0, 0, width, height)
    }

    /**
     * 获取扫码识别后的结果
     *
     * @return 识别成功后的结果数组列表
     */
    external fun obtainResult(): Array<String>?

    /**
     * 获取扫码的编码格式
     */
    external fun obtainType(): Int

    /**
     * 释放操作
     */
    external fun destroy()
}