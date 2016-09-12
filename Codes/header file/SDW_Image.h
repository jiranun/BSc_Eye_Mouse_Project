// By Santi; www.shadowwares.com; 19 Feb, 2010
#ifndef _C_SDW_Image_H_
#include "SDW_DisplayImage_MFC_V1.h"
class C_SDW_Image
{
private: 
	typedef int (__cdecl *SDW_DisplayIplImage)(IplImage *img, HANDLE hPixtureBox, char *version); 
	typedef int (__cdecl *SDW_DisplayFromFile)(char     *fname, HANDLE hPixtureBox, char *version); 
	HANDLE hPictureBox;
	HINSTANCE hDLL;
	SDW_DisplayIplImage DisplayIplImageAddr;
	SDW_DisplayFromFile DisplayFromFileAddr;

public:
	C_SDW_Image(){};
	C_SDW_Image(HANDLE hPixtureBox){DisplayInitial(hPixtureBox);}
	virtual ~C_SDW_Image(){FreeLibrary(hDLL);}
	int DisplayInitial(HANDLE hPicturebox);
	int DisplayIplImage(IplImage *ImageName);
	int DisplayFromFile(char *ImageName);
};
#endif