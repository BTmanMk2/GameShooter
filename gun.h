#pragma once
//add by wzy
//蓝牙所需变量
#include <string>
#include <QFile>
#include <QLibrary>
#include <QXmlStreamReader>
#include <qdebug.h>

typedef struct
{
	unsigned int count;
	unsigned char port;
	unsigned int isReload;
}
BlueMsg;

typedef bool(*MyFunc1)(int n);
typedef bool(*MyFunc2)();
typedef BlueMsg(*MyFunc3)(int n);
typedef unsigned long(*MyFunc4)(int n, bool isLock);

MyFunc1 bluetooth_Start;
MyFunc2 bluetooth_End;
MyFunc3 bluetooth_GetMsg;
MyFunc4 bluetooth_PutMsg;

//枪点捕捉所需变量
typedef struct
{
	int x;
	int y;
}
StPoint;

typedef struct
{
	int width;
	int height;
}
StSize;

typedef struct
{
	bool isNewStPoint;
	StPoint stPoint;
	StSize stSize;
	int proNum;
}
StPointMsg;

struct StPointsMsg
{
	StPointMsg stPointMsg;
	StPointsMsg *nextPointMsg;
};

struct GunInfo
{
	int gunComport;//端口号
	int gunBulletSum;//子弹总数
	int gunBulletUsingNumber;//子弹使用数
	int gunOffsetX;//校准数据
	int gunOffsetY;
	int index;//链接上蓝牙的顺序，从0开始
};

typedef int(*Func_StartOneCam) (int i);
typedef int(*Func_GetOpenStatus) ();
typedef bool(*Func_CloseOneCam) (int i);

typedef int(*Func_StartSingleShootPointModule)(int i);
typedef StPointsMsg* (*Func_GetSingleShootPointsMsg) ();
typedef bool(*Func_EndSingleShootPointModule) ();

Func_StartOneCam StartOneCam;
Func_GetOpenStatus GetOpenStatus;
Func_CloseOneCam CloseOneCam;

Func_StartSingleShootPointModule StartSingleShootPointModule;
Func_GetSingleShootPointsMsg GetSingleShootPointsMsg;
Func_EndSingleShootPointModule EndSingleShootPointModule;
//出错代号，第一位为摄像头，后2位为2把枪
std::string equipmentString = "000";
//是否检测好设备
bool equipmentTest = false;
//武器
GunInfo currentGun[2];
//以连接的武器端口号对应下标
std::map<int, int> comportToIndex;
/*
* 读取Guns.xml
*/
void readGunXMl()
{
	QFile GunsXML("D:\\VRPT_Resource\\Resources\\Guns.xml");
	if (!GunsXML.open(QFile::ReadOnly))
	{
		qCritical() << "can not read file Guns.xml";
		system("pause");
	}
	else
	{
		qDebug() << "read success";
		QXmlStreamReader reader;
		reader.setDevice(&GunsXML);
		int GunInfoNumber = 0;
		while (!reader.atEnd())
		{
			bool isGun;
			QXmlStreamReader::TokenType type = reader.readNext();
			if (type == QXmlStreamReader::StartElement)
			{
				if (reader.name() == "GunName")
				{
					isGun = false;
					reader.readNext();
					currentGun[GunInfoNumber].gunBulletUsingNumber = 0;
				}
				else if (reader.name() == "GunType")
				{
					reader.readNext();
					if (reader.text() == "Gun")
					{
						isGun = true;
					}
					else if (reader.text() == "Stick")
					{
						isGun = false;
					}
					else if (reader.text() == "Chili")
					{
						isGun = false;
					}
				}
				else if (reader.name() == "ComPort")
				{
					reader.readNext();
					currentGun[GunInfoNumber].gunComport = reader.text().toInt();
				}
				else if (reader.name() == "x")
				{
					reader.readNext();
					currentGun[GunInfoNumber].gunOffsetX = reader.text().toInt();
				}
				else if (reader.name() == "y")
				{
					reader.readNext();
					currentGun[GunInfoNumber].gunOffsetY = reader.text().toInt();
					if (isGun)
						GunInfoNumber++;
					if (GunInfoNumber == 2)//读取前2把标为GUN的武器
						break;
				}
			}
		}
		if (GunInfoNumber < 2)
		{
			qCritical() << "GunInfoNumber is: " << GunInfoNumber;
		}
		GunsXML.close();
	}
}

/*
* 初始化蓝牙模块
*/
void blueToothInitial()
{
	QLibrary mylib("D:\\VRPT_Resource\\Resources\\TestBluetoothDLL.dll");
	if (!mylib.load())
	{
		qCritical() << "TestBluetoothDLL.dll not found";
		system("pause");
	}
	else
	{
		bluetooth_Start = (MyFunc1)mylib.resolve("StartBluetoothModule");
		bluetooth_End = (MyFunc2)mylib.resolve("EndBluetoothModule");
		bluetooth_GetMsg = (MyFunc3)mylib.resolve("GetBluetoothMsg");
		bluetooth_PutMsg = (MyFunc4)mylib.resolve("PutBluetoothMsg");
		if (bluetooth_Start == NULL || bluetooth_End == NULL || bluetooth_GetMsg == NULL || bluetooth_PutMsg == NULL)
		{
			qCritical() << "function in TestBluetoothDLL not found";
			system("pause");
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				if (bluetooth_Start(currentGun[i].gunComport))
				{
					qDebug() << currentGun[i].gunComport << (" bluetooth success");
					comportToIndex[currentGun[i].gunComport] = i;
					currentGun[i].index = comportToIndex.size() - 1;
					bluetooth_PutMsg(currentGun[i].index, false);//开锁
					equipmentString[i + 1] = '1';
				}
				else
				{
					qCritical() << "Open COM" << currentGun[i].gunComport << " failed";
				}
			}
		}
	}
}

/*
* 初始化摄像头及枪点模块
*/
void cameraInitial()
{
	QLibrary mylib("D:\\VRPT_Resource\\Resources\\VRPTCameraTracking.dll");
	if (!mylib.load())
	{
		qCritical() << "VRPTCameraTracking.dll not found";
		system("pause");
	}
	else
	{
		StartOneCam = (Func_StartOneCam)mylib.resolve("StartOneCamCache");
		GetOpenStatus = (Func_GetOpenStatus)mylib.resolve("GetOpenStatus");
		CloseOneCam = (Func_CloseOneCam)mylib.resolve("CloseOneCam");
		StartSingleShootPointModule = (Func_StartSingleShootPointModule)mylib.resolve("StartSingleShootPointModuleCache");
		GetSingleShootPointsMsg = (Func_GetSingleShootPointsMsg)mylib.resolve("GetSingleShootPointsMsg");
		EndSingleShootPointModule = (Func_EndSingleShootPointModule)mylib.resolve("EndSingleShootPointModule");
		if (StartOneCam == NULL || GetOpenStatus == NULL || CloseOneCam == NULL || StartSingleShootPointModule == NULL || GetSingleShootPointsMsg == NULL || EndSingleShootPointModule == NULL)
		{
			qCritical() << "function in VRPTCameraTracking not found";
			system("pause");
		}
		else
		{
			if (StartOneCam(4) == 1 && GetOpenStatus() == 2)
			{
				qDebug() << "Success to start the camera." << endl;
				if (StartSingleShootPointModule(4) == 0)
				{
					qDebug() << "Success starting shoot point module." << endl;
					equipmentString[0] = '1';
				}
				else
				{
					qCritical() << "Failed get StartShootPointModule function";
					system("pause");
				}
			}
			else
			{
				qCritical() << "Failed to start the camera!" << endl;
				system("pause");
			}
		}
	}
}

/*
* 关闭蓝牙模块
*/
void blueToothEnd()
{
	if (bluetooth_End())
	{
		qDebug() << ("bluetooth_End success");
	}
	else
		qCritical() << ("bluetooth_End fail");
}

/*
* 关闭摄像头及枪点模块
*/
void cameraEnd()
{
	if (EndSingleShootPointModule())
	{
		qDebug() << ("End shoot point module!");
	}
	else
		qCritical() << ("Failed end shoot point module!");
	if (CloseOneCam(4))
	{
		qDebug() << ("Successed closing the cameras!");
	}
	else
		qCritical() << ("Failed to close the cameras!");
}


/*
*蓝牙信号检测，由于子弹无限制不需要接受蓝牙信号，因此注释此函数
*/
/*void blueToothCheck()
{
	if (comportToIndex.empty())
	{
		//暂时发送一个蓝牙信号供测试用
		lastComport = -1;//测试用
		currentBlueToothLevel = testKeyBoardBlueToothLevel;
	}
	else
	{
		currentBlueToothLevel = testKeyBoardBlueToothLevel;
		for (auto it = comportToIndex.begin(); it != comportToIndex.end(); ++it)
		{
			BlueMsg msg = bluetooth_GetMsg(currentGun[it->second].index);
			if (msg.count != currentGun[it->second].gunBulletUsingNumber)
			{
				lastComport = it->first;
				currentBlueToothLevel = currentGun[it->second].gunType;
				qDebug() << lastComport << " comport shoot";
				if (currentGun[it->second].gunType == Gun)
					gunSoundPlayer->play();
				else if (currentGun[it->second].gunType == Chili)
					chiliSoundPlayer->play();
				else if (currentGun[it->second].gunType == Stick)
					stickSoundPlayer->play();
				currentGun[it->second].gunBulletUsingNumber = msg.count;
				if (currentGun[it->second].gunBulletUsingNumber > currentGun[it->second].gunBulletSum)//子弹用完
				{
					bluetooth_PutMsg(currentGun[it->second].index, true);//闭锁;
				}
			}
		}
	}
}*/