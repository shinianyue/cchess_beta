// xiangqi1Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "xiangqi1.h"
#include "xiangqi1Dlg.h"
#include  "BaseDef.h"
#include  "Thinker.h"
#include "JiangJunDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <thread>


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXiangqi1Dlg dialog

CXiangqi1Dlg::CXiangqi1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXiangqi1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXiangqi1Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	CWinApp* pApp=AfxGetApp();
	
    //加载六个标题图标
	m_hIcons[0]=pApp->LoadIcon(IDI_MAINFRAME);             
    m_hIcons[1]=pApp->LoadIcon(MAKEINTRESOURCE(IDI_ICON1));
	m_hIcons[2]=pApp->LoadIcon(MAKEINTRESOURCE(IDI_ICON2));
	m_hIcons[3]=pApp->LoadIcon(MAKEINTRESOURCE(IDI_ICON3));
	m_hIcons[4]=pApp->LoadIcon(MAKEINTRESOURCE(IDI_ICON4));
	m_hIcons[5]=pApp->LoadIcon(MAKEINTRESOURCE(IDI_ICON5));
    
	//加载红方棋子图标
	m_hIconMan[RED_K]=pApp->LoadIcon(IDI_R_K);
	m_hIconMan[RED_S]=pApp->LoadIcon(IDI_R_S);
	m_hIconMan[RED_X]=pApp->LoadIcon(IDI_R_X);
	m_hIconMan[RED_M]=pApp->LoadIcon(IDI_R_M);
	m_hIconMan[RED_J]=pApp->LoadIcon(IDI_R_J);
	m_hIconMan[RED_P]=pApp->LoadIcon(IDI_R_P);
	m_hIconMan[RED_B]=pApp->LoadIcon(IDI_R_B);
    
	//加载黑方棋子图标
	m_hIconMan[BLACK_K]=pApp->LoadIcon(IDI_B_K);
	m_hIconMan[BLACK_S]=pApp->LoadIcon(IDI_B_S);
	m_hIconMan[BLACK_X]=pApp->LoadIcon(IDI_B_X);
	m_hIconMan[BLACK_M]=pApp->LoadIcon(IDI_B_M);
	m_hIconMan[BLACK_J]=pApp->LoadIcon(IDI_B_J);
	m_hIconMan[BLACK_P]=pApp->LoadIcon(IDI_B_P);
	m_hIconMan[BLACK_B]=pApp->LoadIcon(IDI_B_B);
	
	m_pdcBack=new CDC;     //储存棋盘图案的 DC
	m_pdcBoard=new CDC;    //缓冲 DC

	m_bThinking=FALSE;
    m_nStepCount=1;   //初始化走棋步数为1
	FixManMap();
}

void CXiangqi1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXiangqi1Dlg)
	DDX_Control(pDX, IDC_XIANGQI1_UNDO, m_Undo);
	DDX_Control(pDX, IDC_XIANGQI1_REDO, m_Redo);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressStep);
	DDX_Control(pDX, IDC_LIST1, m_listStep);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXiangqi1Dlg, CDialog)
	//{{AFX_MSG_MAP(CXiangqi1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_XIANGQI1_UNDO, OnXiangqi1Undo)
	ON_BN_CLICKED(IDC_XIANGQI1_REDO, OnXiangqi1Redo)
	ON_BN_CLICKED(IDC_XIANGQI1_HELP, OnXiangqi1Help)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_NEW, OnNew)
	ON_BN_CLICKED(IDC_XIANGQI1_PEACE, OnXiangqi1Peace)
	ON_BN_CLICKED(IDC_JiangJun, OnJiangJun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXiangqi1Dlg message handlers

BOOL CXiangqi1Dlg::OnInitDialog()
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
      SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcons[0]);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
		CClientDC dc(this);
	// TODO: Add extra initialization here
   //创建兼容位图
	m_Bitmap1.CreateCompatibleBitmap(&dc,XBW,YBW);
	m_Bitmap2.CreateCompatibleBitmap(&dc,XBW,YBW);

	//创建兼容位图
	m_pdcBack->CreateCompatibleDC(&dc);
	m_pdcBoard->CreateCompatibleDC(&dc);

	m_pdcBoard->SelectObject(&m_Bitmap1);
	m_pdcBack->SelectObject(&m_Bitmap2);
	
	//画棋盘
	MakeBoard();	//draw Chessboard in Board DC(3D)

	//设置进度条的值
	m_progressStep.CProgressCtrl::SetRange(0,100);
    
	//使相关按钮和菜单项无效
	CMenu* menu=GetMenu();
	m_Undo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_UNDO,TRUE);
    m_Redo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);

	//设置定时器
    m_nTimer=SetTimer(1,100,NULL);

    //新棋局
	New();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXiangqi1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXiangqi1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcons[0]);
	}
	else
	{   
		CClientDC dc(this);
		CRect rc;
		GetUpdateRect(&rc,TRUE);
		rc.OffsetRect(-XOFFSET,-YOFFSET);
		UpdateFace(rc);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXiangqi1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcons[0];
}

//DEL void CXiangqi1Dlg::OnButton1() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL }

void CXiangqi1Dlg::UpdateFace(CRect &rc)
{
	CRect rc1(0,0,BWA*9,BWA*10);
	//取得真正要更新的区域:
	rc1.IntersectRect(&rc1,&rc);

	m_pdcBack->BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),m_pdcBoard,rc.left,rc.top,SRCCOPY);
	
	int left=(rc1.left)/BWA,
		top=(rc1.top)/BWA,
		right=(rc1.right)/BWA,
		bottom=(rc1.bottom)/BWA;

	for(int i=left;i<=right;i++)
		for(int j=top;j<=bottom;j++)
			if(m_nManMap[i+1][j+1]!=32)
				m_pdcBack->DrawIcon(i*BWA+SW,j*BWA+SW,m_hIconMan[ManToIcon[m_nManMap[i+1][j+1]]]);

	if(m_nManPickUp!=32)	m_pdcBack->DrawIcon(m_pointPickUp,m_hIconMan[ManToIcon[m_nManPickUp]]);

	CClientDC dc(this);
	dc.BitBlt(rc.left+XOFFSET,rc.top+YOFFSET,rc.Width(),rc.Height(),m_pdcBack,rc.left,rc.top,SRCCOPY);
}


void CXiangqi1Dlg::FixManMap()
{ 
	//棋盘初始化
	int i;
  for(int i=0;i<11;i++)
		for(int j=0;j<12;j++)m_nManMap[i][j]=32;  
   
		//摆放棋子
	for(i=0;i<32;i++)
	{
		if(m_Face.man[i].x)
			m_nManMap[m_Face.man[i].x][m_Face.man[i].y]=i;
	}
}

void CXiangqi1Dlg::MakeBoard()
{
    //填充
	CBrush br(GetSysColor(COLOR_3DFACE));
	CRect rect(0,0,XBW,YBW);
	m_pdcBoard->FillRect(rect,&br);
	
	//画棋盘
	CPen pen[2];
	pen[0].CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
	pen[1].CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
	for(int i=0;i<=1;i++)
	{
		m_pdcBoard->SelectObject(pen[i]);
		
		//画边框(双线,双框)
		m_pdcBoard->MoveTo(XC[i]-3,YC[i]-3);         //XC[2]={BWA/2,BWA/2-1};	//XC,YC-棋子宽的一半
		m_pdcBoard->LineTo(XC[i]-3,YC[i]+3+BWA*9);   //YC[2]={BWA/2,BWA/2-2};	//[0].[1]分别为阴影和明线的偏移量
		m_pdcBoard->LineTo(XC[i]+3+BWA*8,YC[i]+3+BWA*9);
		m_pdcBoard->LineTo(XC[i]+3+BWA*8,YC[i]-3);
		m_pdcBoard->LineTo(XC[i]-3,YC[i]-3);

		m_pdcBoard->MoveTo(XC[i],YC[i]);
		m_pdcBoard->LineTo(XC[i],YC[i]+BWA*9);
		m_pdcBoard->LineTo(XC[i]+BWA*8,YC[i]+BWA*9);
		m_pdcBoard->LineTo(XC[i]+BWA*8,YC[i]);
		m_pdcBoard->LineTo(XC[i],YC[i]);
	
		//画竖线
		int n;
		for( n=1;n<=8;n++)
		{
			m_pdcBoard->MoveTo(XC[i],YC[i]+BWA*n);
			m_pdcBoard->LineTo(XC[i]+BWA*8,YC[i]+BWA*n);
		}
		for( n=1;n<=7;n++)
		{
			m_pdcBoard->MoveTo(XC[i]+BWA*n,YC[i]);
			m_pdcBoard->LineTo(XC[i]+BWA*n,YC[i]+BWA*4);
		
			m_pdcBoard->MoveTo(XC[i]+BWA*n,YC[i]+BWA*5);
			m_pdcBoard->LineTo(XC[i]+BWA*n,YC[i]+BWA*9);
		}
		//画宫
		m_pdcBoard->MoveTo(XC[i]+BWA*3,YC[i]+BWA*0);	
		m_pdcBoard->LineTo(XC[i]+BWA*5,YC[i]+BWA*2);	//上捺
		
		m_pdcBoard->MoveTo(XC[i]+BWA*5,YC[i]+BWA*0);	
		m_pdcBoard->LineTo(XC[i]+BWA*3,YC[i]+BWA*2);	//上撇
		
		m_pdcBoard->MoveTo(XC[i]+BWA*3,YC[i]+BWA*7);	
		m_pdcBoard->LineTo(XC[i]+BWA*5,YC[i]+BWA*9);	//下捺
		
		m_pdcBoard->MoveTo(XC[i]+BWA*5,YC[i]+BWA*7);	
		m_pdcBoard->LineTo(XC[i]+BWA*3,YC[i]+BWA*9);	//下撇
		//画星
		MB_DrawStar(7,2,i);
		MB_DrawStar(1,2,i);
		MB_DrawStar(0,3,i);
		MB_DrawStar(2,3,i);
		MB_DrawStar(4,3,i);
		MB_DrawStar(6,3,i);
		MB_DrawStar(8,3,i);

		MB_DrawStar(7,7,i);
		MB_DrawStar(1,7,i);
		MB_DrawStar(0,6,i);
		MB_DrawStar(2,6,i);
		MB_DrawStar(4,6,i);
		MB_DrawStar(6,6,i);
		MB_DrawStar(8,6,i);
	}
        CFont  font;
	  font.CreatePointFont(200,"华文隶书",NULL);
      CFont*poldfont= m_pdcBoard->SelectObject(&font);
      m_pdcBoard->SetBkMode(TRANSPARENT);
	  COLORREF  clr=m_pdcBoard->SetTextColor(RGB(255,100,100));
      m_pdcBoard->TextOut(BWA*2,BWA*4.6," 楚河         汉界") ;
	  m_pdcBoard->SetTextColor(clr);
	  m_pdcBoard->SelectObject(poldfont);

}

void CXiangqi1Dlg::MB_DrawStar(int x, int y, int i)
{   //画左半边星
	if(x!=0)
	{
		m_pdcBoard->MoveTo(XC[i]+BWA*x-3,YC[i]+BWA*y-3);
		m_pdcBoard->LineTo(XC[i]+BWA*x-6,YC[i]+BWA*y-3);

		m_pdcBoard->MoveTo(XC[i]+BWA*x-3,YC[i]+BWA*y-3);
		m_pdcBoard->LineTo(XC[i]+BWA*x-3,YC[i]+BWA*y-6);

		m_pdcBoard->MoveTo(XC[i]+BWA*x-3,YC[i]+BWA*y+3);
		m_pdcBoard->LineTo(XC[i]+BWA*x-6,YC[i]+BWA*y+3);

		m_pdcBoard->MoveTo(XC[i]+BWA*x-3,YC[i]+BWA*y+3);
		m_pdcBoard->LineTo(XC[i]+BWA*x-3,YC[i]+BWA*y+6);
	}

    //画右半边星
	if(x!=8)
	{
		m_pdcBoard->MoveTo(XC[i]+BWA*x+3,YC[i]+BWA*y-3);
		m_pdcBoard->LineTo(XC[i]+BWA*x+6,YC[i]+BWA*y-3);

		m_pdcBoard->MoveTo(XC[i]+BWA*x+3,YC[i]+BWA*y-3);
		m_pdcBoard->LineTo(XC[i]+BWA*x+3,YC[i]+BWA*y-6);

		m_pdcBoard->MoveTo(XC[i]+BWA*x+3,YC[i]+BWA*y+3);
		m_pdcBoard->LineTo(XC[i]+BWA*x+6,YC[i]+BWA*y+3);

		m_pdcBoard->MoveTo(XC[i]+BWA*x+3,YC[i]+BWA*y+3);
		m_pdcBoard->LineTo(XC[i]+BWA*x+3,YC[i]+BWA*y+6);
	}

}

void CXiangqi1Dlg::New()  //摆设新棋局
{   
	CFace face;
	New(face);
}

void CXiangqi1Dlg::New(CFace face)
{   
	//清空走法栈
	m_StepList.RemoveAll();
    
	//清空列表框
    while(m_listStep.GetCount()!=0)m_listStep.DeleteString(0);
    
	//使相关菜单项和按钮无效
	CMenu* menu=GetMenu();
	m_Undo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_UNDO,TRUE);
    m_Redo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);
	
	//使走棋从1开始
	m_nStepCount=1;  
	m_Face=face;
	FixManMap();       //摆设新棋局
	m_nManPickUp=32;   //拿起的棋子序号
	CRect rc(0,0,BWA*9,BWA*10);

	UpdateFace(rc);
}

BOOL CXiangqi1Dlg::FaceToPoint(CPoint &point)   //坐标转换成棋位坐标
{
if((point.x-XOFFSET)%BWA<SW||(point.x-XOFFSET)%BWA>BWA-SW)return FALSE;
	if((point.y-YOFFSET)%BWA<SW||(point.y-YOFFSET)%BWA>BWA-SW)return FALSE;
	CPoint p;
	p.x=(point.x-XOFFSET)/BWA+1;
	p.y=(point.y-YOFFSET)/BWA+1;
	if(p.x<1||p.x>9||p.y<1||p.y>10)return FALSE;
	
	point=p;
	return TRUE;
}

CString CXiangqi1Dlg::GetStepName(int man, int x, int y)
{
	int xfrom,yfrom;
	if (man==32)
	{
		 man=m_nManPickUp;
		 xfrom=m_pointBeforePickUp.x;
		 yfrom=m_pointBeforePickUp.y;
	}
	else
	{
		xfrom=m_Face.man[man].x;
		yfrom=m_Face.man[man].y;
	}
	
	const char strman[14][3]=
	{"帅","仕","相","马","车","炮","兵","将","士","象","马","车","炮","卒"};
	
	const char strnumc[10][3]=
	{"零","一","二","三","四","五","六","七","八","九"};

	const char strnuma[10][3]=
	{"０","１","２","３","４","５","６","７","８","９"};

	const char strjpt[3][3]={"进","退","平"};

	const char strqh[2][3]={"前","后"};


	int j,nformat,nresult,njpt,ndis,nA=0,nB=0;
	CString strresult;

	if(SideOfMan[man]==RED)
	{
		for(j=yfrom-1 ;j>0;j--)
			if(ManToType[man]==ManToType[m_nManMap[xfrom][j]] )nA++;
		for(j=yfrom+1 ;j<11;j++)
			if(ManToType[man]==ManToType[m_nManMap[xfrom][j]] )nB++;
	
		if		(nA+nB> 1)	{nformat=2;nresult=nA+1;}
		else if	(nA+nB==1)	{nformat=1;nresult=nA==0?0:1;}
		else				{nformat=0;nresult=10-xfrom;}

		if		(y> yfrom)	njpt=1;
		else if	(y==yfrom)	njpt=2;
		else				njpt=0;

		if(	ManToType[man]==RED_M||
			ManToType[man]==RED_S||
			ManToType[man]==RED_X)ndis=10-x;
		else
		{
		switch(njpt)
			{
			case 0:	ndis = yfrom-y;	break;
			case 1:	ndis = y-yfrom;	break;
			case 2:	ndis =10-x;	break;
			default:	break;
			}
		}
		switch(nformat)
		{
		case 0:	strresult.Format("%s%s%s%s",
								strman[ManToType[man]],
								strnumc[nresult],
								strjpt[njpt],
								strnumc[ndis]);
			break;
		case 2:	strresult.Format("%s%s%s%s",
								strnuma[nresult],
								strman[ManToType[man]],
								strjpt[njpt],
								strnumc[ndis]);
			break;
		case 1:	strresult.Format("%s%s%s%s",
								strqh[nresult],
								strman[ManToType[man]],
								strjpt[njpt],
								strnumc[ndis]);
			break;
		default:break;
		}
	}
	else	//Black side
	{
		for(j=yfrom+1 ;j<11;j++)
			if(ManToType[man]==ManToType[m_nManMap[xfrom][j]] )nA++;
		for(j=yfrom-1 ;j>0 ;j--)
			if(ManToType[man]==ManToType[m_nManMap[xfrom][j]] )nB++;
	
		if		(nA+nB> 1)	{nformat=2;nresult=nA+1;}
		else if	(nA+nB==1)	{nformat=1;nresult=nA==0?0:1;}
		else				{nformat=0;nresult=xfrom;}

		if		(y< yfrom)	njpt=1;
		else if	(y==yfrom)	njpt=2;
		else				njpt=0;

		if(	ManToType[man]==BLACK_M||
			ManToType[man]==BLACK_S||
			ManToType[man]==BLACK_X)ndis=x;
		else
		{
		switch(njpt)
			{
			case 0:	ndis = y-yfrom;	break;
			case 1:	ndis = yfrom-y;	break;
			case 2:	ndis = x;	break;
			default:	break;
			}
		}
		switch(nformat)
		{
		case 0:	strresult.Format("%s%s%s%s",
								strman[ManToType[man]],
								strnuma[nresult],
								strjpt[njpt],
								strnuma[ndis]);
			break;
		case 2:	strresult.Format("%s%s%s%s",
								strnuma[nresult],
								strman[ManToType[man]],
								strjpt[njpt],
								strnuma[ndis]);
			break;
		case 1:	strresult.Format("%s%s%s%s",
								strqh[nresult],
								strman[ManToType[man]],
								strjpt[njpt],
								strnuma[ndis]);
			break;
		default:break;
		}
	}

	CString str1;
	str1.Format("%2d ",m_nStepCount);

  return str1+strresult;
}

void CXiangqi1Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		CPoint p=point;
	if(m_Setting.m_nCOrM[m_Setting.m_nPlayer[m_Face.side]]==MAN&&!m_bThinking)	//是人走
		if(FaceToPoint(p))									//点的是棋位
			if(m_nManMap[p.x][p.y]!=32						//有棋子
				&& m_nManMap[p.x][p.y]/16==m_Face.side)		//是现在走的一方
			{
				SetCapture();
				m_pointBeforePickUp=p;
				PickUp(m_nManMap[p.x][p.y]);
			}
	CDialog::OnLButtonDown(nFlags, point);
}

void CXiangqi1Dlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_nManPickUp!=32)			//移动中
	{
		CPoint p=point;
		if(FaceToPoint(p))			//是棋位
		
		{
			if(!Go(32,p.x,p.y))		//不能走
				Move(32,m_pointBeforePickUp.x,m_pointBeforePickUp.y);
		}
		else						//不是棋位
		{
			Move(32,m_pointBeforePickUp.x,m_pointBeforePickUp.y);
		}
		ReleaseCapture();
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CXiangqi1Dlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		if(m_nManPickUp!=32)
	{
		CPoint last=m_pointPickUp;
		point.Offset(-XOFFSET-MW/2,-YOFFSET-MW/2);
		if(point.x<SW)point.x=SW;
		if(point.x>BWA*8+SW)point.x=BWA*8+SW;
		if(point.y<SW)point.y=SW;
		if(point.y>BWA*9+SW)point.y=BWA*9+SW;

		m_pointPickUp=point;
		CRect rc=MakeUpdateRect(last,m_pointPickUp);
		UpdateFace(rc);		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CXiangqi1Dlg::PickUp(int man)
{
    m_pointPickUp.x=(m_Face.man[man].x-1)*BWA+SW;
	m_pointPickUp.y=(m_Face.man[man].y-1)*BWA+SW;
	m_nManPickUp=man;
	m_Face.man[man].x=0;
	FixManMap();
}

void CXiangqi1Dlg::PutDown(int x, int y)
{
m_Face.man[m_nManPickUp].x=x;
	m_Face.man[m_nManPickUp].y=y;
	m_nManPickUp=32;	
	
	FixManMap();
	CRect rc;
	CPoint point((x-1)*BWA,(y-1)*BWA);
	rc=MakeUpdateRect(m_pointPickUp,point);
	UpdateFace(rc);
}

BOOL CXiangqi1Dlg::Go(CMove move)
{
return	Go(move.man,move.x,move.y);
}

BOOL CXiangqi1Dlg::Go(int man, int x, int y)
{
if(!CanGo( man,x, y))return FALSE;


	m_listStep.AddString(GetStepName(man,x,y));
	m_listStep.SetCurSel(m_listStep.GetCount()-1);
	CStep* pStep=new CStep;
	if(man==32)
	{
		pStep->man=m_nManPickUp;
		pStep->from.x=int(m_pointBeforePickUp.x);
		pStep->from.y=int(m_pointBeforePickUp.y);
	}
	else 
	{
		pStep->man=man;
		pStep->from.x=int(m_Face.man[man].x);
		pStep->from.y=int(m_Face.man[man].y);
	}
	pStep->to.x=x;
	pStep->to.y=y;

	
	if(m_nManMap[x][y]!=32)
	{
		pStep->eaten=m_nManMap[x][y];//吃掉的子
		m_Face.man[m_nManMap[x][y]].x=0;
	}

	m_StepList.Go(pStep);
	Move(man,x,y);
	m_Face.side=!m_Face.side;
    CMenu* menu=GetMenu();
	m_Undo.EnableWindow(TRUE);
	menu->EnableMenuItem(IDC_XIANGQI1_UNDO,FALSE);
	m_nStepCount++;
	m_bThinking=FALSE;

	if(m_Setting.m_nCOrM[m_Setting.m_nPlayer[m_Face.side]]==COM)	//轮到电脑
		    Think();
	return TRUE;
}

BOOL CXiangqi1Dlg::CanGo(const CMove &move)
{
return CanGo(move.man,move.x,move.y);
}

BOOL CXiangqi1Dlg::CanGo()
{

   if (m_Face.man[0].x==0 && m_nManPickUp!=0)
   {     
	        m_listStep.AddString("黑方胜!");

	        m_listStep.SetCurSel(m_listStep.GetCount()-1);
            CMenu* menu=GetMenu();
	        m_Redo.EnableWindow(FALSE);
	        menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);

			MessageBox( "黑方胜!","提醒",MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
if (m_Face.man[16].x==0 && m_nManPickUp!=16)
		{	
	            m_listStep.AddString("红方胜!");

	            m_listStep.SetCurSel(m_listStep.GetCount()-1);
                CMenu* menu=GetMenu();
	            m_Redo.EnableWindow(FALSE);
	            menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);

			MessageBox( "红方胜!","提醒",MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	return TRUE;
}

BOOL CXiangqi1Dlg::CanGo(int man, int x, int y)
{
    int xfrom,yfrom;
	if(!CanGo())return FALSE;
	if(man==32)
	{
		if(m_pointBeforePickUp.x==x&&m_pointBeforePickUp.y==y)//是原来的点
			return FALSE;
		man=m_nManPickUp;
		xfrom=m_pointBeforePickUp.x;
		yfrom=m_pointBeforePickUp.y;
	}
	else
	{
		if(m_Face.man[man].x==0)return FALSE;
		if(m_Face.man[man].x==x&&m_Face.man[man].y==y)return FALSE;
		xfrom=m_Face.man[man].x;
		yfrom=m_Face.man[man].y;
	}
	
	if(!::CanGo(m_nManMap,man,xfrom,yfrom,x,y))return FALSE;

	return TRUE;
}

void CXiangqi1Dlg::Move(int man, int x, int y)
{
	if(man!=32)
	{
	PickUp(man);
	}
	CPoint base(m_pointPickUp);
	
	int count=max(abs((x-1)*BWA+SW-m_pointPickUp.x),abs((y-1)*BWA+SW-m_pointPickUp.y));

	float xstep=float((x-1)*BWA+SW-m_pointPickUp.x)/count;
	float ystep=float((y-1)*BWA+SW-m_pointPickUp.y)/count;

	int index=ManToIcon[m_nManPickUp];
	UINT last=GetTickCount();
	count/=3;
	for(int i=0;i<count;i++)
	{
		while(GetTickCount()>last && GetTickCount()-last<25);//改变这个数来改变移动速度
		last=GetTickCount();
			
		CPoint point=m_pointPickUp;
		m_pointPickUp.x = long(base.x+xstep*i*3);
		m_pointPickUp.y = long(base.y+ystep*i*3);
		CRect rc;
		rc=MakeUpdateRect(m_pointPickUp,point);
		UpdateFace(rc);		

	}
	PutDown(x,y);
}

CRect CXiangqi1Dlg::MakeUpdateRect(CPoint p1, CPoint p2)
{
    CRect rc;
	rc.SetRect(p1,p2);
	rc.NormalizeRect();
	rc.bottom+=BWA;
	rc.right+=BWA;	
	return rc;
}

void CXiangqi1Dlg::Think()
{
if(CanGo())
	{
	m_bThinking = 1;
	}
}

void CXiangqi1Dlg::OnTimer(UINT nIDEvent) 
{
	/*
	static int  index=0;
	SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcons[index]);
	index=++index%5;  
	花里胡哨的 sb
	*/
	// TODO: Add your message handler code here and/or call default
    //实现外观图标的转换
    
	//电脑走棋
	if(m_bThinking)
	{
		

		//翻译“粪”语句
		std::string fen_commend = "position ";
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				//TRACE("%d\n",m_nManMap[j][i]);
				switch (m_nManMap[j][i])
				{
				case 0:
					fen_commend += "k";
					break;
				case 1:
				case 2:
					//士
					fen_commend += "a";
					break;
				case 3:
				case 4:
					//象
					fen_commend += "b";
					break;
				case 5:
				case 6:
					//马
					fen_commend += "n";
					break;
				case 7:
				case 8:
					//车
					fen_commend += "r";
					break;
				case 9:
				case 10:
					//炮
					fen_commend += "c";
					break;
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
					//冰
					fen_commend += "p";
					break;
				//--------------换边------------------------
				case 16:
					fen_commend += "K";
					break;
				case 17:
				case 18:
					//士
					fen_commend += "A";
					break;
				case 19:
				case 20:
					//象
					fen_commend += "B";
					break;
				case 21:
				case 22:
					//马
					fen_commend += "N";
					break;
				case 23:
				case 24:
					//车
					fen_commend += "R";
					break;
				case 25:
				case 26:
					//炮
					fen_commend += "C";
					break;
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
					//冰
					fen_commend += "P";
					break;
				case 32:
					if (i == 0 || j == 0 || j==10 || i==11) 
					{
						;
					}
					else
					{
						fen_commend += "1";
					}
					break;
				default:
					break;
					//fen_commend += "0";
				}
				//fen_commend+= m_nManMap[i][j];
			}
			if (i == 0 || i == 10)
			{
			}
			else fen_commend += "/";
		}
		TRACE(fen_commend.c_str());
		//system(".\\CCStockFish\\CCStockFish-avx2.exe");
		//----------------创造进程和操控------------
		SECURITY_ATTRIBUTES saAttr;
		HANDLE out_w, out_r, in_w, in_r;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&saAttr, sizeof(SECURITY_ATTRIBUTES));
		ZeroMemory(&si,		sizeof(STARTUPINFO));

		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		CreatePipe(&out_r, &out_w, &saAttr, 0);
		CreatePipe(&in_r, &in_w, &saAttr, 0);

		si.cb = sizeof(STARTUPINFO);
		si.hStdError = out_w;
		si.hStdInput = in_w;
		si.hStdOutput = out_w;
		si.dwFlags |= STARTF_USESTDHANDLES;

		std::string path = ".\\CCStockFish\\CCStockFish-avx2.exe";
		std::string working_path = ".\\CCStockFish";
		CreateProcess(path.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, working_path.c_str(), &si, &pi);
		//CloseHandle(out_w);
		//CloseHandle(in_r);
		DWORD bytesRead;
		DWORD bytesWrite;
		//WriteFile(in_w, fen_commend.c_str(), fen_commend.size(), &bytesWrite, NULL);

		char buffer[4096];
		/*
		while (ReadFile(out_r, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
			TRACE(std::string(buffer, bytesRead).c_str());
		}
		*/
		//ReadFile(out_r, buffer, sizeof(buffer), &bytesRead, NULL);
		WriteFile(in_w, std::string("isread\n").c_str(), std::string("isread\n").size(), &bytesWrite, NULL);
		//WaitForSingleObject(pi.hProcess, INFINITE);
		ReadFile(out_r, buffer, sizeof(buffer), &bytesRead, NULL);
		TRACE(std::string(buffer, bytesRead).c_str());
		ReadFile(out_r, buffer, sizeof(buffer), &bytesRead, NULL);
		/*
		
		WriteFile(in_w, std::string("bench\n").c_str(), std::string("bench\n").size(), &bytesWrite, NULL);
		ReadFile(out_r, buffer, sizeof(buffer), &bytesRead, NULL);
		TRACE(std::string(buffer, bytesRead).c_str());
		*/

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		//----------------------------------------
		std::string move_result;
		Go(0, 1, 1);//走棋
		m_Face.side = !m_Face.side;//交换棋手
		//Move(1, 1, 1);
		m_bThinking = 0;
	}

}

void CXiangqi1Dlg::OnXiangqi1Undo() 
{
	// TODO: Add your control notification handler code here
   if(m_nStepCount!=1)
	m_nStepCount--;
	CStep* pStep=m_StepList.Undo();
	if(pStep==NULL)
	{
		return;
	}
   
	m_progressStep.SetPos(0);
	PickUp(pStep->man);

	if(pStep->eaten!=32)
	{
		m_Face.man[pStep->eaten]=pStep->to;
		FixManMap();
	}
     CMenu* menu=GetMenu();
	m_Redo.EnableWindow(TRUE);
	menu->EnableMenuItem(IDC_XIANGQI1_REDO,FALSE);
	if(m_StepList.IsHead())	
	{m_Undo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_UNDO,TRUE);}
	Move(32,pStep->from.x,pStep->from.y);
	m_Face.side=!m_Face.side;

	m_listStep.DeleteString(m_listStep.GetCount()-1);
	m_listStep.SetCurSel(m_listStep.GetCount()-1);
	m_bThinking=FALSE;
}

void CXiangqi1Dlg::OnXiangqi1Redo() 
{
	// TODO: Add your control notification handler code here
	CStep* pStep=m_StepList.Redo();
	if(pStep==NULL)
	{
		return;
	}
      CMenu* menu=GetMenu();
	m_Undo.EnableWindow(TRUE);
	menu->EnableMenuItem(IDC_XIANGQI1_UNDO,FALSE);
    if(m_StepList.IsEnd())
	{m_Redo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);}
	m_listStep.AddString(GetStepName(pStep->man,pStep->to.x,pStep->to.y));
	m_listStep.SetCurSel(m_listStep.GetCount()-1);

	if(pStep->eaten!=32)m_Face.man[pStep->eaten].x=0;

	m_Face.side=!m_Face.side;

	Move(pStep->man,pStep->to.x,pStep->to.y);
	m_nStepCount++;
}

void CXiangqi1Dlg::OnXiangqi1Help() 
{
	// TODO: Add your control notification handler code here
	if(!m_bThinking)Think();

     //使还原键无效
     CMenu* menu=GetMenu();
	 m_Redo.EnableWindow(FALSE);
	menu->EnableMenuItem(IDC_XIANGQI1_REDO,TRUE);
}



void CXiangqi1Dlg::OnAbout() //关于对话框菜单项响应
{
	// TODO: Add your command handler code here
		CAboutDlg dlgAbout;
 	    dlgAbout.DoModal();
}

void CXiangqi1Dlg::OnNew() //新棋局菜单项响应
{
	// TODO: Add your command handler code here
	New();
}




void CXiangqi1Dlg::OnXiangqi1Peace() //求和按钮
{
	// TODO: Add your control notification handler code here
if(MessageBox("是否真的要求和？","求和",MB_YESNO)==IDYES)
		New();
}

void CXiangqi1Dlg::OnJiangJun() 
{
	// TODO: Add your control notification handler code here
	CJiangJunDlg dlg;
	dlg.DoModal();
}
