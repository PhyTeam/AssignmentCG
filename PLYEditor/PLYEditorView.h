// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PLYEditorView.h : interface of the CPLYEditorView class
//

#pragma once


class CPLYEditorView : public CView
{
protected: // create from serialization only
	CPLYEditorView();
	DECLARE_DYNCREATE(CPLYEditorView)

// Attributes
	HGLRC m_hRC;
	bool bhit = false;
public:
	CPLYEditorDoc* GetDocument() const;

	void mouse(int button, int state, int x, int y);

	void mouseMotion(int x, int y);

	void mouseSelectionMode(int x, int y);

	void processHits(GLint hits, GLuint buffer[]);

// Operations
public:
	float aspect;
	/* Camera variables */
	float camera_angle;
	float camera_height;
	float camera_dis;
	float camera_X, camera_Y, camera_Z;
	float lookAt_X, lookAt_Y, lookAt_Z;

	float matrix[16];
	float axis[3];
	float angle = 0.0;
	// Previous mouse cursor position
	int previousX = 0;
	int previousY = 0;
	// Says if the mouse left button is currently pressed
	bool isLeftButtonPressed = false;

	float zoomScale = 0.95f;

	int		screenWidth = 600;
	int		screenHeight = 600;
	bool	bWireFrame = false;
	float cameraX = 0, cameraY = 0, cameraZ = 1000;
	bool isRightButtonPressed = false;
	float	cur_trans_x = 0, cur_trans_y = 0, cur_trans_z = 0;
	float	xselect = 0, yselect = 0;
	bool selectedMode = false;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnLButtonDown(UINT flags, CPoint loc);
	virtual void OnLButtonUp(UINT flags, CPoint loc);
	void OnRButtonDown(UINT flags, CPoint loc);
	virtual void OnMouseMove(UINT flags, CPoint loc);
	virtual void OnSize(UINT uType, int cx, int cy);
	void drawAxis();
	virtual void OnPaint();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
// Implementation
public:
	virtual ~CPLYEditorView();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void SetupOpenGL(void);
	void init();
	void initLight();
// Generated message map functions
protected:
	afx_msg
		void initLight2();
	void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PLYEditorView.cpp
inline CPLYEditorDoc* CPLYEditorView::GetDocument() const
   { return reinterpret_cast<CPLYEditorDoc*>(m_pDocument); }
#endif

