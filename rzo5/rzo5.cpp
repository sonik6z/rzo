#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

// Определение формы по аппроксимированному контуру
string detectShape(const vector<Point>& contour, double area) {
    double perimeter = arcLength(contour, true);
    if (perimeter == 0) return "Unknown";

    // Аппроксимация контура с целью получения минимального количества вершин
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.02 * perimeter, true);
    int vertices = (int)approx.size();

    if (vertices == 3) {
        return "Triangle";
    }
    else if (vertices == 4) {
        // Проверяем, является ли четырёхугольник квадратом
        Rect box = boundingRect(contour);
        double aspectRatio = (double)box.width / box.height;
        // Квадрат: отношение сторон близко к 1 (допуск 0.2)
        if (aspectRatio >= 0.8 && aspectRatio <= 1.2) {
            return "Square";
        }
        else {
            return "Rectangle"; // но по заданию требуются только квадраты, можно всё же считать квадратом, если порог жёстче
        }
    }
    else if (vertices > 4) {
        // Проверка на круг: отношение площади контура к площади описанной окружности
        Point2f center;
        float radius;
        minEnclosingCircle(contour, center, radius);
        double circleArea = CV_PI * radius * radius;
        double areaRatio = area / circleArea;
        if (areaRatio > 0.8) {
            return "Circle";
        }
        else {
            return "Polygon";
        }
    }
    return "Unknown";
}

int main() {
    string videoFile = "rzo5.mkv"; 
    VideoCapture cap(videoFile);
    if (!cap.isOpened()) {
        cerr << "Ошибка: не удалось открыть видео " << videoFile << endl;
        return -1;
    }

    Ptr<BackgroundSubtractor> bgSubtractor = createBackgroundSubtractorMOG2(500, 16, true);

    Mat frame, fgMask, gray, binary;

    while (true) {
        cap >> frame;
        if (frame.empty()) break; 

        bgSubtractor->apply(frame, fgMask);

        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(fgMask, fgMask, MORPH_OPEN, kernel);
        morphologyEx(fgMask, fgMask, MORPH_CLOSE, kernel);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(fgMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double area = contourArea(contour);
            if (area < 500) continue; 

            string shape = detectShape(contour, area);

            Rect bbox = boundingRect(contour);

            rectangle(frame, bbox, Scalar(0, 255, 0), 2);

            Point center(bbox.x + bbox.width / 2, bbox.y + bbox.height / 2);

            int fontFace = FONT_HERSHEY_SIMPLEX;
            double fontScale = 0.6;
            int thickness = 1;
            string text = shape;
            int baseline;
            Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);

            rectangle(frame, Point(center.x - textSize.width / 2 - 2, center.y - textSize.height / 2 - 2),
                Point(center.x + textSize.width / 2 + 2, center.y + textSize.height / 2 + 2),
                Scalar(255, 255, 255), FILLED);

            putText(frame, text, Point(center.x - textSize.width / 2, center.y + textSize.height / 2),
                fontFace, fontScale, Scalar(0, 0, 0), thickness);
        }

        imshow("Moving Shapes Detection", frame);

        char key = (char)waitKey(30);
        if (key == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}