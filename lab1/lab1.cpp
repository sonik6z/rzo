#include <opencv2/opencv.hpp>
#include <iostream>
#include <clocale>
#include <vector>
using namespace cv;
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    Mat image = imread("balbes.jpg");
    if (image.empty())
    {
        cout << "Ошибка!!!!!!!!" << endl;
        return -1;
    }

    Point2f center(image.cols / 2.0, image.rows / 2.0);
    Mat gray, resize1, resize2, rotated1, rotated2, blur, canny;

    // чб
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // размер
    resize(image, resize1, Size(), 0.5, 0.5);   
    resize(image, resize2, Size(), 2, 2);      

    // поворот
    Mat side = getRotationMatrix2D(center, 90, 1.0);   
    Mat back = getRotationMatrix2D(center, 180, 1.0);  
    warpAffine(image, rotated1, side, image.size());
    warpAffine(image, rotated2, back, image.size());

    // размытие по Гауссу 
    GaussianBlur(image, blur, Size(9, 9), 10);

    // границы Кэнни 
    Canny(gray, canny, 50, 150);

    imshow("Исходное изображение", image);

    vector<pair<Mat, string>> images = {
        {gray, "чб_балбес"},
        {resize1, "уменьшанный_балбес"},
        {resize2, "увеличенный_балбес"},
        {rotated1, "балбес_90градусов"},
        {rotated2, "балбес_180градусов"},
        {blur, "размытый_балбес"},
        {canny, "границы_балбес"}
    };

    for (auto& img : images) {
        imshow(img.second, img.first);
        imwrite(img.second + ".jpg", img.first);
        cout << "Сохранено: " << img.second << ".jpg" << endl;
    }

    cout << "\nВсе изображения сохранены в папке с программой" << endl;

    waitKey(0);
    destroyAllWindows();

    return 0;
}
