#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 添加高斯噪声
void gaussian_noise(Mat & image) {
	Mat noise = Mat::zeros(image.size(), image.type());
	randn(noise, (15, 15, 15), (30, 30, 30));
	Mat dst;
	add(image, noise, dst);
}

// 遍历文件夹内的所有图像
int getImagePathList(std::string folder, std::vector<cv::String>& imagePathList)
{
	cv::glob(folder, imagePathList);
	return 0;
}

// 获得图像的文件名，方便后续保存图像
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

int main()
{
	Mat srcImage, mergeImage;		//源图像、通道合并后的图像
	Mat image_V;					// V通道
	vector<Mat> channels_HSV;		//vector<Mat>： 可以理解为存放Mat类型的容器（数组）

	//请修改为自己的图像路径
	cv::String folder = "C:\\Users\\Lunci\\Desktop\\dataset\\high2000";
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
		//imshow("srcImage源图像", srcImage);		//在窗口显示源图像 

		Mat image_hsv;
		// 将BGR颜色空间转换为HSV颜色空间
		cvtColor(srcImage, image_hsv, COLOR_BGR2HSV);
		// 对加载的HSV图像进行通道分离
		split(image_hsv, channels_HSV);
		// 0通道为H分量，1通道为S分量，2通道为V分量
		image_V = channels_HSV.at(2);

		double gamma = 1.25, s = 0.06;
		// 处理V通道数据
		for (int i = 0; i < image_V.rows; i++)
		{
			for (int j = 0; j < image_V.cols; j++)
			{
				double src = image_V.at<uchar>(i, j);
				int dst = s * pow(src, gamma);
				image_V.at<uchar>(i, j) = dst;
			}
		}
		// 将3个单通道重新合并成一个三通道图像
		merge(channels_HSV, mergeImage);
		//imshow("将H，S，V通道合并后的图像", newImage);

		// 添加高斯模糊
		Mat gBlurImage;
		GaussianBlur(mergeImage, gBlurImage, Size(5, 5), 1.25, 1.25);

		// 将HSV颜色空间转换为BGR颜色空间
		Mat output;
		cvtColor(gBlurImage, output, COLOR_HSV2BGR);

		// 添加高斯噪声
		gaussian_noise(output);
		string name = getName(imagePathList[i]);
		string outPath = "C:\\Users\\Lunci\\Desktop\\dataset\\low2000\\" + name;

		imwrite(outPath, output);
		waitKey(100);
	}
	return 0;
}
