///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, Carnegie Mellon University and University of Cambridge,
// all rights reserved.
//
// ACADEMIC OR NON-PROFIT ORGANIZATION NONCOMMERCIAL RESEARCH USE ONLY
//
// BY USING OR DOWNLOADING THE SOFTWARE, YOU ARE AGREEING TO THE TERMS OF THIS LICENSE AGREEMENT.  
// IF YOU DO NOT AGREE WITH THESE TERMS, YOU MAY NOT USE OR DOWNLOAD THE SOFTWARE.
//
// License can be found in OpenFace-license.txt
//
//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite at least one of the following works:
//
//       OpenFace: an open source facial behavior analysis toolkit
//       Tadas Baltrušaitis, Peter Robinson, and Louis-Philippe Morency
//       in IEEE Winter Conference on Applications of Computer Vision, 2016  
//
//       Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
//       Erroll Wood, Tadas Baltrušaitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling 
//       in IEEE International. Conference on Computer Vision (ICCV),  2015 
//
//       Cross-dataset learning and person-speci?c normalisation for automatic Action Unit detection
//       Tadas Baltrušaitis, Marwa Mahmoud, and Peter Robinson 
//       in Facial Expression Recognition and Analysis Challenge, 
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015 
//
//       Constrained Local Neural Fields for robust facial landmark detection in the wild.
//       Tadas Baltrušaitis, Peter Robinson, and Louis-Philippe Morency. 
//       in IEEE Int. Conference on Computer Vision Workshops, 300 Faces in-the-Wild Challenge, 2013.    
//
///////////////////////////////////////////////////////////////////////////////

//  Header for all external CLNF/CLM-Z/CLM methods of interest to the user
#ifndef __LANDMARK_DETECTOR_UTILS_h_
#define __LANDMARK_DETECTOR_UTILS_h_

// OpenCV includes
#include <opencv2/core/core.hpp>

#include "LandmarkDetectorModel.h"

using namespace std;

namespace LandmarkDetector
{
	//===========================================================================	
	// Defining a set of useful utility functions to be used within CLNF


	//=============================================================================================
	// Helper functions for parsing the inputs
	//=============================================================================================
	void get_video_input_output_params(vector<string> &input_video_file, vector<string> &output_files, string &output_codec, vector<string> &arguments);

	void get_camera_params(int &device, float &fx, float &fy, float &cx, float &cy, vector<string> &arguments);

	void get_image_input_output_params(vector<string> &input_image_files, vector<string> &output_feature_files, vector<string> &output_pose_files, vector<string> &output_image_files,
		vector<cv::Rect_<double>> &input_bounding_boxes, vector<string> &arguments);

	//===========================================================================
	// Fast patch expert response computation (linear model across a ROI) using normalised cross-correlation
	//===========================================================================
	// This is a modified version of openCV code that allows for precomputed dfts of templates and for precomputed dfts of an image
	// _img is the input img, _img_dft it's dft (optional), _integral_img the images integral image (optional), squared integral image (optional), 
	// templ is the template we are convolving with, templ_dfts it's dfts at varying windows sizes (optional),  _result - the output, method the type of convolution
	void matchTemplate_m( const cv::Mat_<float>& input_img, cv::Mat_<double>& img_dft, cv::Mat& _integral_img, cv::Mat& _integral_img_sq, const cv::Mat_<float>&  templ, map<int, cv::Mat_<double> >& templ_dfts, cv::Mat_<float>& result, int method );

	//===========================================================================
	// Point set and landmark manipulation functions
	//===========================================================================
	// Using Kabsch's algorithm for aligning shapes
	//This assumes that align_from and align_to are already mean normalised
	cv::Matx22d AlignShapesKabsch2D(const cv::Mat_<double>& align_from, const cv::Mat_<double>& align_to );

	//=============================================================================
	// Basically Kabsch's algorithm but also allows the collection of points to be different in scale from each other
	cv::Matx22d AlignShapesWithScale(cv::Mat_<double>& src, cv::Mat_<double> dst);

	//===========================================================================
	// Visualisation functions
	//===========================================================================
	vector<cv::Point2d> CalculateVisibleLandmarks(const cv::Mat_<double>& shape2D, const cv::Mat_<int>& visibilities);
	vector<cv::Point2d> CalculateVisibleLandmarks(const CLNF& clnf_model);
	vector<cv::Point2d> CalculateVisibleEyeLandmarks(const CLNF& clnf_model);

	vector<cv::Point2d> CalculateAllLandmarks(const cv::Mat_<double>& shape2D);
	vector<cv::Point2d> CalculateAllLandmarks(const CLNF& clnf_model);
	vector<cv::Point2d> CalculateAllEyeLandmarks(const CLNF& clnf_model);
	void DrawLandmarks(cv::Mat img, vector<cv::Point> landmarks);

	void Draw(cv::Mat img, const cv::Mat_<double>& shape2D, const cv::Mat_<int>& visibilities);
	void Draw(cv::Mat img, const cv::Mat_<double>& shape2D);
	void Draw(cv::Mat img, const CLNF& clnf_model);

	//============================================================================
	// Face detection helpers
	//============================================================================

	// Face detection using Haar cascade classifier
	bool DetectFaces(vector<cv::Rect_<double> >& o_regions, const cv::Mat_<uchar>& intensity);
	bool DetectFaces(vector<cv::Rect_<double> >& o_regions, const cv::Mat_<uchar>& intensity, cv::CascadeClassifier& classifier);
	// The preference point allows for disambiguation if multiple faces are present (pick the closest one), if it is not set the biggest face is chosen
	bool DetectSingleFace(cv::Rect_<double>& o_region, const cv::Mat_<uchar>& intensity, cv::CascadeClassifier& classifier, const cv::Point preference = cv::Point(-1,-1));

	// Face detection using HOG-SVM classifier
	bool DetectFacesHOG(vector<cv::Rect_<double> >& o_regions, const cv::Mat_<uchar>& intensity, std::vector<double>& confidences);
	bool DetectFacesHOG(vector<cv::Rect_<double> >& o_regions, const cv::Mat_<uchar>& intensity, dlib::frontal_face_detector& classifier, std::vector<double>& confidences);
	// The preference point allows for disambiguation if multiple faces are present (pick the closest one), if it is not set the biggest face is chosen
	bool DetectSingleFaceHOG(cv::Rect_<double>& o_region, const cv::Mat_<uchar>& intensity, dlib::frontal_face_detector& classifier, double& confidence, const cv::Point preference = cv::Point(-1,-1));

	//============================================================================
	// Matrix reading functionality
	//============================================================================

	// Reading a matrix written in a binary format
	void ReadMatBin(std::ifstream& stream, cv::Mat &output_mat);

	// Reading in a matrix from a stream
	void ReadMat(std::ifstream& stream, cv::Mat& output_matrix);

	// Skipping comments (lines starting with # symbol)
	void SkipComments(std::ifstream& stream);

}
#endif
