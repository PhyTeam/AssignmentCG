#if !defined (_MESH_CLASS)
#define _MESH_CLASS

#include "supportClass.h"

#define SAFE_DELETE(ptr) if(ptr != NULL) delete ptr
#define SAFE_DELETE_ARRAY(ptr) if(ptr != NULL) delete[] ptr;

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
	int		normalIndex;
};

class Face
{
public:
	int			nVerts;
	VertexID*	vert;
	Vector3		facenorm;
	unsigned char color[4];
	Face()
	{
		nVerts	= 0;
		vert	= NULL;
	}
	~Face()
	{
		if(vert !=NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	float vMax[3];
	float vMin[3];

	float origin[3];

	void cal_origin();
	
	int			numVerts;
	Point3*		pt;

	int			numFaces;
	Face*		face;
	int			numNorm;
	Vector3*	norm;

	float		slideX, slideY, slideZ;
	float		rotateX, rotateY, rotateZ;
	float		scaleX, scaleY, scaleZ;

public:
	Mesh()
	{
		numVerts	= 0;
		pt			= NULL;
		numFaces	= 0;
		face		= NULL;

		slideX = slideY = slideZ = 0;
		rotateX = rotateY = rotateZ = 0;
		scaleX = scaleY = scaleZ = 0;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}	
		if(face != NULL)
		{
			delete[] face;
		}

		SAFE_DELETE_ARRAY(norm);
		
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void DrawColor();
	void Draw();

	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void Draw2();
	void CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void CreatePlane(float size);
	void CreateSphere(int nSlice, int nStack, float radius);
	void CreateGeometryShape_Large(float R1, float R2, float Height,float distance);
	void CreateCylinder(int nSegment, float fHeight, float fRadius);
	void SetColor(int colorIdx);
	void CreateMixShape(int nSegment, float fHeight, float fDistance, float fRadius1, float fRadius2);

	void CalculateFacesNorm();
	void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
private:
	void Points_For_Sphere(Point3* pt, int nSlice, int nStack, int radius, int& run);
	void TangentLine(float C1[3], float C2[3], float xA, float yA, float* D1, float* D2);
	void TangentLine_Cut_Circle(float C1[3], float C2[3], float x, float y, float *D1, float *D2, float* p);
	void TangentLine_Get_Through(float C1[2], float C2[2], float& x, float& y);
	void SolveQuadFormula(float a, float b, float c, float& val1, float& val2);
	int GetColour(int& colour);
};

#endif