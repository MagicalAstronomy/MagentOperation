#include "DahengDriver.h"


using namespace std;


void* pRGB24Buffer;
cv::Mat buffer;





//用户继承采集事件处理类
class CSampleCaptureEventHandler : public ICaptureEventHandler
{
public:
	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
	{
		/*ConvertToRGB24：无论当前是黑白还是彩色，几位输出都能使用这个函数返回RGB24格式的void*，内存大小是width*height*3
		GX_BIT_0_7是选取有效位，对8位数据只能选取0-7，对10位数据可以选择0-7，1-8，2-9，推荐选2-9
		GX_RAW2RGB_NEIGHBOUR 是邻域平均插值算法
		*/
		pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);

		/*把pRGB24Buffer地址的width* height * 3大小的内存内容拷贝到buffer这个mat中*/
		memcpy(buffer.data, pRGB24Buffer, 2048 * 2448 * 3);

		/*使用create创建一个8UC3类型的mat对象*/
		buffer.create(2048, 2448, CV_8UC3);



		//不知道为什么要对这个temp图像进行翻转 先暂时不用了吧
		//flip(temp_img, temp_img, 0);

		//这个flag是dahengdriver类的属性，用来标志已经有新帧产生，还没有读出的情况，先不使用了吧
		//flag = true;



		// resize(temp_img, *src_img, temp_img.size());
		//imshow("picture capture", *src_img);
		//waitKey(1);
		/*cout << "收到一帧图像!" << endl;
		cout << "ImageInfo: " << objImageDataPointer->GetStatus() << endl;
		cout << "图片宽度: " << objImageDataPointer->GetWidth() << endl;
		cout << "图片高度: " << objImageDataPointer->GetHeight() << endl;
		cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << endl;
		cout << "帧数：" << objImageDataPointer->GetFrameID() << endl;*/

		* (bool*)(pUserParam) = true;

	}
};




DahengDriver::DahengDriver()
{
	buffer = cv::Mat::zeros(cv::Size(2048, 2448), CV_8UC3);
	frameFlag = false;
	flagPtr = (void*)&frameFlag;
	/*对大恒库进行初始化*/
	IGXFactory::GetInstance().Init();

	try
	{
		do
		{
			//枚举设备
			gxdeviceinfo_vector vectorDeviceInfo;
			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
			for (unsigned int i = 0; i < vectorDeviceInfo.size(); i++)
			{
				cout << vectorDeviceInfo[i].GetVendorName() << endl;
				cout << vectorDeviceInfo[i].GetModelName() << endl;
				//更多设备信息详见IGXDeviceInfo 接口
			}
			if (0 == vectorDeviceInfo.size())
			{
				cout << "无可用设备!" << endl;
				break;
			}
			//打开第一台设备以及设备下面第一个流
			ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
			ObjStreamPtr = ObjDevicePtr->OpenStream(0);

			//获取远端设备属性控制器，可用于配置大多数常用参数
			ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

			//获取流层属性控制器,用于配置视频流
			ObjStreamFeatureControlPtr = ObjStreamPtr->GetFeatureControl();

			//设置白平衡，按照当前CCD需要red2,green1,blue1.8
			ObjFeatureControlPtr->GetEnumFeature("BalanceRatioSelector")->SetValue("Red");
			ObjFeatureControlPtr->GetFloatFeature("BalanceRatio")->SetValue(2.0000);
			ObjFeatureControlPtr->GetEnumFeature("BalanceRatioSelector")->SetValue("Green");
			ObjFeatureControlPtr->GetFloatFeature("BalanceRatio")->SetValue(1.0000);
			ObjFeatureControlPtr->GetEnumFeature("BalanceRatioSelector")->SetValue("Blue");
			ObjFeatureControlPtr->GetFloatFeature("BalanceRatio")->SetValue(1.8000);


			//设置Buffer 处理模式
			ObjStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("NewestOnly");

			////设置曝光时间(示例中写死us,只是示例,并不代表真正可工作参数)
			ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(10000);

			

			//注册回调采集
			pCaptureEventHandler = new CSampleCaptureEventHandler();
			//ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

			ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, flagPtr);

			

			/*用于设置采集buffer个数 没太明白有什么用，先不使用了吧
			ObjStreamPtr->SetAcqusitionBufferNumber(2);
			*/

			//发送开采命令，已移动到按钮指令中
			//ObjStreamPtr->StartGrab();
			//ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
			ObjStreamPtr->StartGrab();
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		} while (0);
	}
	catch (CGalaxyException& e)
	{
		cout << "错误码: " << e.GetErrorCode() << endl;
		cout << "错误描述信息: " << e.what() << endl;
	}
	catch (std::exception& e)
	{
		cout << "错误描述信息: " << e.what() << endl;
	}


}

DahengDriver::~DahengDriver()
{

	//发送停采命令
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();
	//注销采集回调
	ObjStreamPtr->UnregisterCaptureCallback();
	//反初始化库
	IGXFactory::GetInstance().Uninit();
	//销毁事件回调指针
	if (NULL != pCaptureEventHandler)
	{
		delete pCaptureEventHandler;
		pCaptureEventHandler = NULL;
	}
	//销毁设备指针和流指针
	ObjStreamPtr->Close();
	ObjDevicePtr->Close();
	delete flagPtr;
}

void DahengDriver::getPicture(cv::Mat& img)
{
	//将当前帧输出到img
	buffer.copyTo(img);
	frameFlag = false;

	cout << "成功拷贝 " << endl;
}

void DahengDriver::startGrab()
{
	ObjStreamPtr->StartGrab();
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
}

void DahengDriver::stopGrab()
{
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();
}