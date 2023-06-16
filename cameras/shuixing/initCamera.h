#pragma once
#include"IGXFactory.h"
#include<list>
#include<string>

#define DLL_EXPORT extern "C" _declspec(dllexport)


//DLL_EXPORT int initCamera(std::string macAdd);


DLL_EXPORT int initCamera(const char* macAdd);
