#pragma once
//add by wzy
//�����������
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

//ǹ�㲶׽�������
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
	int gunComport;//�˿ں�
	int gunBulletSum;//�ӵ�����
	int gunBulletUsingNumber;//�ӵ�ʹ����
	int gunOffsetX;//У׼����
	int gunOffsetY;
	int index;//������������˳�򣬴�0��ʼ
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
//������ţ���һλΪ����ͷ����2λΪ2��ǹ
std::string equipmentString = "000";
//�Ƿ�����豸
bool equipmentTest = false;
//����
GunInfo currentGun[2];
//�����ӵ������˿ںŶ�Ӧ�±�
std::map<int, int> comportToIndex;
/*
* ��ȡGuns.xml
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
					if (GunInfoNumber == 2)//��ȡǰ2�ѱ�ΪGUN������
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
* ��ʼ������ģ��
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
					bluetooth_PutMsg(currentGun[i].index, false);//����
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
* ��ʼ������ͷ��ǹ��ģ��
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
* �ر�����ģ��
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
* �ر�����ͷ��ǹ��ģ��
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
*�����źż�⣬�����ӵ������Ʋ���Ҫ���������źţ����ע�ʹ˺���
*/
/*void blueToothCheck()
{
	if (comportToIndex.empty())
	{
		//��ʱ����һ�������źŹ�������
		lastComport = -1;//������
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
				if (currentGun[it->second].gunBulletUsingNumber > currentGun[it->second].gunBulletSum)//�ӵ�����
				{
					bluetooth_PutMsg(currentGun[it->second].index, true);//����;
				}
			}
		}
	}
}*/