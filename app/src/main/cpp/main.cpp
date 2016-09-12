#include <jni.h>
#include <string>
#include <android/log.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// Android log function wrappers
static const char* kTAG = "ScanIt Library";

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

float angle_3(Point a, Point b, Point c) {
    Point ab = {b.x - a.x, b.y - a.y};
    Point cb = {b.x - c.x, b.y - c.y};

    float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
    float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

    float alpha = atan2(cross, dot);

    return (int) floor(alpha * 180. / CV_PI + 0.5);
}

extern "C" {
void
Java_com_arthurpachachura_scanit_NativeApi_analyzeFrame__JJ(JNIEnv *env, jclass type,
                                                            jlong matAddrRgba, jlong matAddrGray) {
    Mat *rgba = (Mat *) matAddrRgba;
    Mat *gray = (Mat *) matAddrGray;

    //Downsample and upsample to remove noise
    resize(*gray, *gray, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(*gray, *gray, Size(), 2, 2, INTER_LINEAR);

    //Canny edge filter
    Canny(*gray, *gray, 800, 850, 5, true);

    //Connect nearby edges using a morphological transformation
    Size kernalSize(5, 5);
    Mat element = getStructuringElement(MORPH_RECT, kernalSize, Point(1, 1));
    morphologyEx(*gray, *gray, MORPH_CLOSE, element);

    //Find contours
    vector<vector<Point>> contours;
    int vertices;
    findContours(*gray, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    vector<Point> approx;   //polygon approximation


    for (int i = 0; i < contours.size(); i++) {
        //Approximate contour to polygon
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.01, true);

        //Count vertices
        vertices = (int) approx.size();
        if (vertices >= 4 && (vertices <= 6)) {
            //Attempt to simplify approximation into quadrilateral
            /*vector<double> cos;     //angles of each vertex
            for (int j=2; j<vertices+1; j++)
                cos.push_back(angle_3(approx[j%vertices], approx[j-2], approx[j-1]));
            sort(cos.begin(), cos.end());
            double mincos = cos.front();
            double maxcos = cos.back();*/

            if (vertices == 4) {
                Rect r = boundingRect(contours[i]);
                double ratio = abs(1.0 - (double) r.width / (double) r.height);

                //Draw resulting contour on screen
                line(*rgba, approx.at(0), approx.at(1), cvScalar(0, 255, 0), 3);
                line(*rgba, approx.at(1), approx.at(2), cvScalar(0, 255, 0), 3);
                line(*rgba, approx.at(2), approx.at(3), cvScalar(0, 255, 0), 3);
                line(*rgba, approx.at(3), approx.at(0), cvScalar(0, 255, 0), 3);
            }
        }
    }

    return;
}

    jstring
    Java_com_arthurpachachura_scanit_NativeApi_validate
            (JNIEnv *env, jobject instance,
             jlong matAddrGr, jlong matAddrRgba) {
        cv:
        Rect();
        cv::Mat();
        std::string hello2 = "Hello from validate";
        return env->NewStringUTF(hello2.c_str());
    }

    jstring
    Java_com_arthurpachachura_scanit_NativeApi_stringFromJNI
            (JNIEnv *env, jobject instance) {
        std::string hello = "Hola from C++";
        LOGI("All good!", __LINE__);
        return env->NewStringUTF(hello.c_str());
    }
}