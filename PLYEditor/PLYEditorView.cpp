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

// PLYEditorView.cpp : implementation of the CPLYEditorView class
//

#include "stdafx.h"
#include <ctime>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PLYEditor.h"
#endif

#include "PLYEditorDoc.h"
#include "PLYEditorView.h"

#include <GL\glut.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPLYEditorView

IMPLEMENT_DYNCREATE(CPLYEditorView, CView)

BEGIN_MESSAGE_MAP(CPLYEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPLYEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_CHECK2, &CPLYEditorView::OnCheck2)
	ON_UPDATE_COMMAND_UI(ID_CHECK2, &CPLYEditorView::OnUpdateCheck2)
END_MESSAGE_MAP()

// CPLYEditorView construction/destruction

CPLYEditorView::CPLYEditorView()
{
	// TODO: add construction code here
	this->camera_dis = 10;
	this->camera_angle = 0;
	this->camera_height = 0;
}

CPLYEditorView::~CPLYEditorView()
{
}

int CPLYEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		;// return -1;

		 // TODO:  Add your specialized creation code here
		 //Setup Opengl
	SetupOpenGL();
	//Create Timer
	//m_hTimer = SetTimer(1, 100, NULL);


	return 0;
}

BOOL CPLYEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

bool CPLYEditorView::isCreateOpenGL = false;

// CPLYEditorView drawing

void CPLYEditorView::OnDraw(CDC* /*pDC*/)
{
	CPLYEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPLYEditorView printing


void CPLYEditorView::SetupOpenGL(void)
{
	//Declare Pixel Format
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	HDC hdc = GetDC()->m_hDC;

	// Set Pixel format.
	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	m_hRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, m_hRC);
	this->hdc = hdc;


	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j) matrix[i * 4 + j] = 1.0; else matrix[i * 4 + j] = 0.0;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	initLight();
	//initLight2();
	init();
}

void CPLYEditorView::init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 1.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void CPLYEditorView::initLight() {
	GLfloat	lightDiffuse[] = { 0.5f, 0.5f, 0.5f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient[] = { 0.2f, 0.2f, 0.2f, .7f };
	GLfloat 	light_position1[] = { 60.0f, 60.0f, 60.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void CPLYEditorView::initLight2() {
	GLfloat	lightDiffuse[] = { 0.5f, 0.5f, 0.5f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient[] = { 1.0f, 0.2f, 0.1f, 1.0f };
	GLfloat 	light_position1[] = { -6.0f, -6.0f, 6.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
}

void CPLYEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPLYEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPLYEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPLYEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPLYEditorView::OnLButtonDown(UINT flags, CPoint loc)
{
	int x = loc.x;
	int y = loc.y;
	isLeftButtonPressed = true;
	previousX = x;
	previousY = y;
	angle = 0.0;
}

void CPLYEditorView::OnLButtonUp(UINT flags, CPoint loc)
{
	isLeftButtonPressed = false;
}

void CPLYEditorView::OnRButtonDown(UINT flags, CPoint loc) {
	Mesh *object3d = NULL;
	if (GetDocument()->LMesh.size() > 0)
		object3d = GetDocument()->LMesh[0];
	if (object3d == NULL)
		return;
	int x = loc.x;
	int y = loc.y;
	isRightButtonPressed = true;
	previousX = x;
	previousY = y;
	cur_trans_x = object3d->slideX;
	cur_trans_y = object3d->slideY;
	cur_trans_z = object3d->slideZ;
	angle = 0.0;

	// selected mode
	xselect = x; yselect = y;
	GLuint selectBuf[512];
	GLint hits;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(512, selectBuf);
	(void)glRenderMode(GL_SELECT);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	/*  create 5x5 pixel picking region near cursor location */
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y),
		5.0, 5.0, viewport);
	//glMatrixMode(GL_MODELVIEW);

	gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 1.0f, 10000.0f);
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(matrix);

	glMatrixMode(GL_MODELVIEW);
	//glScaled(0.05, 0.05, 0.05);
	//glTranslatef(object3d->slideX, object3d->slideY, object3d->slideZ);
	glTranslatef(object3d->origin[0], object3d->origin[1], object3d->origin[2]);

	glLoadName(1);

	object3d->Draw2();

	glPopMatrix();
	//glFlush();

	//auto document = this->GetDocument();
	//int count = 1;
	//for (auto const& i : document->LMesh) {

	//	glLoadName(count++);
	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glTranslated(i->position.x, i->position.y, i->position.z);

	//	glTranslatef(i->origin[0], i->origin[1], i->origin[2]);

	//	i->Draw2();

	//	glPopMatrix();
	//}

	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);

	Invalidate();
}

void CPLYEditorView::OnMouseMove(UINT flags, CPoint loc)
{
	mouseMotion(loc.x, loc.y);
}

void CPLYEditorView::OnSize(UINT uType, int cx, int cy)
{
	CView::OnSize(uType, cx, cy);

	//Setting up viewport
	if (cy == 0)
		aspect = cx;
	this->screenWidth = cx;
	this->screenHeight = cy;
	this->aspect = (GLfloat)cx / cy;
	glViewport(0, 0, cx, cy);
}

void CPLYEditorView::drawAxis()
{
	glPushMatrix();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-400, 0, 0);//x
	glVertex3f(400, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);//y
	glVertex3f(0, 400, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -400);//z
	glVertex3f(0, 0, 400);
	glEnd();

	glPopMatrix();
}

#define DEG2RAD (3.14159f/180.0f)

void CPLYEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	wglMakeCurrent(hdc, m_hRC);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 1.0f, 10000.0f);
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, screenWidth, screenHeight);
	glRotatef(30.0, 1, 0.5, 0);
	drawAxis();
	glMultMatrixf(matrix);

	glMatrixMode(GL_MODELVIEW);



	auto document = this->GetDocument();
	int count = 1;
	for (auto const& i : document->LMesh) {

		glLoadName(count++);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glTranslated(i->position.x, i->position.y, i->position.z);

		glTranslatef(i->origin[0], i->origin[1], i->origin[2]);

		if (bWireFrame)
			i->DrawWireframe();
		else
		{
			i->Draw2();
		}

		glPopMatrix();
	}

	//glutSolidTeapot(200.0);
	glFlush();
	SwapBuffers(dc.m_ps.hdc);
	Invalidate();
}

void CPLYEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) {
		selectedMode = !selectedMode;
	}

	char key = nChar - 128;
	switch (key) {
	case '-':
	case '_':
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(1.f / zoomScale, 1.f / zoomScale, 1.f / zoomScale);
		glMultMatrixf(matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
		glPopMatrix();
		break;
	case '+':
	case '=':
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(zoomScale, zoomScale, zoomScale);
		glMultMatrixf(matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
		glPopMatrix();
		break;
	}
}

BOOL CPLYEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	zoomScale = 1 + (zDelta / 120.0) * 0.1f;
	glScalef(zoomScale, zoomScale, zoomScale);
	glMultMatrixf(matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
	glPopMatrix();
	return 0;
}

void CPLYEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
	isRightButtonPressed = false;
	bhit = false;
}

void CPLYEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CPLYEditorView::mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			isLeftButtonPressed = true;
			previousX = x;
			previousY = y;
			angle = 0.0;
		}
		else if (state == GLUT_UP) {
			isLeftButtonPressed = false;
		}
		break;
	case 3:
	case 4:
		if (state == GLUT_DOWN) {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(zoomScale, zoomScale, zoomScale);
			glMultMatrixf(matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
			glPopMatrix();
		}
		break;
	}

	glutPostRedisplay();
}

void CPLYEditorView::mouseMotion(int x, int y)
{
	if (isRightButtonPressed)
		return mouseSelectionMode(x, y);
	if (isLeftButtonPressed) {
		// Project previous coords on to the sphere.
		float x1 = (2 * previousX - screenWidth) / (float)screenWidth;
		float y1 = (screenHeight - 2 * previousY) / (float)screenHeight;
		float z1 = 1 - x1*x1 - y1*y1;

		// Be careful not to take the square root of a negative number.
		if (z1 < 0) z1 = 0; else z1 = sqrt(z1);

		// Project current coords on to the sphere.
		float x2 = (2 * x - screenWidth) / (float)screenWidth;
		float y2 = (screenHeight - 2 * y) / (float)screenHeight;
		float z2 = 1 - x2*x2 - y2*y2;

		if (z2 < 0) z2 = 0; else z2 = sqrt(z2);

		// Take the cross product to find the axis of rotation.
		axis[0] = y1*z2 - y2*z1;
		axis[1] = z1*x2 - z2*x1;
		axis[2] = x1*y2 - x2*y1;

		// Angle is computed from the magnitude of the cross product.
		float norm = sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
		angle = asin(fmin(1.0f, norm)) * 180 / 3.14159;

		// Normalize the axis.
		axis[0] /= norm;
		axis[1] /= norm;
		axis[2] /= norm;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRotatef(angle * 1.5f, axis[0], axis[1], axis[2]);
		glMultMatrixf(matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
		glPopMatrix();

		previousX = x;
		previousY = y;

		Invalidate();
	}
}

void CPLYEditorView::mouseSelectionMode(int x, int y) {
	float PI = 3.1415926535898f;
	if (bhit && isRightButtonPressed) {// selected mode -> translate
		float xdiff = previousX - x;
		float ydiff = previousY - y;
		previousX = x;
		previousY = y;
		//cameraX = camera_dis*cos(camera_angle);
		//cameraY = camera_height;
		//cameraZ = camera_dis*sin(camera_angle);
		GLint upx = 0, upy = 0, upz = 0, leftx = 0, lefty = 0, leftz = 0;
		Vector3 OE(cameraX, cameraY, cameraZ);
		Vector3 up(0.0, 1.0, 0.0);
		Vector3 left = OE.cross(up);
		left.normalize();
		Vector3 wup = left.cross(OE);
		wup.normalize();
		float fovy1 = 45.0 / 180.0* PI;	// near
										// reset translate vector
										//object3d->slideX = cur_trans_x + 2 * xdiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE)) *left.x + 2 * ydiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE))*wup.x;
										///object3d->slideY = cur_trans_y + 2 * xdiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE)) *left.x + 2 * ydiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE))*wup.y;
										//object3d->slideZ = cur_trans_z + 2 * xdiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE)) *left.x + 2 * ydiff*tan(fovy1 / 2) / screenHeight*sqrt(OE.dot(OE))*wup.z;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRotatef(angle * 1.5f, axis[0], axis[1], axis[2]);

		/*float ox = selected_mesh->position.x;
		float oy = selected_mesh->position.y;
		float oz = selected_mesh->position.z;
		selected_mesh->position.set(
		ox + xdiff*left.x*1.3 + ydiff*wup.x*1.3,
		oy + xdiff*left.y*1.3 + ydiff*wup.y*1.3,
		oz + xdiff*left.z*1.3 + ydiff*wup.z*1.3
		);*/

		glTranslatef(xdiff*left.x*1.3 + ydiff*wup.x*1.3, xdiff*left.y*1.3 + ydiff*wup.y*1.3, xdiff*left.z*1.3 + ydiff*wup.z*1.3);
		glMultMatrixf(matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)matrix);
		glPopMatrix();

		init();
		Invalidate();

	}

}

void CPLYEditorView::processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr;
	GLfloat maxz1 = 10, maxz2 = -1, z1, z2;
	GLint name = -1;
	//printf("hits = %d\n", hits);
	ptr = (GLuint *)buffer;
	/*for (i = 0; i < hits; i++) {  // for each hit
	names = *ptr;
	printf(" number of names for hit = %d\n", names); ptr++;
	printf("  z1 is %g;", (float)*ptr / 0x7fffffff); z1 = (float)*ptr / 0x7fffffff; ptr++;
	printf(" z2 is %g\n", (float)*ptr / 0x7fffffff); z2 = (float)*ptr / 0x7fffffff; ptr++;
	if (z1 < maxz1) {
	maxz1 = z1;
	name = *ptr;
	}
	printf("   the name is ");
	for (j = 0; j < names; j++) {  // for each name
	printf("%d ", *ptr); ptr++;
	}
	printf("\n");
	}*/
	GLuint* bufp = buffer;
	GLuint numnames;
	GLuint hitnames[10];
	int hi = 0;
	for (unsigned int j = 0; j < hits; j++) {

		numnames = *bufp++;
		z1 = *bufp++;
		z2 = *bufp++;
		while (numnames--) {
			name = *bufp++;
			hitnames[hi++] = name;
			current_item = name;
		}
	}

	if (hits > 0) {
		bhit = true;
		//printf("Selected");

	}

	/*auto doc = GetDocument();
	auto ls_mesh = doc->LMesh;
	selected_mesh = ls_mesh[current_item];
	*/
}

BOOL CPLYEditorView::OnEraseBkgnd(CDC* pDC)
{
	return 1;//Prevent flicker
			 //return CView::OnEraseBkgnd(pDC);
}
// CPLYEditorView diagnostics

#ifdef _DEBUG
void CPLYEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CPLYEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPLYEditorDoc* CPLYEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPLYEditorDoc)));
	return (CPLYEditorDoc*)m_pDocument;
}



#endif //_DEBUG


// CPLYEditorView message handlers


void CPLYEditorView::OnCheck2()
{
	// TODO: Add your command handler code here
	bWireFrame = !bWireFrame;
}



void CPLYEditorView::OnUpdateCheck2(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

}