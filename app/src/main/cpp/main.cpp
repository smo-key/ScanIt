#include <jni.h>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

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

#define MIN_THRESHOLD_CANNY 125
#define THRESHOLD_CANNY 150
#define APERTURE_CANNY 3
#define EPLISON_APPROX_TOLERANCE_FACTOR 0.02
#define MORPH_KERNEL_SIZE 5
#define DILATE_KERNEL_SIZE 5
#define MIN_AREA 16

extern "C" void
Java_com_arthurpachachura_scanit_NativeApi_analyzeFrameThreshold(JNIEnv *env, jclass type,
                                                                 jlong matAddrRgba,
                                                                 jlong matAddrGray) {
    Mat *rgba = (Mat *) matAddrRgba;
    Mat *gray = (Mat *) matAddrGray;

    //Downsample and upsample to remove noise
    resize(*gray, *gray, Size(), 0.25, 0.25, INTER_LINEAR);
    resize(*gray, *gray, Size(), 4, 4, INTER_LINEAR);

    //Threshold the image into white and black
    threshold(*gray, *gray, 0, 255, THRESH_BINARY | THRESH_OTSU);

    //Find edges
    Canny(*gray, *gray, 150, 200, APERTURE_CANNY, true);
}

extern "C" void
Java_com_arthurpachachura_scanit_NativeApi_analyzeFrameCanny__JJ(JNIEnv *env, jclass type,
                                                            jlong matAddrRgba, jlong matAddrGray) {
    Mat *rgba = (Mat *) matAddrRgba;
    Mat *gray = (Mat *) matAddrGray;

    //Downsample and upsample to remove noise
    resize(*gray, *gray, Size(), 0.5, 0.5, INTER_CUBIC);
    resize(*gray, *gray, Size(), 1. / 0.5, 1. / 0.5, INTER_CUBIC);

    //Canny edge filter
    Canny(*gray, *gray, MIN_THRESHOLD_CANNY, THRESHOLD_CANNY, APERTURE_CANNY, true);

    //Connect nearby edges using a morphological transformation
    Size morphKernel(MORPH_KERNEL_SIZE, MORPH_KERNEL_SIZE);
    Mat morphElement = getStructuringElement(MORPH_RECT, morphKernel, Point(1, 1));
    morphologyEx(*gray, *gray, MORPH_CLOSE, morphElement);

    //Dilate the image to expand all lines
    Size dilateKernel(DILATE_KERNEL_SIZE, DILATE_KERNEL_SIZE);
    Mat dilateElement = getStructuringElement(MORPH_RECT, dilateKernel, Point(1, 1));
    dilate(*gray, *gray, dilateElement);

    //Find contours
    vector<vector<Point>> contours;
    int vertices;
    double area;
    double minArea = (gray->cols * gray->rows) / MIN_AREA;
    findContours(*gray, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    vector<Point> approx;   //polygon approximation

    for (int i = 0; i < contours.size(); i++) {
        //Approximate contour to polygon
        approxPolyDP(Mat(contours[i]), approx,
                     arcLength(Mat(contours[i]), true) * EPLISON_APPROX_TOLERANCE_FACTOR, true);

        //Count vertices
        vertices = (int) approx.size();
        area = contourArea(approx);
        if (vertices >= 4 && (vertices <= 6) && (area > minArea)) {
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
                line(*gray, approx.at(0), approx.at(1), cvScalar(255), 5);
                line(*gray, approx.at(1), approx.at(2), cvScalar(255), 5);
                line(*gray, approx.at(2), approx.at(3), cvScalar(255), 5);
                line(*gray, approx.at(3), approx.at(0), cvScalar(255), 5);
            }
        }
        else {
            for (int j = 0; j < vertices + 1; j++) {
                line(*gray, approx.at((unsigned long) (j % vertices)), approx.at(
                        (unsigned long) ((j + 1) % vertices)), cvScalar(180), 2);
            }
        }
    }

    return;
}