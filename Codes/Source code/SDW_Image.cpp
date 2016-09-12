// By Santi; www.shadowwares.com; 19 Feb, 2010
#include "stdafx.h"
#include "SDW_Image.h"
int C_SDW_Image::DisplayInitial(HANDLE hPictureBox)
{
	this->hPictureBox = hPictureBox;
	hDLL = LoadLibrary(_T(SDW_IMAGE_DLLNAME));
	if(hDLL == NULL){
		return(-1);	// Cannot load the library
	}
	DisplayIplImageAddr = (SDW_DisplayIplImage)GetProcAddress(hDLL, (LPCSTR)"SDW_DisplayIplImage");
	DisplayFromFileAddr = (SDW_DisplayFromFile)GetProcAddress(hDLL, (LPCSTR)"SDW_DisplayFromFile");
	if(DisplayIplImageAddr==NULL || DisplayFromFileAddr==NULL){
		return(-2);	// Cannot get process address of the library
	}
	return(0);		// Success
}

int C_SDW_Image::DisplayFromFile(char *ImageName)
{
	return(DisplayFromFileAddr(ImageName, this->hPictureBox, SDW_IMAGE_VERSION));
}

int C_SDW_Image::DisplayIplImage(IplImage *iplimage)
{
	return(DisplayIplImageAddr(iplimage, this->hPictureBox, SDW_IMAGE_VERSION));
}