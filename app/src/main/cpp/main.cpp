#include <jni.h>
#include <string>
#include <android/log.h>
#include "opencv2/core.hpp"

// Android log function wrappers
static const char* kTAG = "ScanIt Library";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

extern "C" {
    jstring
    Java_com_arthurpachachura_scanit_CameraTestActivity_validate
            (JNIEnv *env, jobject instance,
                                                                 jlong matAddrGr, jlong matAddrRgba) {
        cv::Rect();
        cv::Mat();
        std::string hello2 = "Hello from validate";
        return env->NewStringUTF(hello2.c_str());
    }

    jstring
    Java_com_arthurpachachura_scanit_CameraTestActivity_stringFromJNI(JNIEnv *env, jobject instance) {
        std::string hello = "Hola from C++";
        LOGI("All good!", __LINE__);
        return env->NewStringUTF(hello.c_str());
    }
}