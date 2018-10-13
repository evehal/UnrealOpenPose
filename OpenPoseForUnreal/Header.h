#pragma once

#if defined (EXPORTBUILD)    
# define _DLLExport __declspec (dllexport)    
# else    
# define _DLLExport __declspec (dllimport)    
#endif    


extern "C" _DLLExport unsigned char*  extract_from_image(int logging_level, char* image_path,
	char* model_pose, char* model_folder, char* net_resolution,
	char* output_resolution, int num_gpu_start, float scale_gap, int scale_number,
	bool disable_blending, float render_threshold, float alpha_pose,bool show_frame);
	
