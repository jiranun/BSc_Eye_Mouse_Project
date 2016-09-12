// EyeMouseDlg.cpp : implementation file
//


#include "stdafx.h"
#include "EyeMouse.h"
#include "EyeMouseDlg.h"
#include "afxwin.h"

#include "SDW_Image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "cv.h"
#include "highgui.h"
#include "stdafx.h"
#include "math.h"
#include "EyeMouseDlg.h"
#define sub 3
#define border 20
#define THRESHOLD 8

CvHaarClassifierCascade *cascade_le,*cascade_re;
CvMemStorage			*storage,*storage2;
CvRect *leftEye,*RightEye;
CvCapture* capture = cvCaptureFromCAM(0);
int ix,iy;
int cenX,cenY;
int poX,poY;

int right_click() 
{ 
        // right mouse button down 
        INPUT   Input={0};  
        Input.type        = INPUT_MOUSE; 
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN; 
        ::SendInput(1,&Input,sizeof(INPUT)); 
         
        // right mouse button up 
        ::ZeroMemory(&Input,sizeof(INPUT)); 
        Input.type        = INPUT_MOUSE; 
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP; 
        ::SendInput(1,&Input,sizeof(INPUT)); 
		return 0;
}

int left_click() 
{ 
        // left mouse button down 
        INPUT   Input={0};  
        Input.type        = INPUT_MOUSE; 
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN; 
        ::SendInput(1,&Input,sizeof(INPUT)); 
         
        // left mouse button up 
        ::ZeroMemory(&Input,sizeof(INPUT)); 
        Input.type        = INPUT_MOUSE; 
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP; 
        ::SendInput(1,&Input,sizeof(INPUT)); 
		return 0;
} 
void MousePos(INPUT *b,float Ang,float d)
{
	if(d<=8) d=4;
	poX += ((d-4)/2)*cos(Ang);
	poY += ((d-4)/2)*sin(Ang);
	poX = (poX<0 ? 0 : poX); 
	poY = (poY<0 ? 0 : poY);
	poX = (poX>ix ? ix : poX); 
	poY = (poY>iy ? iy : poY); 
	b->mi.dx = poX*65535/ix;
	b->mi.dy = poY*65535/iy;
	b->mi.mouseData = 0;
	b->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
	b->mi.time = 0;
	b->mi.dwExtraInfo = 0;
	SendInput(1,b,sizeof(INPUT));
}


int detectEyes(IplImage *img) { 
	    /* detect left eye */
	    CvSeq *le = cvHaarDetectObjects(
	        img,            
	        cascade_le,      
			storage,        
	        1.1, 3, 0,      
	        cvSize(35, 35)   
	    );

		/*detect right eye */
	   CvSeq *re = cvHaarDetectObjects(
	        img,            
	        cascade_re,      
			storage2,        
	        1.1, 3, 0,      
	        cvSize(35, 35) 
	    );
	   if(le->total == 0||re->total==0) return 0; 
	   else{	/* get the first detected eye */
		  leftEye = (CvRect*)cvGetSeqElem(le, 0);
		  RightEye = (CvRect*)cvGetSeqElem(re, 0);
		  //printf("%d  %d || ",leftEye->y,RightEye->y);
		  //Åº¤ÔéÇ
			leftEye->y+= leftEye->height*0.35;
			leftEye->height -= leftEye->height*0.35;
			RightEye->y+= RightEye->height*0.35;
			RightEye->height -= RightEye->height*0.35;
		  if(abs(leftEye->y - RightEye->y)<20 && abs(leftEye->x - RightEye->x)>30) return 1;
		  else return 0;
	   }
}	 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEyeMouseDlg dialog




CEyeMouseDlg::CEyeMouseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEyeMouseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEyeMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEyeMouseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CEyeMouseDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CEyeMouseDlg message handlers
CButton *button1;
unsigned int start = 1;
BOOL CEyeMouseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	button1 = (CButton *) GetDlgItem(IDC_BUTTON1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEyeMouseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_CLOSE){
		cvReleaseCapture( &capture );
		CDialog::EndDialog(0);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEyeMouseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEyeMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEyeMouseDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(start){ 
		button1->SetWindowTextW(_T("PAUSE"));
		start = 0; 
	}
	else {
		button1->SetWindowTextW(_T("START")); 
		start = 1;
	}
	//cvNamedWindow("LeftEye",1);
	//cvNamedWindow("RightEye",1);
	IplImage* frame;
	CStatic *pPic = (CStatic *)GetDlgItem(IDC_STATIC);
   	C_SDW_Image sdwImage;
	sdwImage.DisplayInitial(pPic->GetSafeHwnd());
	cascade_le = (CvHaarClassifierCascade*)cvLoad("haarcascade_righteye_2splits.xml", 0, 0, 0);
	cascade_re = (CvHaarClassifierCascade*)cvLoad("haarcascade_lefteye_2splits.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	storage2= cvCreateMemStorage(0);
	int detect = 1,first = 1;
	int click[4]={0,0,0,0}; // left right both

	ix=GetSystemMetrics(SM_CXSCREEN);
	iy=GetSystemMetrics(SM_CYSCREEN);
	poX = ix/2; poY = iy/2;
	INPUT *buffer = new INPUT[1]; //allocate a buffer
	buffer->type = INPUT_MOUSE;
	
	IplImage* match_re = cvCreateImage(cvSize(border*2+1,border*2+1),IPL_DEPTH_32F,1);
	IplImage* match_re2 = cvCreateImage(cvSize(border*2+1,border*2+1),IPL_DEPTH_32F,1);
	CvPoint minloc_r, minloc_l, maxloc_l, maxloc_r;
    double  minval_l, maxval_l, minval_r, maxval_r;
	IplImage *tl,*tr;
	IplImage *thres_l,*thres_r;

	MSG	stMsg;

	CvSeq* circle_L;

	while(start){
		frame = cvQueryFrame( capture );
		if(!frame ) break;
		cvFlip(frame,frame,1);
		sdwImage.DisplayIplImage(frame);
		while (::PeekMessage (&stMsg, NULL, 0, 0, PM_REMOVE)) 
			{
				::TranslateMessage (&stMsg);
				::DispatchMessage (&stMsg);
			}
	}

	while(!start){
		CvPoint Lcenter,Rcenter,center;
			if(detect){ 
				/////// detecting until found/////////
				while(1) {
					frame = cvQueryFrame( capture );
					if(!frame ) break;
					cvFlip(frame,frame,1);
					sdwImage.DisplayIplImage(frame);
					int isDetect = detectEyes(frame);
					if(isDetect){
						detect = 0;
						break;
					}
				}
				////////////////////////////////////
				
				///// create left eye template /////
				tl = cvCreateImage(cvSize(frame->width,frame->height),frame->depth,frame->nChannels);
				cvCopy(frame,tl,NULL);
				cvSetImageROI(tl,cvRect(leftEye->x,leftEye->y,leftEye->width,leftEye->height));
				/////////////////////////////////////

				///// create right eye template /////
				tr = cvCreateImage(cvSize(frame->width,frame->height),frame->depth,frame->nChannels);
				cvCopy(frame,tr,NULL);
				cvSetImageROI(tr,cvRect(RightEye->x,RightEye->y,RightEye->width,RightEye->height));
				/////////////////////////////////////
			}

		frame = cvQueryFrame( capture );
		if( !frame ) break;
		cvFlip(frame,frame,1);


		if(leftEye->x-border<=0||leftEye->y-border<=0||
			leftEye->y+leftEye->height+border>=frame->height||
			RightEye->x+RightEye->width+border>=frame->width||
			RightEye->y+RightEye->height>=frame->height||
			RightEye->y-border<=0) { 
				detect = 1; continue;
		}


		////////// Tracking ////////////
		cvSetImageROI(frame,cvRect(leftEye->x-border,leftEye->y-border,leftEye->width+border*2,leftEye->height+border*2));
		cvMatchTemplate( frame, tl, match_re, CV_TM_SQDIFF_NORMED );
		cvMinMaxLoc( match_re, &minval_l, &maxval_l, &minloc_l, &maxloc_l, 0 );
	
		cvResetImageROI(frame);

		cvSetImageROI(frame,cvRect(RightEye->x-border,RightEye->y-border,RightEye->width+border*2,RightEye->height+border*2));
		cvMatchTemplate( frame, tr, match_re, CV_TM_SQDIFF_NORMED );
		cvMinMaxLoc( match_re, &minval_r, &maxval_r, &minloc_r, &maxloc_r, 0 );
		/////////////////////////////////
		
		

		leftEye->x = leftEye->x-border+minloc_l.x; 
		leftEye->y = leftEye->y-border+minloc_l.y;

		RightEye->x = RightEye->x-border+ minloc_r.x; 
		RightEye->y = RightEye->y-border+ minloc_r.y; 
		
	
		
		Lcenter.x = leftEye->x+leftEye->width/2;
		Lcenter.y = leftEye->y+leftEye->height/2;
		Rcenter.x = RightEye->x+RightEye->width/2;
		Rcenter.y = RightEye->y+RightEye->height/2;

		center.x = (Lcenter.x+Rcenter.x)/2;
		center.y = (Lcenter.y+Rcenter.y)/2;



		if(first){
			cenX = center.x;
			cenY = center.y;
			first = 0;
		}
		
		if(minval_r>THRESHOLD||minval_l>THRESHOLD){
				detect = 1;
				
		}
		if(abs(leftEye->x - RightEye->x)<10||abs(leftEye->y - RightEye->y)>15) {
				detect = 1;
				
		}


		cvResetImageROI(frame);

		cvSetImageROI(frame,cvRect(leftEye->x,leftEye->y,leftEye->width,leftEye->height));
		thres_l = cvCreateImage(cvSize(leftEye->width,leftEye->height),frame->depth,1);
		cvCvtColor( frame, thres_l, CV_RGB2GRAY );
		cvResetImageROI(frame);

		cvSetImageROI(frame,cvRect(RightEye->x,RightEye->y,RightEye->width,RightEye->height));
		thres_r = cvCreateImage(cvSize(RightEye->width,RightEye->height),frame->depth,1);
		cvCvtColor( frame, thres_r, CV_RGB2GRAY );
		cvResetImageROI(frame);

		cvEqualizeHist(thres_l,thres_l);
		cvEqualizeHist(thres_r,thres_r);

		CvScalar t1 = cvAvg(thres_l,0);
		CvScalar t2 = cvAvg(thres_r,0);
		
		cvThreshold(thres_l, thres_l,t1.val[0]/10, 255, CV_THRESH_BINARY);
		cvThreshold(thres_r, thres_r,t2.val[0]/10, 255, CV_THRESH_BINARY);

		cvErode(thres_l,thres_l,0,1);
		cvErode(thres_r,thres_r,0,1);

		IplConvKernel *kernel = cvCreateStructuringElementEx(7, 7, 1, 1, CV_SHAPE_ELLIPSE, NULL);
		cvMorphologyEx(thres_l, thres_l, NULL, kernel, CV_MOP_CLOSE, 1);
		cvMorphologyEx(thres_r, thres_r, NULL, kernel, CV_MOP_CLOSE, 1);
		cvMorphologyEx(thres_l, thres_l, NULL, kernel, CV_MOP_CLOSE, 1);
		cvMorphologyEx(thres_r, thres_r, NULL, kernel, CV_MOP_CLOSE, 1);

		int a = t1.val[0]/25;
		int b = t2.val[0]/25;

		if((cvCountNonZero(thres_r)>=RightEye->width*RightEye->height-b)&&(cvCountNonZero(thres_l)>=leftEye->width*leftEye->height-a)) { click[2]++; click[0]=click[1]=0; } 
		else if(cvCountNonZero(thres_l)>=leftEye->width*leftEye->height-a) { click[0]++; click[1]=click[2]=0; }
		else if(cvCountNonZero(thres_r)>=RightEye->width*RightEye->height-b) { click[1]++; click[0]=click[2]=0; }
		else click[0]=click[1]=click[2]=0;
		


		if(click[0]>12) {
				left_click(); 
				click[0]=click[1]=click[2]=0;
		}
		else if(click[1]>12) {
				right_click();
				click[0]=click[1]=click[2]=0;
		}
		else if(click[2]>12) { 
				left_click(); left_click();
				click[0]=click[1]=click[2]=0;
		}


		cvCircle(frame,Lcenter,2,CV_RGB(255, 255, 255),1,8,0);
		cvCircle(frame,Rcenter,2,CV_RGB(255, 255, 255),1,8,0);
		cvCircle(frame,center,3,CV_RGB(0, 0, 255),1,8,0);
		cvLine(	frame,
				center,
				cvPoint(cenX,cenY),
				cvScalar(0,0,0,0),
				1,8,0);

		float dist = (float)sqrt(((float)cenX-(float)center.x)*((float)cenX-(float)center.x)+((float)cenY-(float)center.y)*((float)cenY-(float)center.y));
		float Angle = atan2((float)(center.y-cenY),(float)(center.x-cenX));
		
		MousePos(buffer,Angle,dist); 

		cvRectangle(
			frame,
			cvPoint(leftEye->x, leftEye->y),
			cvPoint(
				leftEye->x + leftEye->width, 
				leftEye->y + leftEye->height
			),
			CV_RGB(0, 255, 0), 
			1, 8, 0
		);

		cvRectangle(
			frame,
			cvPoint(RightEye->x, RightEye->y),
			cvPoint(
				RightEye->x + RightEye->width, 
				RightEye->y + RightEye->height
			),
			CV_RGB(255, 0, 0), 
			1, 8, 0
		);

		sdwImage.DisplayIplImage(frame);
		//cvShowImage ("LeftEye",thres_l);
		//cvShowImage ("RightEye",thres_r);
		while (::PeekMessage (&stMsg, NULL, 0, 0, PM_REMOVE)) 
			{
				::TranslateMessage (&stMsg);
				::DispatchMessage (&stMsg);
			}
	}

	delete (buffer);
	cvReleaseCapture( &capture );
	cvDestroyAllWindows();
}
