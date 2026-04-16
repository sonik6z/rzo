#include <opencv2/opencv.hpp>
#include <vector>
#include <ctime>

using namespace cv;
using namespace std;

int main() {
    Mat canvas(600, 800, CV_8UC3, Scalar(255, 255, 255));

    line(canvas, Point(0, 0), Point(799, 599), Scalar(255, 105, 180), 3);

    rectangle(canvas, Point(100, 100), Point(300, 250), Scalar(255, 20, 147), 2);

    circle(canvas, Point(400, 300), 100, Scalar(255, 192, 203), -1);

    ellipse(canvas, Point(400, 300), Size(150, 70), 30, 0, 360, Scalar(255, 105, 180), 2);

    vector<Point> pts = { Point(550, 100), Point(600, 150), Point(650, 250),
                         Point(550, 300), Point(550, 300), Point(450, 200) };
    polylines(canvas, pts, true, Scalar(255, 20, 147), 3);
    fillPoly(canvas, vector<vector<Point>>{pts}, Scalar(255, 182, 193));

    imwrite("rzo2saved.jpg", canvas);

    srand(time(NULL));

    for (int i = 0; i < 15; i++) {
        Mat frame = canvas.clone();
        int x = rand() % 700;
        int y = rand() % 550;
        putText(frame, "Zelenova Sofya 22AM", Point(x, y),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 105, 180), 2);
        imshow("Animation", frame);
        waitKey(800);
    }

    waitKey(0);
    return 0;
}