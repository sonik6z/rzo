#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

//определениее фигуры по контуру
string detectShape(const vector<Point>& approx, double area, double perimeter) {
    int vertices = (int)approx.size();

    if (vertices == 3) {
        return "Triangle";
    }
    else if (vertices == 4) {
        Rect rect = boundingRect(approx);
        double rectArea = rect.width * rect.height;
        double areaRatio = area / rectArea;
        if (areaRatio > 0.85) {
            return "Rectangle";
        }
        else {
            return "Quadrilateral";
        }
    }
    else if (vertices > 4) {
        Point2f center;
        float radius;
        minEnclosingCircle(approx, center, radius);
        double circleArea = CV_PI * radius * radius;
        double areaRatio = area / circleArea;
        if (areaRatio > 0.80) {
            return "Circle";
        }
        else {
            return "Polygon";
        }
    }
    else {
        return "Unknown";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    string filename = "rzo4.png";
    Mat img = imread(filename);
    if (img.empty()) {
        cerr << "Ошибка: не удалось загрузить изображение " << filename << endl;
        return -1;
    }

    Mat original = img.clone();

    Mat gray, blurred, edges;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    Canny(blurred, edges, 50, 150);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area < 500) continue;

        double perimeter = arcLength(contours[i], true);
        vector<Point> approx;
        approxPolyDP(contours[i], approx, 0.02 * perimeter, true);

        string shapeName = detectShape(approx, area, perimeter);

        Moments m = moments(contours[i]);
        if (m.m00 == 0) continue; 
        Point2f center(m.m10 / m.m00, m.m01 / m.m00);

        string text = shapeName + " P=" + format("%.2f", perimeter);
        int fontFace = FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.5;
        int thickness = 1;
        int baseline = 0;
        Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);

        drawContours(original, vector<vector<Point>>{contours[i]}, -1, Scalar(0, 255, 0), 2);

        Point textOrg(center.x - textSize.width / 2, center.y + textSize.height / 2);

        rectangle(original, textOrg - Point(2, baseline + 2),
            textOrg + Point(textSize.width + 2, 2), Scalar(255, 255, 255), FILLED);
        putText(original, text, textOrg, fontFace, fontScale, Scalar(0, 0, 0), thickness);
    }


    imshow("Detected Shapes", original);
    waitKey(0);

    return 0;
}