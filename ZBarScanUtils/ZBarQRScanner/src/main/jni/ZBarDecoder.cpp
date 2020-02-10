//
// Created by XuYanjun on 2020/1/14.
//

#include "ZBarDecoder.h"

zbar_image_scanner_t *zscn = 0;

static JNINativeMethod gMethods[] = {
        {"init",         "()I",                   (void *) init},
        {"scanImage",    "([BIIIIII)I",           (void *) scanImage},
        {"obtainResult", "()[Ljava/lang/String;", (void *) obtainResult},
        {"obtainType",   "()I",                   (void *) obtainType},
        {"destroy",      "()V",                   (void *) destroy},
};

const char *JNI_NATIVE_INTERFACE_CLASS = "com/yancy/zbarqrscanner/QRDecoder";

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
        return JNI_ERR;
    }
    jclass cls = env->FindClass(JNI_NATIVE_INTERFACE_CLASS);
    if (cls == NULL) {
        return JNI_ERR;
    }
    jint nRes = env->RegisterNatives(cls, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
    if (nRes < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
        return;
    }
    jclass cls = env->FindClass(JNI_NATIVE_INTERFACE_CLASS);
    if (cls == NULL) {
        return;
    }
    env->UnregisterNatives(cls);
}

int init(JNIEnv *env, jobject obj) {
    //初始化 ImageScanner
    zscn = zbar_image_scanner_create();
    if (!zscn) {
        throw_exc(env, "java/lang/OutOfMemoryError", NULL);
        return (0);
    }
    zbar_image_scanner_set_config(zscn, ZBAR_NONE, ZBAR_CFG_X_DENSITY, 3);
    zbar_image_scanner_set_config(zscn, ZBAR_NONE, ZBAR_CFG_Y_DENSITY, 3);

    return 1;
}

#define VALIDATE_CROP(u, m) \
    if((u) < 0) {           \
        (m) += (u);         \
        (u) = 0;            \
    }

int scanImage(JNIEnv *env, jobject obj,
              jbyteArray jbytes, jint width, jint height,
              jint cropX, jint cropY, jint cropWidth, jint cropHeight) {
    results.clear();
    // 创建Image
    zbar_image_t *zimg = zbar_image_create();
    if (!zimg) {
        throw_exc(env, "java/lang/OutOfMemoryError", NULL);
        return (-1);
    }

    // 设置Image宽高
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    zbar_image_set_size(zimg, static_cast<unsigned int>(width), static_cast<unsigned int>(height));

    // 设置裁剪区域
    if (!(cropX == 0 && cropY == 0 && cropWidth == width && cropHeight == height)) {
        VALIDATE_CROP(cropX,cropWidth)
        VALIDATE_CROP(cropY,cropHeight)
        zbar_image_set_crop(zimg, cropX, cropY, cropWidth, cropHeight);
    }

    // 设置格式，这里固定死为 Y800，即灰度图
    const char *format = "Y800";
    uint32_t fourcc = 0;
    for (int i = 0; i < 4; ++i) {
        fourcc |= ((uint32_t) format[i]) << (8 * i);
    }
    zbar_image_set_format(zimg, fourcc);

    // 设置数据
    jbyte *raw = NULL;
    int rawlen = 0;
    if (jbytes) {
        raw = env->GetByteArrayElements(jbytes, NULL);
        if (!raw)
            return -1;
        rawlen = env->GetArrayLength(jbytes);
    }
    zbar_image_set_data(zimg, raw, static_cast<unsigned long>(rawlen), Image_cleanupByteArray);
    //zbar_image_set_userdata(zimg, env->NewGlobalRef(jbytes));

    // 设置为数据，开始检测
    if (zscn != 0) {
        int n = zbar_scan_image(zscn, zimg);
        if (n < 0)
            throw_exc(env, "java/lang/UnsupportedOperationException",
                      "unsupported image format");

        //获取数据
        const zbar_symbol_set_t *zsyms =
                zbar_image_scanner_get_results(zscn);
        if (zsyms) {
            zbar_symbol_set_ref(zsyms, 1);
            const zbar_symbol_t *zsym = zbar_symbol_set_first_symbol(zsyms);
            if (zsym) {
                obtainScanData(zsym);
                //释放内存数据
                zbar_symbol_ref(zsym, -1);
            }
        }
        return results.size();
    }
    return -1;
}

void obtainScanData(const zbar_symbol_t *zsym) {
    if (zsym) {
        ScanResult result;
        zbar_symbol_ref(zsym, 1);
        result.type = zbar_symbol_get_type(zsym);
        result.data = zbar_symbol_get_data(zsym);
        results.push_back(result);
        obtainScanData(zbar_symbol_next(zsym));
    }
}

jobjectArray obtainResult(JNIEnv *env, jobject obj) {
    if (!results.empty()) {
        jclass cls = env->FindClass("java/lang/String");
        jobjectArray array = env->NewObjectArray(results.size(), cls, nullptr);
        for (int i = 0; i < results.size(); i++) {
            const char *data = results[i].data;
            jstring res = env->NewStringUTF(data);
            env->SetObjectArrayElement(array, i, res);
        }
        return array;
    }
    return nullptr;
}

jint obtainType(JNIEnv *env, jobject obj) {
    int type = 0;
    if (!results.empty()) {
        for (ScanResult result:results) {
            type = result.type;
            if (type != 0) {
                break;
            }
        }
    }
    return type;
}

void destroy(JNIEnv *env, jobject obj) {
    if (zscn) {
        zbar_image_scanner_destroy(zscn);
    }
}
