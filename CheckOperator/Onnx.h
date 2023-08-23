#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
#include <onnxruntime_cxx_api.h>

//onnx 1.6.0
//#include "cuda_provider_factory.h"

#include <cuda_runtime_api.h>
#include<mutex>

using namespace std;
//using namespace cv;
//using namespace dnn;

#define MAXONNXTHREAD 4

struct BBoxInfo
{
    cv::Rect rect;
	int classId; // For coco benchmarking
	float prob;
    int boxID;
};

class Onnx
{
public:
	Onnx();
	~Onnx();
	void read_onnx(string path);
    void detect(cv::Mat Img, vector<vector<BBoxInfo>> &det_boxes);

	void setInPutSize(int width, int height, int channel);

    void setConfidence(float confidence){m_confThres=confidence;}

//    static Onnx* getInstance();
//    static std::mutex m_onnxMutex;

        bool m_bIsValid=false;

protected:
    vector<vector<BBoxInfo>> non_max_suppression_onnx(cv::Mat Img, Ort::Value & input_tensor, float conf_thres, float iou_thres, int class_num);

private:

//    static Onnx* m_onnxInstance;
//    class GC
//    {
//    public:
//        ~GC()
//        {
//            if(m_onnxInstance!=NULL)
//            {
////                delete  m_onnxInstance;
////                m_onnxInstance=NULL;
//            }
//        }
//    };
//    static GC m_gc;


	int m_intIsWidth = 640;
	int m_intIsHeight = 640;
	int m_intIsChannel = 3;
	int m_intIsBatchSize = 1;

	std::tuple<int, int, int> m_inputSize;
	std::tuple<int, int>m_input_w_h;
	//Ort::Session * m_session ;
	Ort::Session  m_session{ nullptr };
	std::vector<int64_t> m_input_node_dims;
	std::vector<const char*> m_input_node_names;
	std::vector<const char*> m_output_node_names;
	int m_classNum = 1;
	float m_confThres = 0.25;
	float m_iouThres = 0.45;
    Ort::Env m_env;//onnx环境,最好伴随整个检测生命周期


};

 class  OnnxGloable
{
public:
    OnnxGloable();
    ~OnnxGloable();
    static OnnxGloable* getInstance();
    static std::mutex m_onnxMutex[MAXONNXTHREAD];
    Onnx onnxArray[MAXONNXTHREAD];

private:
    int onnxThread;
    static OnnxGloable* m_onnxInstance;
    class GC
    {
    public:
        ~GC()
        {
            if(m_onnxInstance!=NULL)
            {
//                delete  m_onnxInstance;
//                m_onnxInstance=NULL;
            }
        }
    };
    static GC m_gc;
};



