#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

void gaussian_noise(Mat & image) {
	Mat noise = Mat::zeros(image.size(), image.type());
	randn(noise, (15, 15, 15), (30, 30, 30));
	Mat dst;
	add(image, noise, dst);
}

int getImagePathList(std::string folder, std::vector<cv::String>& imagePathList)
{
	//search all the image in a folder
	cv::glob(folder, imagePathList);
	return 0;
}

string getName(string str) {
	int length = str.size();
	int start = 0;
	for (int i = length-1; i >= 0; --i) {
		if (str[i] == '\\') {
			start = i;
			break;
		}
	}
	return str.substr(start + 1);
}
//Mat& addPoissonNoiseIntoImage(Mat& src) {
//	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//	static Mat noisyImg = src.clone();
//	std::default_random_engine generator(seed);
//	std::vector<float> uniqueValsOfMat = BasicAlgo::getInstance()->uniqueValuesInMat(src);
//	unsigned long int val = uniqueValsOfMat.size();
//	float vals = pow(2, (ceil(log2(val))));
//	for (int i = 0; i < src.rows; i++)
//	{
//		for (int j = 0; j < src.cols; j++) {
//			float pixelVal = (double)(src.at<uchar>(i, j));
//			pixelVal = pixelVal * vals;
//			std::poisson_distribution<int> distribution(pixelVal);
//			float generatedVal = distribution(generator) / float(vals);
//			noisyImg.at<uchar>(i, j) = generatedVal;
//		}
//	}
//	return noisyImg;
//}


int main()
{
	//【0】定义相关变量
	Mat srcImage, newImage;					//源图像、通道合并后的图像
	Mat srcImage_B, srcImage_G, srcImage_R;	//R、G、B各个通道
	Mat image_H, image_S, image_V;			//H、S、V各个通道
	vector<Mat> channels_BGR;		//vector<Mat>： 可以理解为存放Mat类型的容器（数组）
	vector<Mat> channels_HSV;
	//【1】读取原始图像并检查图像是否读取成功  

	cv::String folder = "C:\\Users\\Lunci\\Desktop\\high";
	std::vector<cv::String> imagePathList;
	getImagePathList(folder, imagePathList);
	for (int i = 0; i < imagePathList.size(); i++)
	{
		std::cout << imagePathList[i] << "\n";
		auto srcImage = cv::imread(imagePathList[i]);
		if (srcImage.empty())
		{
			cout << "读取图像有误，请重新输入正确路径！\n";
			return -1;
		}
		Mat image_hsv;
		cvtColor(srcImage, image_hsv, COLOR_BGR2HSV);
		split(image_hsv, channels_HSV);
		image_V = channels_HSV.at(2);

		double gamma = 1.1, s = 0.03;
		// 处理v通道数据
		for (int i = 0; i < image_V.rows; i++)
		{
			for (int j = 0; j < image_V.cols; j++)
			{
				double src = image_V.at<uchar>(i, j);
				int dst = s * pow(src, gamma);
				image_V.at<uchar>(i, j) = dst;
			}
		}

		merge(channels_HSV, newImage);
		
		Mat result;
		GaussianBlur(newImage, result, Size(5, 5), 1.25, 1.25);
		Mat output2;
		cvtColor(result, output2, COLOR_HSV2BGR);
		gaussian_noise(output2);
		string name = getName(imagePathList[i]);
		string outPath = "C:\\Users\\Lunci\\Desktop\\low\\" + name;
		imwrite(outPath, output2);

		waitKey(100);
		
	}
	return 0;

	//srcImage = imread("C:\\Users\\Lunci\\Desktop\\12513.jpg");	//请修改为自己的图像路径
	//if (srcImage.empty())
	//{
	//	cout << "读取图像有误，请重新输入正确路径！\n";
	//	return -1;
	//}
	////imshow("srcImage源图像", srcImage);		//在窗口显示源图像  

	////【3】将BGR颜色空间转换为HSV颜色空间
	//Mat image_hsv;
	//cvtColor(srcImage, image_hsv, COLOR_BGR2HSV);
	//// imshow("HSV颜色空间图像", image_hsv);

	////【4】对加载的HSV图像进行通道分离
	//split(image_hsv, channels_HSV);
	////0通道为H分量，1通道为S分量，2通道为V分量
	//image_H = channels_HSV.at(0);
	//image_S = channels_HSV.at(1);
	//image_V = channels_HSV.at(2);
	//////分别显示H，S，V各个通道图像
	////imshow("image_H通道", image_H);
	////imshow("image_S通道", image_S);
	////imshow("image_V通道", image_V);

	//double gamma = 1.1, s = 0.03;
	//// 处理v通道数据
	//for (int i = 0; i < image_V.rows; i++)
	//{
	//	for (int j = 0; j < image_V.cols; j++)
	//	{
	//		double src = image_V.at<uchar>(i, j);
	//		int dst = s * pow(src, gamma);
	//		image_V.at<uchar>(i, j) = dst;
	//	}
	//}
	//

	//// imshow("处理后的V通道图像", image_V);
	////【5】将3个单通道重新合并成一个三通道图像
	//merge(channels_HSV, newImage);
	////imshow("将H，S，V通道合并后的图像", newImage);

	////imshow("V通道经过变换后的图像", image_V);
	//Mat result;
	//GaussianBlur(newImage, result, Size(5, 5), 1.25, 1.25);
	////imshow("高斯模糊后的图像", newnewImage);
	////Mat output1;
	////cvtColor(newImage, output1, COLOR_HSV2BGR);
	////imshow("未经高斯模糊后恢复为RGB图", output1);
	//Mat output2;
	//cvtColor(result, output2, COLOR_HSV2BGR);
	////imshow("经高斯模糊后恢复为RGB图", output2);

	//gaussian_noise(output2);
	//imshow("添加高斯噪声后的RGB图", output2);

	//imwrite("C:\\Users\\Lunci\\Desktop\\12513_1.jpg", output2);

	////【6】保持等待状态  
	//waitKey(0);
	//return 0;
}
