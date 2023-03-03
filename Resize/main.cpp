#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int getImagePathList(std::string folder, std::vector<cv::String>& imagePathList)
{
	// search all the image in a folder
	glob(folder, imagePathList);
	return 0;
}

string getName(string str) {
  // get image name from its path
	int end = str.size() -1;
	while(str[end] != '\\')
      --end;
	return str.substr(end + 1);
}

int main()
{
	String folder = "D:\\dataset";
	vector<String> imagePathList;
	getImagePathList(folder, imagePathList);
	for (int i = 0; i < imagePathList.size(); i++)
	{
		cout << imagePathList[i] << "\n";
		auto srcImage = cv::imread(imagePathList[i]);
		if (srcImage.empty())
		{
			cout << "cannot read the image, retry plz！\n";
			return -1;
		}
    // start to resize
		Size dsize = Size(600, 400);
		Mat output;
		resize(srcImage, output, dsize, 0, 0, INTER_AREA);
		string name = getName(imagePathList[i]);
		string outPath = "D:\\new_dataset" + '\\' + name;
		imwrite(outPath, output);

		waitKey(100);
	}
	return 0;
}

