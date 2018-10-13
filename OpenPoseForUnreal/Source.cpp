
#define  EXPORTBUILD   

#include "Header.h"
// ------------------------- OpenPose Library Tutorial - Pose - Example 1 - Extract from Image -------------------------
// This first example shows the user how to:
// 1. Load an image (`filestream` module)
// 2. Extract the pose of that image (`pose` module)
// 3. Render the pose on a resized copy of the input image (`pose` module)
// 4. Display the rendered pose (`gui` module)
// In addition to the previous OpenPose modules, we also need to use:
// 1. `core` module: for the Array<float> class that the `pose` module needs
// 2. `utilities` module: for the error & logging functions, i.e. op::error & op::log respectively

// 3rdparty dependencies
// GFlags: DEFINE_bool, _int32, _int64, _uint64, _double, _string
#include <gflags/gflags.h>
// Allow Google Flags in Ubuntu 14
#ifndef GFLAGS_GFLAGS_H_
namespace gflags = google;
#endif
// OpenPose dependencies
#include <openpose/core/headers.hpp>
#include <openpose/filestream/headers.hpp>
#include <openpose/gui/headers.hpp>
#include <openpose/pose/headers.hpp>
#include <openpose/utilities/headers.hpp>

    
#pragma comment(lib, "openpose.lib")    
#pragma comment(lib, "gflags.lib") 
#pragma comment(lib, "opencv_world310.lib") 



unsigned char* extract_from_image(int logging_level, char* image_path, char* model_pose,
	char* model_folder, char* net_resolution,
	char* output_resolution, int num_gpu_start, float scale_gap, int scale_number,
	bool disable_blending, float render_threshold, float alpha_pose, bool show_frame)
{

	
	op::log("OpenPose Library Tutorial - Example 1.", op::Priority::High);
	// ------------------------- INITIALIZATION -------------------------
	// Step 1 - Set logging level
	// - 0 will output all the logging messages
	// - 255 will output nothing
	op::check(0 <= logging_level && logging_level <= 255, "Wrong logging_level value.",
		__LINE__, __FUNCTION__, __FILE__);
	op::ConfigureLog::setPriorityThreshold((op::Priority)logging_level);
	op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);
	// Step 2 - Read Google flags (user defined configuration)
	// outputSize
	const auto outputSize = op::flagsToPoint(output_resolution, "-1x-1");
	// netInputSize
	const auto netInputSize = op::flagsToPoint(net_resolution, "-1x368");
	// poseModel
	const auto poseModel = op::flagsToPoseModel(model_pose);
	// Check no contradictory flags enabled
	if (alpha_pose < 0. || alpha_pose > 1.)
		op::error("Alpha value for blending must be in the range [0,1].", __LINE__, __FUNCTION__, __FILE__);
	if (scale_gap <= 0. && scale_number > 1)
		op::error("Incompatible flag configuration: scale_gap must be greater than 0 or scale_number = 1.",
			__LINE__, __FUNCTION__, __FILE__);
	// Logging
	op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);
	// Step 3 - Initialize all required classes
	op::ScaleAndSizeExtractor scaleAndSizeExtractor(netInputSize, outputSize, scale_number, scale_gap);
	op::CvMatToOpInput cvMatToOpInput{ poseModel };
	op::CvMatToOpOutput cvMatToOpOutput;
	op::PoseExtractorCaffe poseExtractorCaffe{ poseModel, model_folder, num_gpu_start };
	op::PoseCpuRenderer poseRenderer{ poseModel, (float)render_threshold, !disable_blending,
		(float)alpha_pose };
	op::OpOutputToCvMat opOutputToCvMat;
	op::FrameDisplayer frameDisplayer{ "OpenPose Tutorial - Example 1", outputSize };

	cv::Mat inputImage = op::loadImage(image_path, CV_LOAD_IMAGE_COLOR);
	if (inputImage.empty())
		op::error("Could not open or find the image: " + std::string(image_path), __LINE__, __FUNCTION__, __FILE__);

	
	// Step 4 - Initialize resources on desired thread (in this case single thread, i.e. we init resources here)
	poseExtractorCaffe.initializationOnThread();
	poseRenderer.initializationOnThread();

	// ------------------------- POSE ESTIMATION AND RENDERING -------------------------
	// Step 1 - Read and load image, error if empty (possibly wrong path)
	// Alternative: cv::imread(FLAGS_image_path, CV_LOAD_IMAGE_COLOR);
	const op::Point<int> imageSize{ inputImage.cols, inputImage.rows };
	// Step 2 - Get desired scale sizes
	std::vector<double> scaleInputToNetInputs;
	std::vector<op::Point<int>> netInputSizes;
	double scaleInputToOutput;
	op::Point<int> outputResolution;
	std::tie(scaleInputToNetInputs, netInputSizes, scaleInputToOutput, outputResolution)
		= scaleAndSizeExtractor.extract(imageSize);
	// Step 3 - Format input image to OpenPose input and output formats
	const auto netInputArray = cvMatToOpInput.createArray(inputImage, scaleInputToNetInputs, netInputSizes);
	auto outputArray = cvMatToOpOutput.createArray(inputImage, scaleInputToOutput, outputResolution);
	// Step 4 - Estimate poseKeypoints
	poseExtractorCaffe.forwardPass(netInputArray, imageSize, scaleInputToNetInputs);
	const auto poseKeypoints = poseExtractorCaffe.getPoseKeypoints();
	// Step 5 - Render poseKeypoints
	poseRenderer.renderPose(outputArray, poseKeypoints, scaleInputToOutput);
	// Step 6 - OpenPose output format to cv::Mat
	auto outputImage = opOutputToCvMat.formatToCvMat(outputArray);

	// ------------------------- SHOWING RESULT AND CLOSING -------------------------
	// Step 1 - Show results
	if(show_frame)
		frameDisplayer.displayFrame(outputImage, 0); // Alternative: cv::imshow(outputImage) + cv::waitKey(0)
												 // Step 2 - Logging information message
	op::log("Example 1 successfully finished.", op::Priority::High);
	// Return successful message
	return 0;
	
}

int main() {
	extract_from_image(0, "D:/visual studio 2015/openpose130/examples/media/COCO_val2014_000000000192.jpg", "COCO", "D:/visual studio 2015/openpose130/models/",
		"-1x368", "-1x-1", 0, 0.3f, 1, false, 0.05f, 0.6f, true);
	return 0;
}
