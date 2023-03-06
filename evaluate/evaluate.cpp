#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 均方误差MSE
double getMSE(Mat& srcImage, Mat& dstImage)
{
    Mat src = dstImage, dst = srcImage;
    int channels = dstImage.channels(), rowsNumber = src.rows, colsNumber = src.cols * channels;
    double sigma = 0.0, mse = 0.0;
    for (int i = 0; i < rowsNumber; i++)
        for (int j = 0; j < colsNumber; j++)
            mse += (src.ptr<uchar>(i)[j] - dst.ptr<uchar>(i)[j]) * (src.ptr<uchar>(i)[j] - dst.ptr<uchar>(i)[j]);
    mse = mse / (rowsNumber * colsNumber);
    return mse;
}

// 图片信噪比SNR
double getSNR(Mat& srcImage, Mat& dstImage)
{
    Mat src = dstImage, dst = srcImage;
    int channels = dstImage.channels(), rowsNumber = src.rows, colsNumber = src.cols * channels;

    double sigma = 0.0, mse = 0.0, SNR = 0.0;
    for (int i = 0; i < rowsNumber; i++) {
        for (int j = 0; j < colsNumber; j++) {
            sigma += (src.ptr<uchar>(i)[j]) * (src.ptr<uchar>(i)[j]);
            mse += (src.ptr<uchar>(i)[j] - dst.ptr<uchar>(i)[j]) * (src.ptr<uchar>(i)[j] - dst.ptr<uchar>(i)[j]);
        }
    }
    SNR = 10 * log10(sigma / mse);
    return SNR;
}

// 使用高斯模糊加速计算结构相似性SSIM
double getSSIM(Mat& srcImage, Mat& dstImage) {
    const double C1 = 6.5025, C2 = 58.5225;
    Mat I1, I2;
    srcImage.convertTo(I1, CV_32F);
    dstImage.convertTo(I2, CV_32F);
    Mat I1_2 = I1.mul(I1);
    Mat I2_2 = I2.mul(I2);
    Mat I1_I2 = I1.mul(I2);
    Mat mu1, mu2;
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);
    Mat sigma1_2, sigam2_2, sigam12;
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigam2_2, Size(11, 11), 1.5);
    sigam2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigam12, Size(11, 11), 1.5);
    sigam12 -= mu1_mu2;
    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigam12 + C2;
    t3 = t1.mul(t2);

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigam2_2 + C2;
    t1 = t1.mul(t2);

    Mat ssim_map;
    divide(t3, t1, ssim_map);
    Scalar mssim = mean(ssim_map);

    double ssim = (mssim.val[0] + mssim.val[1] + mssim.val[2]) / 3;
    return ssim;
}

// 不使用高斯模糊进行ssim计算
double getSSIM_nogb(Mat& srcImage, Mat dstImage)
{
    double C1 = 6.5025, C2 = 58.5225;
    int width = srcImage.cols;
    int height = srcImage.rows;
    double mean_x = 0;
    double mean_y = 0;
    double sigma_x = 0;
    double sigma_y = 0;
    double sigma_xy = 0;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            mean_x += dstImage.at<uchar>(v, u);
            mean_y += dstImage.at<uchar>(v, u);

        }
    }
    mean_x = mean_x / width / height;
    mean_y = mean_y / width / height;
    for (int v = 0; v < height; v++)
    {
        for (int u = 0; u < width; u++)
        {
            sigma_x += (srcImage.at<uchar>(v, u) - mean_x) * (srcImage.at<uchar>(v, u) - mean_x);
            sigma_y += (dstImage.at<uchar>(v, u) - mean_y) * (dstImage.at<uchar>(v, u) - mean_y);
            sigma_xy += abs((srcImage.at<uchar>(v, u) - mean_x) * (dstImage.at<uchar>(v, u) - mean_y));
        }
    }
    sigma_x = sigma_x / (width * height - 1);
    sigma_y = sigma_y / (width * height - 1);
    sigma_xy = sigma_xy / (width * height - 1);
    double fenzi = (2 * mean_x * mean_y + C1) * (2 * sigma_xy + C2);
    double fenmu = (mean_x * mean_x + mean_y * mean_y + C1) * (sigma_x + sigma_y + C2);
    double ssim = fenzi / fenmu;
    return ssim;

}

int main()
{
    // 请修改为自己的图像路径
    String gtPath = "C:\\Users\\Lunci\\Desktop\\daytime_10331.jpg", outPath = "C:\\Users\\Lunci\\Desktop\\LLFlow_10331.jpg";;
    auto gtImage = imread(gtPath), outImage = imread(outPath);
    if (gtImage.empty() || outImage.empty())
    {
        cout << "读取图像有误，请重新输入正确路径！\n";
        return -1;
    }

    double mse = getMSE(gtImage, outImage), snr = getSNR(gtImage, outImage);
    double psnr = 10.0 * log10((255 * 255) / mse), ssim = getSSIM(gtImage, outImage);
    cout << "计算MSE值得到：MSE = " << mse << endl;;
    cout << "计算SNR值得到：SNR = " << snr << endl;;
    cout << "计算PSNR值得到：PSNR = " << psnr << endl;;
    cout << "使用高斯模糊计算SSIM值得到：SSIM = " << ssim << endl;;
    double ssim_nogb = getSSIM_nogb(gtImage, outImage);
    cout << "不使用高斯模糊计算SSIM值得到：SSIM = " << ssim_nogb << endl;
    waitKey(100);
    return 0;
}
