package com.yancy.zbarqrscanner

/**
 * 说明：
 * @author  Yancy
 * @date 2020/1/14
 */
object QRDecoder {
    init {
        System.loadLibrary("QRDecoder")
        init()
    }

    private external fun init():Int

    external fun scanImage(data: ByteArray, width: Int, height: Int): Int

    external fun obtainResult(): Array<String>?

    external fun obtainType(): Int

    external fun destroy()
}