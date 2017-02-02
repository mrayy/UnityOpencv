#pragma once




extern "C" void* DN_CreateDetectorInstance(char *datacfg, char *cfgfile, char *weightfile, float threshold, int gpuIndex);
extern "C" int DN_PredictDetectorLabels(void* instance, float* img, int w, int h);
extern "C" void DN_CopyDetectorBoxes(void* instance, float*boxes, int* classes, float* probs);
extern "C" void DN_DestroyDetectorInstance(void* instance);
extern "C" char* DN_GetClassName(void* instance, int classID);