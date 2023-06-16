#pragma once
#include <string>
#include <mutex>
#include <thread>
#include "IniFile.h"
using namespace std;

class SystemSetting
{
public:
	
	static SystemSetting* getInstance();

	void Save();

	void setTrainPath(string val);
	string getTrainPath();

	void setSaveTestPath(string val);
	string getSaveTestPath();

	void setBrand(string val);
	string getBrand();

	void setCameraNum(int val);
	int getCameraNum();

	void setWork1(bool val);
	bool getWork1();

	void setWork2(bool val);
	bool getWork2();

	void setWork3(bool val);
	bool getWork3();

	void setWorkTime1(string val);
	string getWorkTime1();
			
	void setWorkTime2(string val);
	string getWorkTime2();
				
	void setWorkTime3(string val);
	string getWorkTime3();

	void setStyle(int val);
	int getStyle();

private:
	bool Load();

public:
	static string FilePath;
private:
	static std::mutex m_Mutex;
	static SystemSetting* m_Instance;//实例

	SystemSetting();

	string m_TrainPath;
	string m_SaveTestPath;
	string m_Brand;
	int m_CameraNum;

	bool m_Work1;
	bool m_Work2;
	bool m_Work3;
	string m_WorkTime1;
	string m_WorkTime2;
	string m_WorkTime3;

	int m_Style;

	INIFile m_IniFile;//iniFile：ini文件类


};



