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

// PLYEditorDoc.cpp : implementation of the CPLYEditorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PLYEditor.h"
#endif

#include "PLYEditorDoc.h"
#include "PLYReader.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPLYEditorDoc

IMPLEMENT_DYNCREATE(CPLYEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CPLYEditorDoc, CDocument)
	//ON_COMMAND(ID_FILE_OPEN, &CPLYEditorDoc::OnFileOpen)
	ON_COMMAND(ID_BUTTON2, &CPLYEditorDoc::OnButton2)
END_MESSAGE_MAP()


// CPLYEditorDoc construction/destruction

CPLYEditorDoc::CPLYEditorDoc()
{
	// TODO: add one-time construction code here

}

CPLYEditorDoc::~CPLYEditorDoc()
{
}

BOOL CPLYEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CPLYEditorDoc serialization

void CPLYEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		CFile* cfile = ar.GetFile();
		
		CString filename = cfile->GetFilePath(); // return full path and filename
									  //::MessageBox(0, filename, L"Hello", MB_OK);
		char fn[200];
		strcpy(fn, CStringA(filename).GetString());
		Mesh* m = readFile(fn);
		theApp.AddToRecentFileList(filename.GetString());
		this->LMesh.push_back(m);
		this->UpdateAllViews(NULL);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPLYEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPLYEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPLYEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPLYEditorDoc diagnostics

#ifdef _DEBUG
void CPLYEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPLYEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
void CPLYEditorDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE);
	//dlg.m_ofn.nMaxFile = 1;
	dlg.m_ofn.lpstrFilter = L"Ply Files (*.ply)\0*.ply\0";
	dlg.m_ofn.lpstrTitle = L"Open PLY File As";

	CString filename;

	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		//::MessageBox(0, filename, L"Hello", MB_OK);
		char fn[200];
		strcpy(fn, CStringA(filename).GetString());
		Mesh* m = readFile(fn);
		theApp.AddToRecentFileList(filename.GetString());
		this->LMesh.push_back(m);
		this->UpdateAllViews(NULL);
	}
}
#endif //_DEBUG


// CPLYEditorDoc commands


void CPLYEditorDoc::OnButton2()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	//dlg.m_ofn.nMaxFile = 1;
	dlg.m_ofn.lpstrFilter = L"Ply Files (*.ply)\0*.ply\0";
	dlg.m_ofn.lpstrTitle = L"Open PLY File As";

	CString filename;

	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
									  //::MessageBox(0, filename, L"Hello", MB_OK);
		char fn[200];
		strcpy(fn, CStringA(filename).GetString());
		Mesh* m = readFile(fn);
		theApp.AddToRecentFileList(filename.GetString());
		this->LMesh.push_back(m);
		this->UpdateAllViews(NULL);
	}
}