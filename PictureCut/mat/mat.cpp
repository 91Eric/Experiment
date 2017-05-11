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
	void on_mouse(int event, int x, int y, int flags, void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
	{
		static Point pre_pt = (-1, -1);//��ʼ����  
		static Point cur_pt = (-1, -1);//ʵʱ����  
		char temp[16];
		if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ  
		{
			org.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
			sprintf(temp, "(%d,%d)", x, y);
			/*out << x << " " << y << " ";
			cout << x << " " << endl;
			out.flush();*/
			pre_pt = Point(x, y);
			putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//�ڴ�������ʾ����  
			circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//��Բ  
			imshow("img", img);
		}
		else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
		{
			img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//ֻ��ʵʱ��ʾ����ƶ�������  
			imshow("img", tmp);
		}
		else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
		{
			img.copyTo(tmp);
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
			rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
			imshow("img", tmp);
		}
		else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ�����  
		{
			org.copyTo(img);
			sprintf(temp, "(%d,%d)", x, y);
			cur_pt = Point(x, y);
			/*out << x << " " << y << " ";
			out.flush();*/
			putText(img, temp, cur_pt, FONT_HERSHEY_PLAIN, 0.5, Scalar(0, 0, 0, 255));
			circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
			rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
			imshow("img", img);
			img.copyTo(tmp);
			//��ȡ���ΰ�Χ��ͼ�񣬲����浽dst��  
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
				imwrite((saveDir + vec[i]).c_str(), dst);//�����ͼ
			}
			
			/*namedWindow("dst");
			imshow("dst", dst);
			waitKey(0);*/
		}
	}

	bool TraverseFiles(string path, vector<string> & vec)
	{
		_finddata_t file_info;
		string current_path = path + "*.jpg"; //���Զ������ĺ�׺Ϊ*.exe��*.txt���������ض���׺���ļ���*.*��ͨ�����ƥ����������,·�����ӷ��������б��/���ɿ�ƽ̨  
		//���ļ����Ҿ��  
		int handle = _findfirst(current_path.c_str(), &file_info);
		//����ֵΪ-1�����ʧ��  
		if (-1 == handle)
			return false;
		do
		{
			//�ж��Ƿ���Ŀ¼  
			//string attribute;
			//if (file_info.attrib == _A_SUBDIR) //��Ŀ¼  
			//	attribute = "dir";
			//else
			//	attribute = "file";
			vec.push_back(file_info.name);
		
			//����ļ���Ϣ������,�ļ���(����׺)���ļ�����޸�ʱ�䡢�ļ��ֽ���(�ļ�����ʾ0)���ļ��Ƿ�Ŀ¼  
			//cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //��õ�����޸�ʱ����time_t��ʽ�ĳ����ͣ���Ҫ����������ת������ʱ����ʾ  
			//file_num++;

		} while (!_findnext(handle, &file_info));  //����-1�������  
		//�ر��ļ����  
		_findclose(handle);
		return true;
	}
	void main()
	{
		namedWindow("img");//����һ��img����  
		setMouseCallback("img", on_mouse, 0);//���ûص����� 
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




