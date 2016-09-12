package com.arthurpachachura.scanit;

import com.arthurpachachura.scanit.util.Polygon;

import org.opencv.core.Mat;

/**
 * Native code APIs
 */
final class NativeApi {

    static FrameAnalysis analyzeFrame(Mat rgba, Mat gray) {
        analyzeFrame(rgba.getNativeObjAddr(), gray.getNativeObjAddr());
        return new FrameAnalysis(new Polygon());
    }

    private native static void analyzeFrame(long matAddrRgba, long matAddrGray);

    static class FrameAnalysis {
        Polygon view;

        FrameAnalysis(Polygon view) {
            this.view = view;
        }

        public Polygon getView() {
            return view;
        }
    }

    //public native String stringFromJNI();
    //public native String validate(long matAddrGr, long matAddrRgba);
}
