#include "SystemSetting.h"


mutex SystemSetting::m_Mutex;
SystemSetting* SystemSetting::m_Instance = nullptr;
string SystemSetting::FilePath = "";

SystemSetting::SystemSetting()
{
	Load();
}


SystemSetting* SystemSetting::getInstance()
{
	m_Mutex.lock();
	if (m_Instance == NULL) {
		m_Instance = new SystemSetting();
	}
	m_Mutex.unlock();
	return m_Instance;
}


//加载ini文件中值,注意修改默认值
bool  SystemSetting::Load()
{
	if (SystemSetting::FilePath=="")
	{
		return false;
	}
	if (!this->m_IniFile.Open(FilePath)) return false;
	bool f;
	string val;

	f=this->m_IniFile.GetValue("Path", "SaveTestPath", val);
	if (f) setSaveTestPath(val);
	else setSaveTestPath("Default");

	f = this->m_IniFile.GetValue("Path", "TrainPath", val);
	if (f) setTrainPath(val);
	else setTrainPath("Default");

	f = this->m_IniFile.GetValue("Brand", "Brand", val);
	if (f) setBrand(val);
	else setBrand("Default");

	f = this->m_IniFile.GetValue("Camera", "CameraNumber", val);
	if (f)setCameraNum(atoi(val.c_str()));
	else setCameraNum(6);//相机数量

	f = this->m_IniFile.GetValue("Work", "Work1", val);
	if (f) setWork1(val=="true"?true:false);
	else setWork1(true);

	f = this->m_IniFile.GetValue("Work", "Work2", val);
	if (f) setWork2(val == "true" ? true : false);
	else setWork2(true);

	f = this->m_IniFile.GetValue("Work", "Work3", val);
	if (f) setWork3(val == "true" ? true : false);
	else setWork3(true);

	f = this->m_IniFile.GetValue("Work", "WorkTime1", val);
	if (f) setWorkTime1(val);
	else setWorkTime1("07:00");

	f = this->m_IniFile.GetValue("Work", "WorkTime2", val);
	if (f) setWorkTime2(val);
	else setWorkTime2("16:00");

	f = this->m_IniFile.GetValue("Work", "WorkTime3", val);
	if (f) setWorkTime3(val);
	else setWorkTime3("23:00");

	f = this->m_IniFile.GetValue("Save", "Style", val);
	if (f) setStyle(atoi(val.c_str()));
	else setStyle(0);
	return true;
}

void SystemSetting::Save()
{
	m_IniFile.SetValue("Path", "SaveTestPath", this->getSaveTestPath());
	m_IniFile.SetValue("Path", "TrainPath", this->getTrainPath());
	m_IniFile.SetValue("Brand", "Brand", this->getBrand());
	m_IniFile.SetValue("Camera", "CameraNumber", to_string(this->getCameraNum()));
	m_IniFile.SetValue("Work", "Work1", this->getWork1()?"true":"false");
	m_IniFile.SetValue("Work", "Work2", this->getWork2() ? "true" : "false");
	m_IniFile.SetValue("Work", "Work3", this->getWork3() ? "true" : "false");
	m_IniFile.SetValue("Work", "WorkTime1", this->getWorkTime1());
	m_IniFile.SetValue("Work", "WorkTime2", this->getWorkTime2());
	m_IniFile.SetValue("Work", "WorkTime3", this->getWorkTime3());
	m_IniFile.SetValue("Save", "Style", to_string(this->getStyle()));

	m_IniFile.Save();
}


void SystemSetting::setTrainPath(string val)
{
	this->m_TrainPath = val;
}
string SystemSetting::getTrainPath()
{
	return this->m_TrainPath;
}


void SystemSetting::setSaveTestPath(string val)
{
	this->m_SaveTestPath = val;
}
string SystemSetting::getSaveTestPath()
{
	return this->m_SaveTestPath;
}


void SystemSetting::setBrand(string val)
{
	this->m_Brand = val;
}
string SystemSetting::getBrand()
{
	return this->m_Brand;
}


void SystemSetting::setCameraNum(int val)
{
	this->m_CameraNum = val;
}

int SystemSetting::getCameraNum()
{
	return this->m_CameraNum;
}

void SystemSetting::setWork1(bool val)
{
	this->m_Work1 = val;
}
bool SystemSetting::getWork1()
{
	return this->m_Work1;
}

void SystemSetting::setWork2(bool val)
{
	this->m_Work2 = val;
}
bool SystemSetting::getWork2()
{
	return this->m_Work2;
}

void SystemSetting::setWork3(bool val)
{
	this->m_Work3 = val;
}
bool SystemSetting::getWork3()
{
	return this->m_Work3;
}

void SystemSetting::setWorkTime1(string val)
{
	this->m_WorkTime1 = val;
}
string SystemSetting::getWorkTime1()
{
	return this->m_WorkTime1;
}

void SystemSetting::setWorkTime2(string val)
{
	this->m_WorkTime2 = val;
}
string SystemSetting::getWorkTime2()
{
	return this->m_WorkTime2;
}

void SystemSetting::setWorkTime3(string val)
{
	this->m_WorkTime3 = val;
}
string SystemSetting::getWorkTime3()
{
	return this->m_WorkTime3;
}

void SystemSetting::setStyle(int val)
{
	this->m_Style = val;
}
int SystemSetting::getStyle()
{
	return this->m_Style;
}