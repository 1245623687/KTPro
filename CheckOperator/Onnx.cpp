#include "Onnx.h"

OnnxGloable ::GC OnnxGloable ::m_gc;
std::mutex OnnxGloable::m_onnxMutex[MAXONNXTHREAD];
OnnxGloable* OnnxGloable::m_onnxInstance = nullptr;

inline std::wstring String2WideString(const std::string& s)
{
	std::string curlLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	wchar_t* _Dest = new wchar_t[_Dsize];
	size_t i;
	mbstowcs_s(&i, _Dest, _Dsize, _Source, s.size());
	std::wstring result = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, curlLocale.c_str());
	return result;
}

Onnx::Onnx()
{
	m_intIsWidth = 640;
	m_intIsHeight = 640;
	m_intIsChannel = 3;
	m_intIsBatchSize = 1;
    m_classNum = 1;
	m_confThres = 0.25;
	m_iouThres = 0.45;
}

Onnx::~Onnx()
{

}

void Onnx::read_onnx(string path)
{
	m_env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test");//创建onnxruntime运行环境

   // initialize session options if needed
	Ort::SessionOptions session_options;//创建会话设置选项
    //使用CPU
    //session_options.SetIntraOpNumThreads(12);//设置运行线程数
    //使用GPU
    OrtSessionOptionsAppendExecutionProvider_CUDA(session_options, 0);

	session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);//设置会话属性

    m_session = Ort::Session(m_env, String2WideString(path).c_str(), session_options);//创建会话
   // m_session = Ort::Session(m_env, L"hengyan_best.onnx", session_options);//创建会话

	//cuda should be here
	auto m_prociders = Ort::GetAvailableProviders();

	//配置输入节点
	// print model input layer (node names, types, shape etc.)
	Ort::AllocatorWithDefaultOptions allocator;

	// print number of model input nodes
	//size_t num_input_nodes = m_session->GetInputCount();//yolov5 just one input
	size_t num_input_nodes = m_session.GetInputCount();//yolov5 just one input

	//printf("Number of inputs = %zu\n", num_input_nodes);
	m_input_node_names.resize(num_input_nodes);
	//std::vector<int64_t> input_node_dims;  // simplify... this model has only 1 input node {1, 3, 224, 224}.
	// Otherwise need vector<vector<>>

	//std::cout << "num_input_nodes = " << num_input_nodes << std::endl;
	// iterate over all input nodes
	for (int i = 0; i < num_input_nodes; i++) {

		//char* input_name = m_session->GetInputName(i, allocator);
		char* input_name = m_session.GetInputName(i, allocator);

		m_input_node_names[i] = input_name;

		//Ort::TypeInfo type_info = m_session->GetInputTypeInfo(i);
		Ort::TypeInfo type_info = m_session.GetInputTypeInfo(i);
		auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

		// print input shapes/dims
		m_input_node_dims = tensor_info.GetShape();

	}
	//配置输出节点
	m_output_node_names.push_back("output");

	//获取输入name
	const char* input_name = m_session.GetInputName(0, allocator);
	//std::cout << "input_name:" << input_name << std::endl;
	//获取输出name
	const char* output_name = m_session.GetOutputName(0, allocator);
	//std::cout << "output_name: " << output_name << std::endl;

	auto input_dims = m_session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
	auto output_dims = m_session.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();


	m_intIsBatchSize = input_dims[0];
	m_intIsChannel = input_dims[1];
	m_intIsWidth = input_dims[2];
	m_intIsHeight = input_dims[3];
	m_classNum = output_dims[2] - 5;


	//下面进行warm up,创建一个输入tensor跑一遍网络进行warm up

	//创建一个矩阵数据,尺寸是模型输入尺寸
	cv::Mat imgRGBFLoat(m_intIsWidth, m_intIsHeight, CV_32FC3);

	//下面进行
		//图像预处理
	cv::Mat preprocessedImage;
	cv::dnn::blobFromImage(imgRGBFLoat, preprocessedImage);//HWC->CHW

	auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	//创建输入tensor
	Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, reinterpret_cast<float*>(preprocessedImage.data),
		m_intIsWidth * m_intIsHeight * m_intIsChannel * m_intIsBatchSize, m_input_node_dims.data(), 4);

	auto output_tensors = m_session.Run(Ort::RunOptions{ nullptr }, m_input_node_names.data(), &input_tensor, 1, m_output_node_names.data(), 1);

}

void Onnx::setInPutSize(int width, int height, int channel)
{
	m_intIsWidth = width;
	m_intIsHeight = height;
    m_intIsChannel = channel;
}

//Onnx *Onnx::getInstance()
//{
//    //m_onnxMutex.lock();
//    if(m_onnxInstance==NULL)
//    {
//        m_onnxInstance=new Onnx();
//    }
//    //m_onnxMutex.unlock();
//    return m_onnxInstance;

//}


vector<vector<BBoxInfo>> Onnx::non_max_suppression_onnx(cv::Mat Img, Ort::Value & input_tensor, float conf_thres = 0.25, float iou_thres = 0.45, int class_num = 1)
{
	//prediction, conf_thres=0.25, iou_thres=0.45, classes=None, agnostic=False, labels=()
	int min_wh = 2, max_wh = 4096;
	int max_det = 300, max_nms = 30000;
	//float time_limit = 10.0;
	//bool redundant = true;
	bool multi_label = false;
	if (class_num >= 1) {
		multi_label = true;
	}

	const float * prob = input_tensor.GetTensorData<float>();//tensor
	int obj_count = input_tensor.GetTensorTypeAndShapeInfo().GetElementCount() / (class_num + 5);
	std::vector<cv::Rect> boxes_vec;
	std::vector<int> clsIdx_vec;
	std::vector<float> scores_vec;
	std::vector<int>boxIdx_vec;
	//std::cout << "obj_count is " << obj_count << std::endl;

	for (int i = 0; i < obj_count; i++) 
	{
		if (*(prob + i * (5 + class_num) + 4) < conf_thres)
			continue;
		if (multi_label) {
			for (int cls_idx = 0; cls_idx < class_num; cls_idx++) {
				float x1 = *(prob + i * (5 + class_num)) - *(prob + i * (5 + class_num) + 2) / 2;
				float y1 = *(prob + i * (5 + class_num) + 1) - *(prob + i * (5 + class_num) + 3) / 2;
				float x2 = *(prob + i * (5 + class_num)) + *(prob + i * (5 + class_num) + 2) / 2;
				float y2 = *(prob + i * (5 + class_num) + 1) + *(prob + i * (5 + class_num) + 3) / 2;
				// mix_conf = obj_conf * cls_conf > conf_thres
				float mix_conf = (*(prob + i * (5 + class_num) + 4)) * (*(prob + i * (5 + class_num) + 5 + cls_idx));
				if (mix_conf > conf_thres) {
					boxes_vec.push_back(cv::Rect(x1 + cls_idx * max_wh,
						y1 + cls_idx * max_wh,
						x2 - x1,
						y2 - y1)
					);
					//scores_vec.push_back((*(prob+i*(5+class_num)+4)));
					scores_vec.push_back(mix_conf);
					//std::cout<<"score "<<(*(prob+i*(5+class_num)+5+cls_idx))<<std::endl;
					clsIdx_vec.push_back(cls_idx);
					//cv::Rect
				}
			}

		}
		else {

		}

	}//obj_count


	float width_scale = (float)m_intIsWidth / Img.cols;
	float height_scale = (float)m_intIsHeight / Img.rows;

	cv::dnn::NMSBoxes(boxes_vec, scores_vec, 0.0f, iou_thres, boxIdx_vec);
	vector<vector<BBoxInfo>> det_boxes;
	det_boxes.resize(m_classNum);
	for (int i = 0; i < boxIdx_vec.size(); i++) {
		BBoxInfo det_box_tmp;
		int x1 = (boxes_vec[boxIdx_vec[i]].x - clsIdx_vec[boxIdx_vec[i]] * max_wh) / width_scale;
		int y1 = (boxes_vec[boxIdx_vec[i]].y - clsIdx_vec[boxIdx_vec[i]] * max_wh) / height_scale;
		int w = boxes_vec[boxIdx_vec[i]].width / width_scale;
		int h = boxes_vec[boxIdx_vec[i]].height / height_scale;
		det_box_tmp.prob = scores_vec[boxIdx_vec[i]];
		det_box_tmp.classId = clsIdx_vec[boxIdx_vec[i]];
        det_box_tmp.rect = cv::Rect(x1, y1, w, h);
		det_boxes[det_box_tmp.classId].push_back(det_box_tmp);
	}

	return det_boxes;

}

void Onnx::detect(cv::Mat Img, vector<vector<BBoxInfo>> &det_boxes)
{
	//auto input_w_h = std::tuple<int,int>(std::get<0>(m_inputSize),std::get<1>(m_inputSize));
	//对输入的图片进行按最长边比例缩放,然后填充短边至模型所需要的输入
    cv::Mat resized;

    if(Img.cols!=m_intIsWidth||Img.rows!=m_intIsHeight)
    {
         cv::resize(Img, resized, cv::Size(m_intIsWidth, m_intIsHeight));
    }
    else
    {
        resized=Img;
    }





	cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);
	//cv::imwrite("resized.jpg",resized);
	//std::cout<<"getOutput 1"<<std::endl;
	cv::Mat imgRGBFLoat;
	//std::cout<<"getOutput 2"<<std::endl;
	//图片类型进行转换，然后除以255进行归一化
	resized.convertTo(imgRGBFLoat, CV_32F, 1.0 / 255);//preprocess 

	//图片填充到opencv的blob然后进行通道变换
	cv::Mat preprocessedImage;
	cv::dnn::blobFromImage(imgRGBFLoat, preprocessedImage);//HWC->CHW
	//std::cout<<"getOutput 3"<<std::endl;
	// create input tensor object from data values
	auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
	/*   Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_tensor_values.data(), input_tensor_size, input_node_dims.data(), 4); */
		//图片数据填充进输入的tensor
	Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, reinterpret_cast<float*>(preprocessedImage.data),
		m_intIsHeight * m_intIsWidth * m_intIsChannel * m_intIsBatchSize, m_input_node_dims.data(), 4);

	//执行推理获取输出的tensor
	auto output_tensors = m_session.Run(Ort::RunOptions{ nullptr }, m_input_node_names.data(), &input_tensor, 1, m_output_node_names.data(), 1);

	//get the output and format boxes
	//对输出tensor进行后处理获取所需要的格式数据
	det_boxes.clear();
    float confThres=m_confThres>0.1?m_confThres:0.1;
        //cv::imwrite("./save.bmp", Img);
    det_boxes = non_max_suppression_onnx(Img, output_tensors[0], confThres, m_iouThres, m_classNum);

}

OnnxGloable::OnnxGloable()
{

}

OnnxGloable::~OnnxGloable()
{

}

OnnxGloable *OnnxGloable::getInstance()
{
    //m_onnxMutex.lock();
    if(m_onnxInstance==NULL)
    {
        m_onnxInstance=new OnnxGloable();
    }
    //m_onnxMutex.unlock();
    return m_onnxInstance;
}
