#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 遍历文件夹内的所有图像
int getImagePathList(std::string folder, std::vector<cv::String>& imagePathList)
{
	cv::glob(folder, imagePathList);
	return 0;
}

// 获取图像的文件名
string getName(string str) {
	int length = str.size();
	int start = 0;
	for (int i = length - 1; i >= 0; --i) {
		if (str[i] == '\\') {
			start = i;
			break;
		}
	}
	return str.substr(start + 1);
}

int main()
{
	// 请修改为自己的文件夹路径
	cv::String folder = "C:\\Users\\Lunci\\Desktop\\dataset\\high";
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

		// 按照指定的宽、高对图像进行尺寸变换
		Size dsize = Size(600, 400);
		Mat output;
		resize(srcImage, output, dsize, 0, 0, INTER_AREA);
		string name = getName(imagePathList[i]);
		string outPath = "C:\\Users\\Lunci\\Desktop\\dataset\\new_high\\" + name;
		imwrite(outPath, output);

		waitKey(100);
	}
	return 0;
}
