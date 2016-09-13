package com.arthurpachachura.scanit;

import com.arthurpachachura.scanit.util.Polygon;

import org.opencv.core.Mat;

/**
 * Native code APIs
 */
final class NativeApi {

    static FrameAnalysis analyzeFrame(Mat rgba, Mat gray) {
        analyzeFrameThreshold(rgba.getNativeObjAddr(), gray.getNativeObjAddr());
        return new FrameAnalysis(new Polygon());
    }

    private native static void analyzeFrameCanny(long matAddrRgba, long matAddrGray);

    private native static void analyzeFrameThreshold(long matAddrRgba, long matAddrGray);

    static class FrameAnalysis {
        Polygon view;

        FrameAnalysis(Polygon view) {
            this.view = view;
        }

        public Polygon getView() {
            return view;
        }
    }
}
