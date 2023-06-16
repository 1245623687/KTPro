#ifndef INIFILE_H
#define INIFILE_H

#include"windows.h"
#include <map>
#include <string>
#include <vector>
#include<QDebug>

using namespace std;

typedef map<string,string> mapDirectory;
typedef map<string,string>::iterator itmapDirectory;

typedef struct _Section
{
	mapDirectory dMap;
	string SectionName;
}Section;


typedef vector<Section> vecMap;
typedef vector<Section>::iterator itVector;

class INIFile
{
public:
	inline bool IsCreated(){return m_bCreated;};
	bool Save(string filename);
	bool Save();
	bool Open(string filename);
	bool GetValue(string strSection,string varName,string &varvalue);
	bool SetValue(string strSection,string varName = "",string varvalue = "",bool breplace = true);
	bool ProcessLine(string strLine);
	INIFile();
	INIFile(string filename);
	bool Create(string filename);
//private:
	vecMap m_FileContent; 
	bool m_bCreated;
	string m_initPath;
    string m_initPathBackup;
};
#endif
