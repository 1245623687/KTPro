#pragma once



#include <iostream>
#include<string>
#include <fstream>
#include <vector>
#include"dsbase.h"
using namespace std;



#ifdef __cplusplus //
extern "C" 
{
#endif
    __declspec(dllexport) void saveRecord(string fileName, string productName, int allNum, int ngNum[CAMERANUM_MAX], int cameraNum, int maxChannel, int data[16][128]);

	__declspec(dllexport) void getRecord(string fileName, char* productName, int* allNum, int* ngNum, int *cameraNum, int data[16][128]);

    __declspec(dllexport) int getSimpleRecord(string fileName, char* productName, int* allNum, int* ngNum);
#ifdef __cplusplus
}
#endif

