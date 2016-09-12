package com.arthurpachachura.scanit.util;

import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;

/**
 * OpenCV Polygon
 */
public class Polygon {
    private MatOfPoint corners;

    public Polygon() {
        this.corners = null;
    }

    public Polygon(MatOfPoint corners) {
        this.corners = corners;
    }

    public MatOfPoint getCornersMat() {
        return corners;
    }

    public Point[] getCornersArray() {
        return corners.toArray();
    }
}
