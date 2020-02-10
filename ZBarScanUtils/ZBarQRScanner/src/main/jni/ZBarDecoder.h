//
// Created by 18081333 on 2020/1/14.
//

#ifndef ZBARSCANUTILS_ZBARDECODER_H
#define ZBARSCANUTILS_ZBARDECODER_H

#include <inttypes.h>
#include <assert.h>
#include <zbar.h>
#include <jni.h>
#include <vector>
#include "android/log.h"

#define LOG_TAG "Yancy"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace zbar;

static JavaVM *jvm = NULL;

typedef struct ScanResult {
    int type;
    const char *data;
} ScanResult;

std::vector<ScanResult> results;

#ifdef __cplusplus
extern "C" {
#endif

static inline void throw_exc(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = env->FindClass(name);
    if (cls) {
        env->ThrowNew(cls, msg);
    }
    env->DeleteLocalRef(cls);
}

// TODO 待确认正确性，释放图像数据的，防止内存泄漏，尝试使用 lambda 表达式解决
static void
Image_cleanupByteArray(zbar_image_t *zimg) {
    /*jobject data = static_cast<jobject>(zbar_image_get_userdata(zimg));
    assert(data);

    JNIEnv *env = NULL;
    if (jvm->AttachCurrentThread(&env, NULL))
        return;
    assert(env);
    if (env && data) {
        void *raw = (void *) zbar_image_get_data(zimg);
        assert(raw);
        *//* const image data is unchanged - abort copy back *//*
        env->ReleaseByteArrayElements(static_cast<jbyteArray>(data), static_cast<jbyte *>(raw), JNI_ABORT);
        env->DeleteGlobalRef(data);
        zbar_image_set_userdata(zimg, NULL);
    }*/
}


JNIEXPORT int init(JNIEnv *env, jobject obj);

JNIEXPORT int scanImage(JNIEnv *env, jobject obj,
                        jbyteArray jbytes, jint width, jint height,
                        jint cropX, jint cropY, jint cropWidth, jint cropHeight);

JNIEXPORT jobjectArray obtainResult(JNIEnv *env, jobject obj);
JNIEXPORT jint obtainType(JNIEnv *env, jobject obj);
JNIEXPORT void destroy(JNIEnv *env, jobject obj);

void obtainScanData(const zbar_symbol_t *zsym);


#ifdef __cplusplus
}
#endif

#endif //ZBARSCANUTILS_ZBARDECODER_H
