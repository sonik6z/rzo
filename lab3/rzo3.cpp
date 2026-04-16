#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
using namespace cv;
using namespace std;

void saveImage(Mat& img, string name)
{
    string filename = name + ".png";
    imwrite(filename, img);
    cout << "Сохранено: " << filename << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Mat img = imread("rzo3.png");
    if (img.empty()) {
        cout << "ОШибка! Это очень грустно" << endl;
        return -1;
    }

    cout << "Изображение загружено: " << img.cols << "x" << img.rows << endl;

    imshow("Original", img);
    waitKey(0);

    //HSV голубой
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    Mat mask_hsv;
    inRange(hsv, Scalar(80, 100, 100), Scalar(120, 255, 255), mask_hsv);
    saveImage(mask_hsv, "1_HSV_mask");

    Mat result_hsv;
    img.copyTo(result_hsv, mask_hsv);
    saveImage(result_hsv, "2_HSV_result");

    imshow("Mask HSV", mask_hsv);
    imshow("HSV", result_hsv);
    waitKey(0);

    //YCrCb
    Mat ycrcb;
    cvtColor(img, ycrcb, COLOR_BGR2YCrCb);

    vector<Mat> channels;
    split(ycrcb, channels);

    saveImage(channels[0], "3_YCrCb_Y");
    saveImage(channels[1], "4_YCrCb_Cr");
    saveImage(channels[2], "5_YCrCb_Cb");

    imshow("Y channel", channels[0]);
    imshow("Cr channel", channels[1]);
    imshow("Cb channel", channels[2]);
    waitKey(0);

    Mat mask_ycrcb;
    inRange(ycrcb, Scalar(0, 0, 120), Scalar(255, 150, 255), mask_ycrcb);
    saveImage(mask_ycrcb, "6_YCrCb_mask");

    imshow("Mask YCrCb", mask_ycrcb);
    waitKey(0);

    //Lab
    Mat lab;
    cvtColor(img, lab, COLOR_BGR2Lab);

    vector<Mat> labChannels;
    split(lab, labChannels);

    saveImage(labChannels[0], "7_Lab_L");
    saveImage(labChannels[1], "8_Lab_a");
    saveImage(labChannels[2], "9_Lab_b");

    imshow("L channel", labChannels[0]);
    imshow("a channel", labChannels[1]);
    imshow("b channel", labChannels[2]);
    waitKey(0);

    Mat mask_lab;
    threshold(labChannels[2], mask_lab, 130, 255, THRESH_BINARY);
    saveImage(mask_lab, "10_Lab_mask");

    imshow("Mask Lab", mask_lab);
    waitKey(0);

    //Хром
    Mat imgFloat;
    img.convertTo(imgFloat, CV_32F);

    vector<Mat> bgr;
    split(imgFloat, bgr);

    Mat sum = bgr[0] + bgr[1] + bgr[2];

    Mat r = bgr[2] / sum;
    Mat g = bgr[1] / sum;
    Mat b = bgr[0] / sum;

    Mat r_show, g_show, b_show;
    r.convertTo(r_show, CV_8U, 255);
    g.convertTo(g_show, CV_8U, 255);
    b.convertTo(b_show, CV_8U, 255);

    saveImage(r_show, "11_chromatic_R");
    saveImage(g_show, "12_chromatic_G");
    saveImage(b_show, "13_chromatic_B");

    imshow("Chromatic R", r_show);
    imshow("Chromatic G", g_show);
    imshow("Chromatic B", b_show);
    waitKey(0);

    Mat mask_chromatic;
    Mat blue_high = b > 0.4;
    Mat red_low = r < 0.3;
    Mat green_low = g < 0.3;
    mask_chromatic = blue_high & red_low & green_low;

    mask_chromatic.convertTo(mask_chromatic, CV_8U, 255);
    saveImage(mask_chromatic, "14_chromatic_mask");

    imshow("Mask Chromatic", mask_chromatic);
    waitKey(0);

    waitKey(0);
    destroyAllWindows();

    return 0;
}