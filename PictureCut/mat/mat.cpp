// mat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "cstdio"
#include <fstream>
#include <io.h>
using namespace std;
using namespace cv;

size_t i = 201;
vector<string> vec;
const string rootDir = "E:/Picture/Glass/";
const string saveDir = "E:/Picture/Glass/new/";

#ifdef MAIN
int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("1.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
	if (img.empty())
	{
		cout << "can't open file" << endl;
		exit(1);
	}
	else
	{
		namedWindow("img", CV_WINDOW_AUTOSIZE);
		imshow("img", img);
		while (waitKey(1)!=27);
		destroyAllWindows();
		exit(0);
		
	}

	return 0;
}

#endif

	fstream out("result.txt",fstream::app);
	cv::Mat org, dst, img, tmp;
	void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
	{
		static Point pre_pt = (-1, -1);//初始坐标  
		static Point cur_pt = (-1, -1);//实时坐标  
		char temp[16];
		if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
		{
			org.copyTo(img);//将原始图片复制到img中  
			sprintf(temp, "(%d,%d)", x, y);
			/*out << x << " " << y << " ";
			cout << x << " " << endl;
			out.flush();*/
			pre_pt = Point(x, y);
			putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
			circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
			imshow("img", img);
		}
		else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
		{
			img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
			imshow("img", tmp);
		}
		else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
		{
			img.copyTo(tmp);
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
			imshow("img", tmp);
		}
		else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
		{
			org.copyTo(img);
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			/*out << x << " " << y << " ";
			out.flush();*/
			putText(img, temp, cur_pt, FONT_HERSHEY_PLAIN, 0.5, Scalar(0, 0, 0, 255));
			circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
			rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
			imshow("img", img);
			img.copyTo(tmp);
			//截取矩形包围的图像，并保存到dst中  
			int width = abs(pre_pt.x - cur_pt.x);
			int height = abs(pre_pt.y - cur_pt.y);
			/*sprintf(temp, "(%d,%d)", height, width);
			putText(img, temp, Point(120,100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));*/
			if (width <200|| height == 200)
			{
				cout << "size too small!" << endl;
				cout << width << "," << height << endl;
				return;
			}
			else
			{
				dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));   //
				Mat d(256, 256, dst.type());
				//resize(dst, d,Size(256,256));
				imwrite((saveDir + vec[i]).c_str(), dst);//保存截图
			}
			
			/*namedWindow("dst");
			imshow("dst", dst);
			waitKey(0);*/
		}
	}

	bool TraverseFiles(string path, vector<string> & vec)
	{
		_finddata_t file_info;
		string current_path = path + "*.jpg"; //可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台  
		//打开文件查找句柄  
		int handle = _findfirst(current_path.c_str(), &file_info);
		//返回值为-1则查找失败  
		if (-1 == handle)
			return false;
		do
		{
			//判断是否子目录  
			//string attribute;
			//if (file_info.attrib == _A_SUBDIR) //是目录  
			//	attribute = "dir";
			//else
			//	attribute = "file";
			vec.push_back(file_info.name);
		
			//输出文件信息并计数,文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件是否目录  
			//cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示  
			//file_num++;

		} while (!_findnext(handle, &file_info));  //返回-1则遍历完  
		//关闭文件句柄  
		_findclose(handle);
		return true;
	}
	void main()
	{
		namedWindow("img");//定义一个img窗口  
		setMouseCallback("img", on_mouse, 0);//调用回调函数 
		TraverseFiles(rootDir, vec);
		for (vector<string>::const_iterator it = vec.cbegin(); it !=vec.cend(); it++)
		{
			cout << *it << endl;
		}
		
		bool flag = true;
		while (i < vec.size())
		{
			org = imread(rootDir + vec[i]);
			if (org.empty())
			{
				cout << rootDir + vec[i] + "read error!" << endl;
				i++;
				continue;
			}
			//cout << "Type:" << org.type() << endl;
			org.copyTo(img);
			org.copyTo(tmp);

			flag = false;
			imshow("img", img);
			char keyValue = waitKey(10);
			switch (keyValue)
			{
			case 'q':
				i = (i < vec.size() - 1) ? ++i : vec.size() - 1;
				if (i == vec.size() - 1)
					cout << "This is last pic" << endl;
				flag = true;
				break;
			case 'w':
				i = (i >= 1) ? --i : 0;
				if (i == 0)
					cout << "This is first pic" << endl;
				flag = true;
				break;
			case 27:
				goto END;
				break;
			default:
				break;
			}
		}
	END:
		out.close();
		destroyAllWindows();
		system("pause");
	}




