// xiangqi1Dlg.h : header file
//
#include "BaseClasses.h"
#include "StepList.h"
#include "Thinker.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#if !defined(AFX_XIANGQI1DLG_H__B36CF4DD_30A2_4DE4_BF8D_51AE48D17B1C__INCLUDED_)
#define AFX_XIANGQI1DLG_H__B36CF4DD_30A2_4DE4_BF8D_51AE48D17B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXiangqi1Dlg dialog

class CXiangqi1Dlg : public CDialog
{
// Construction
public:
	CXiangqi1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXiangqi1Dlg)
	enum { IDD = IDD_XIANGQI1_DIALOG };
	CButton	m_Undo;
	CButton	m_Redo;
	CProgressCtrl	m_progressStep;
	CListBox	m_listStep;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXiangqi1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Think();
	bool thinker_existance = 0;
	CRect MakeUpdateRect(CPoint p1, CPoint p2);	
	void Move(int man, int x, int y);           
	BOOL CanGo(int man, int x, int y);          
	BOOL CanGo();                               
	BOOL CanGo( const CMove &move);             
	BOOL Go(int man, int x, int y);             
	BOOL Go(CMove move);                        
	void PutDown(int x,int y);                  
	void PickUp(int man);                       
	CString GetStepName(int man, int x, int y);	
	BOOL FaceToPoint(CPoint &point);            
	void New(CFace face);                       
	void New();                                 
	void MB_DrawStar(int x,int y,int i);       
	void MakeBoard();                           
	void FixManMap();                           
	void UpdateFace(CRect &rc);                 
	CSetting m_Setting;	
    int m_nTimer;				
	CStepList m_StepList;		
    BOOL m_bThinking;			
	CDC* m_pdcBoard;			
	CDC* m_pdcBack;				
    int m_nManPickUp;			
	int m_nManMap[11][12];		
	int m_nStepCount;			
	HICON m_hIconMan[14];		
	CBitmap m_Bitmap1;			
	CBitmap m_Bitmap2;		    
	CFace m_Face;				
	CPoint m_pointPickUp;		
	CPoint m_pointBeforePickUp;	
	// Generated message map functions
	//{{AFX_MSG(CXiangqi1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnXiangqi1Undo();
	afx_msg void OnXiangqi1Redo();
	afx_msg void OnXiangqi1Help();
	afx_msg void OnAbout();
	afx_msg void OnNew();
	afx_msg void OnXiangqi1Peace();
	afx_msg void OnJiangJun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcons[6];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XIANGQI1DLG_H__B36CF4DD_30A2_4DE4_BF8D_51AE48D17B1C__INCLUDED_)
