//Cao Phúc
//1512500

//Assignment 1 Đồ họa máy tính

#include <windows.h>
#include <math.h>
#include <gl.h>
#include <glut.h>
#include <iostream>



//Định nghĩa
//----------------------------------------------------------------------------------------------
#define PI			3.1415926
#define	COLORNUM		14
#define DEG2RAD	3.141592/180.0f




float	ColorArr[COLORNUM][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0,  0.0, 1.0 },
{ 1.0, 1.0,  0.0 },{ 1.0, 0.0, 1.0 },{ 0.0, 1.0, 1.0 },
{ 0.3, 0.3, 0.3 },{ 0.5, 0.5, 0.5 },{ 0.9,  0.9, 0.9 },
{ 1.0, 0.5,  0.5 },{ 0.5, 1.0, 0.5 },{ 0.5, 0.5, 1.0 },
{ 0.0, 0.0, 0.0 },{ 0.9, 0.8, 1.0 } };




//----------------------------------------------------------------------------------------------
//Định nghĩa Điễm trong không gian ba chiều và các hàm kèm theo
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	void set(Point3& p)
						{ x = p.x; y = p.y; z = p.z;}
	Point3() { x = y = z = 0;}
	Point3(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}

};

//----------------------------------------------------------------------------------------------
//Lớp to màu
class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
						{ r = red; g = green; b = blue;}
	void set(Color3& c)
						{ r = c.r; g = c.g; b = c.b;}
	Color3() { r = g = b = 0;}
	Color3(float red, float green, float blue)
						{ r = red; g = green; b = blue;}

};
class Point2
{
	public:
		Point2() { x = y = 0.0f; } // constructor 1
		Point2(float xx, float yy) { x = xx; y = yy; } // constructor 2
		void set(float xx, float yy) { x = xx; y = yy; }
		float getX() { return x;}
		float getY() { return y;}
		void draw()		{	glBegin(GL_POINTS);
								glVertex2f((GLfloat)x, (GLfloat)y);
							glEnd();
						}
	private:
		float 	x, y;
};
class IntRect
{
	 public:
		IntRect() { l = 0; r = 100; b = 0; t = 100; } // constructor
		IntRect( int left, int right, int bottom, int top)
			{ l = left; r = right; b = bottom; t = top; }
		void set( int left, int right, int bottom, int top)
			{ l = left; r = right; b = bottom; t = top; }
		void draw(){
						glRecti(l, b, r, t);
						glFlush();
					} // draw this rectangle using OpenGL
		int  getWidth(){return (r-l);}
		int  getHeight() { return (t-b);}
	 private:
		int	l, r, b, t;
};


class RealRect
{
	 public:
		RealRect() { l = 0; r = 100; b = 0; t = 100; } // constructor
		RealRect( float left, float right, float bottom, float top)
			{ l = left; r = right; b = bottom; t = top; }
		void set( float left, float right, float bottom, float top)
			{ l = left; r = right; b = bottom; t = top; }
		float  getWidth(){return (r-l);}
		float  getHeight() { return (t-b);}
		void RealRect::draw(){
							glRectf(l, b, r, t);
							glFlush();
						};// draw this rectangle using OpenGL
	 private:
		float	l, r, b, t;
};

//----------------------------------------------------------------------------------------------
//Lớp Vector3 định nghĩa một vector trong không gian 3 chiều
class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	void set(Vector3& v)
						{ x = v.x; y = v.y; z = v.z;}
	void flip()
						{ x = -x; y = -y; z = -z;}
	void normalize();
	Vector3() { x = y = z = 0;}
	Vector3(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	Vector3(Vector3& v)
						{ x = v.x; y = v.y; z = v.z;}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};


Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x*b.x + y*b.y + z*b.z;
}
void Vector3::normalize()
{
	float temp = sqrt(x*x + y*y + z*z);
	x = x/temp;
	y = y/temp;
	z = z/temp;
}



//----------------------------------------------------------------------------------------------
//Định nghĩa class VertexID


class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
};

//----------------------------------------------------------------------------------------------
//Định nghĩa class Face
class Face
{
public:
	int		nVerts;
	VertexID*	vert;
	
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

//----------------------------------------------------------------------------------------------
//Định nghĩa class Mesh
class Mesh
{
public:
	int		numVerts;
	Point3*		pt;
	
	int		numFaces;
	Face*		face;
	float		slideX, slideY, slideZ;
	float		rotateX, rotateY, rotateZ;
	float		scaleX, scaleY, scaleZ;

public:
	Mesh()
	{
		numVerts	= 0;
		pt		= NULL;
		numFaces	= 0;
		face		= NULL;
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
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void DrawColor();

	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void CreateCubeHole(float fSizeX, float fSizeY, float fSizeZ, float fThick);
	void CreateOvan(float fRadius, float len, float fHeight);
	void CreateOvanHole(float fRadius, float len, float fHeight, float fradius);
	void CreateCylindrical (int nSegment, float fHeight, float fRadius);
	void SetColor(int colorIdx);

};

//Tạo hình lập phương
void Mesh::CreateCube(float	fSize)
{
	int i;

	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set( fSize, fSize, fSize);
	pt[2].set( fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set( fSize, -fSize, fSize);
	pt[6].set( fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);


	numFaces= 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	
	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for(i = 0; i<face[4].nVerts ; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for(i = 0; i<face[5].nVerts ; i++)
		face[5].vert[i].colorIndex = 5;

}


void Mesh::CreateTetrahedron()
{
	int i;
	numVerts=4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces= 4;
	face = new Face[numFaces];

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	

	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;	
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	
	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;


	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;
}


void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;
			
			//ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]); 
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}
//Set color

void Mesh::SetColor(int colorIdx) {
	for (int f = 0; f < numFaces; f++) {
		for (int v = 0; v < face[f].nVerts; v++) {
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

//Tạo hình hộp chữ nhật
void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ){
	numVerts = 8;
	float x = fSizeX/2;
	float y = fSizeY/2;
	float z = fSizeZ/2;
	pt = new Point3[numVerts];
	pt[0].set(-x, y, z);
	pt[1].set(x, y, z);
	pt[2].set(x, -y, z);
	pt[3].set(-x, -y, z);
	pt[7].set(-x, y, -z);
	pt[6].set(x, y, -z);
	pt[5].set(x, -y, -z);
	pt[4].set(-x, -y, -z);
	
	numFaces = 6;
	face = new Face[numFaces];
	//Khoi tao cac mat
	for(int i=0;i<6;i++){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
	}
	//Mat thu nhat
	for(int i=0;i<4;i++){
		face[0].vert[i%4].vertIndex = i%4;
	}
	//Mat thu hai
	int lan =4;
	for(int i=0;i<4;i++){
		face[1].vert[i].vertIndex = i%4+lan;
	}
	//cac mac con lai
	for(int i=0;i<4;i++){
		face[i+2].vert[0].vertIndex = i;
		face[i+2].vert[1].vertIndex = i+1;
		face[i+2].vert[2].vertIndex = 7-i-1;
		face[i+2].vert[3].vertIndex = 7-i;
	}
	//Set mau
	for (int j =0;j<numFaces;j++){

		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j;

	}
}
//Set tọa đô
void setPtInCubeHole(Point3* pt,int lan,float x, float y, float z){
	pt[0+lan].set(-x, y, z);
	pt[1+lan].set(x, y, z);
	pt[2+lan].set(x, y, -z);
	pt[3+lan].set(-x, y, -z);
	pt[4+lan].set(-x, -y, z);
	pt[5+lan].set(x, -y, z);
	pt[6+lan].set(x, -y, -z);
	pt[7+lan].set(-x, -y, -z);
}
//Tạo hình hộp chữ nhật có lỗ ở giữa
void Mesh::CreateCubeHole(float fSizeX, float fSizeY, float fSizeZ, float fThick)
{

	numVerts = 16;
	pt = new Point3[numVerts];
	
	setPtInCubeHole(pt,0,fSizeX,fSizeY,fSizeZ);
	setPtInCubeHole(pt,8,fSizeX-fThick,fSizeY,fSizeZ-fThick);


	numFaces = 16;
	face = new Face[numFaces];

	int i;
	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	
	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	

	//top face left
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 9;
	face[2].vert[3].vertIndex = 8;
	

	//top face right
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 2;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 11;
	face[3].vert[3].vertIndex = 10;
	

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	
	//------------
	//top face top
	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[6].nVerts];
	face[6].vert[0].vertIndex = 0;
	face[6].vert[1].vertIndex = 3;
	face[6].vert[2].vertIndex = 11;
	face[6].vert[3].vertIndex = 8;
	
	//top face bottom
	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[7].nVerts];
	face[7].vert[0].vertIndex = 1;
	face[7].vert[1].vertIndex = 2;
	face[7].vert[2].vertIndex = 10;
	face[7].vert[3].vertIndex = 9;
	
	int n = 8;
	//bottom face left
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 4;
	face[n].vert[1].vertIndex = 5;
	face[n].vert[2].vertIndex = 13;
	face[n].vert[3].vertIndex = 12;
	
	//bottom face right
	n = 9;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 7;
	face[n].vert[1].vertIndex = 6;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 15;
	
	//bottom face top
	n = 10;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 4;
	face[n].vert[1].vertIndex = 7;
	face[n].vert[2].vertIndex = 15;
	face[n].vert[3].vertIndex = 12;
	
	//bottom face bottom
	n = 11;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 5;
	face[n].vert[1].vertIndex = 6;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 13;
	

	//inner left
	n = 12;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 8;
	face[n].vert[1].vertIndex = 9;
	face[n].vert[2].vertIndex = 13;
	face[n].vert[3].vertIndex = 12;
	
	//inner right
	n = 13;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 11;
	face[n].vert[1].vertIndex = 10;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 15;
	
	//inner top
	n = 14;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 8;
	face[n].vert[1].vertIndex = 11;
	face[n].vert[2].vertIndex = 15;
	face[n].vert[3].vertIndex = 12;
	
	//inner bottom
	n = 15;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 9;
	face[n].vert[1].vertIndex = 10;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 13;
	
		//Set mau
	for (int j =0;j<numFaces;j++){

		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j;

	}

}
//Tạo hình oval
void Mesh::CreateOvan (float fRadius, float len, float fHeight){
	int nSegment = 180 / 10;
	GLfloat angle = PI / nSegment;
	numVerts = 4 * nSegment;
	pt = new Point3[numVerts];
	numFaces = 2 * nSegment + 3;
	face = new Face[numFaces];
	//Set tọa độ đỉnh phía trên
	for (int i = 0; i< nSegment; i++){
		pt[i].set(fRadius *cos(i*angle + PI / 2) - len / 2, fHeight / 2, fRadius*sin(i*angle + PI / 2));
	}
	for (int i = nSegment; i< 2*nSegment; i++){
		pt[i].set(fRadius *cos(i*angle + PI +PI / 2) - len / 2, -fHeight / 2, fRadius*sin(i*angle + PI +PI / 2));
	}
	for (int i = 2*nSegment; i< 3*nSegment; i++){
		pt[i].set(fRadius *cos(i*angle + PI + PI / 2) + len / 2, fHeight / 2, fRadius*sin(i*angle + PI + PI / 2));
		
	}
	for (int i = 3*nSegment; i< 4*nSegment; i++){
		pt[i].set(fRadius *cos(i*angle + PI / 2) + len / 2, -fHeight / 2, fRadius*sin(i*angle +  PI / 2));
	}

	//the surround Faces
	for (int j = 0; j < nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = j + 1;
		face[j].vert[2].vertIndex = j + nSegment + 1;
		face[j].vert[3].vertIndex = j + nSegment;
		
	}
	//the surround Faces
	for (int j = nSegment; j < 2 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j + nSegment;
		face[j].vert[1].vertIndex = j + nSegment + 1;
		face[j].vert[2].vertIndex = j + 2 * nSegment + 1;
		face[j].vert[3].vertIndex = j + 2 * nSegment;
	
	}
	//left
	face[2 * nSegment - 1].nVerts = 4;
	face[2 * nSegment - 1].vert = new VertexID[face[2 * nSegment - 1].nVerts];
	face[2 * nSegment - 1].vert[0].vertIndex = 0;
	face[2 * nSegment - 1].vert[1].vertIndex = nSegment;
	face[2 * nSegment - 1].vert[3].vertIndex = 3 * nSegment - 1;
	face[2 * nSegment - 1].vert[2].vertIndex = 4 * nSegment - 1;
	
	//right
	face[2 * nSegment].nVerts = 4;
	face[2 * nSegment].vert = new VertexID[face[2 * nSegment].nVerts];
	face[2 * nSegment].vert[0].vertIndex = nSegment - 1;
	face[2 * nSegment].vert[1].vertIndex = 2 * nSegment - 1;
	face[2 * nSegment].vert[3].vertIndex = 2 * nSegment;
	face[2 * nSegment].vert[2].vertIndex = 3 * nSegment;
	
	//top
	face[2 * nSegment + 1].nVerts = 2 * nSegment;
	face[2 * nSegment + 1].vert = new VertexID[face[2 * nSegment + 1].nVerts];
	for (int i = 0; i < 2 * nSegment; i++) {
		if (i < nSegment)face[2 * nSegment + 1].vert[i].vertIndex = i;
		else face[2 * nSegment + 1].vert[i].vertIndex = i + nSegment;
	}
	
	//bottom
	face[2 * nSegment + 2].nVerts = 2 * nSegment;
	face[2 * nSegment + 2].vert = new VertexID[face[2 * nSegment + 2].nVerts];
	for (int i = 0; i < 2 * nSegment; i++) {
		if (i < nSegment)face[2 * nSegment + 2].vert[i].vertIndex = i + nSegment;
		else face[2 * nSegment + 2].vert[i].vertIndex = i + nSegment * 2;
	}

		//Set mau
	for (int j =0;j<numFaces;j++){

		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j;

	}
	
}
void Mesh::CreateOvanHole(float fRadius, float len, float fHeight, float fradius)
{
	int nSegment = 180 / 10;
	GLfloat angle = PI / nSegment;
	numVerts = 8 * nSegment;
	pt = new Point3[numVerts];
	numFaces = 8 * nSegment + 8;
	face = new Face[numFaces];
	for (int i = 0; i < 4 * nSegment; i++) {
		if (i < nSegment)pt[i].set(fRadius *cos(i*angle + PI / 2) - len / 2, fHeight / 2, fRadius*sin(i*angle + PI / 2));
		else if (i < 2 * nSegment) pt[i].set(fRadius *cos(i*angle + PI + PI / 2) - len / 2, -fHeight / 2, fRadius*sin(i*angle + PI + PI / 2));
		else if (i < 3 * nSegment) pt[i].set(fRadius *cos(i*angle + PI + PI / 2) + len / 2, fHeight / 2, fRadius*sin(i*angle + PI + PI / 2));
		else  pt[i].set(fRadius *cos(i*angle + PI / 2) + len / 2, -fHeight / 2, fRadius*sin(i*angle + PI / 2));
	}
	for (int i = 4 * nSegment; i < 8 * nSegment; i++) {
		if (i < 5 * nSegment)pt[i].set(fradius *cos(i*angle + PI / 2) - len / 2, fHeight / 2, fradius*sin(i*angle + PI / 2));
		else if (i < 6 * nSegment) pt[i].set(fradius *cos(i*angle + PI + PI / 2) - len / 2, -fHeight / 2, fradius*sin(i*angle + PI + PI / 2));
		else if (i < 7 * nSegment) pt[i].set(fradius *cos(i*angle + PI + PI / 2) + len / 2, fHeight / 2, fradius*sin(i*angle + PI + PI / 2));
		else  pt[i].set(fradius *cos(i*angle + PI / 2) + len / 2, -fHeight / 2, fradius*sin(i*angle + PI / 2));
	}
	//the surround near
	for (int j = 0; j < nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = j + 1;
		face[j].vert[2].vertIndex = j + nSegment + 1;
		face[j].vert[3].vertIndex = j + nSegment;
	}
	//top near
	for (int j = 2 * nSegment; j < 3 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = j + 1;
		face[j].vert[2].vertIndex = j + 4 * nSegment + 1;
		face[j].vert[3].vertIndex = j + 4 * nSegment;
		
	}
	// bottom near
	for (int j = 3 * nSegment; j < 4 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = j + 1;
		face[j].vert[2].vertIndex = j + 4 * nSegment + 1;
		face[j].vert[3].vertIndex = j + 4 * nSegment;
		
	}
	// inner near
	for (int j = 4 * nSegment; j < 5 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j + 2 * nSegment;
		face[j].vert[1].vertIndex = j + 2 * nSegment + 1;
		face[j].vert[2].vertIndex = j + 3 * nSegment + 1;
		face[j].vert[3].vertIndex = j + 3 * nSegment;
		
	}
	//the surround far
	for (int j = nSegment; j < 2 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j + nSegment;
		face[j].vert[1].vertIndex = j + nSegment + 1;
		face[j].vert[2].vertIndex = j + 2 * nSegment + 1;
		face[j].vert[3].vertIndex = j + 2 * nSegment;
		
	}
	// bottom far
	for (int j = 5 * nSegment; j < 6 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j - 4 * nSegment;
		face[j].vert[1].vertIndex = j - 4 * nSegment + 1;
		face[j].vert[2].vertIndex = j + 1;
		face[j].vert[3].vertIndex = j;
		
	}
	// bottom far
	for (int j = 6 * nSegment; j < 7 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j - 2 * nSegment;
		face[j].vert[1].vertIndex = j - 2 * nSegment + 1;
		face[j].vert[2].vertIndex = j - 6 * nSegment + 1;
		face[j].vert[3].vertIndex = j - 6 * nSegment;
		
	}
	// inner far
	for (int j = 7 * nSegment; j < 8 * nSegment - 1; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j - 3 * nSegment;
		face[j].vert[1].vertIndex = j - 3 * nSegment + 1;
		face[j].vert[2].vertIndex = j - 2 * nSegment + 1;
		face[j].vert[3].vertIndex = j - 2 * nSegment;
		
	}
	//left left
	face[8 * nSegment - 1].nVerts = 4;
	face[8 * nSegment - 1].vert = new VertexID[face[8 * nSegment - 1].nVerts];
	face[8 * nSegment - 1].vert[0].vertIndex = 0;
	face[8 * nSegment - 1].vert[1].vertIndex = nSegment;
	face[8 * nSegment - 1].vert[3].vertIndex = 3 * nSegment - 1;
	face[8 * nSegment - 1].vert[2].vertIndex = 4 * nSegment - 1;
	
	//left top
	face[8 * nSegment + 1].nVerts = 4;
	face[8 * nSegment + 1].vert = new VertexID[face[8 * nSegment + 1].nVerts];
	face[8 * nSegment + 1].vert[0].vertIndex = 0;
	face[8 * nSegment + 1].vert[1].vertIndex = 4 * nSegment;
	face[8 * nSegment + 1].vert[3].vertIndex = 3 * nSegment - 1;
	face[8 * nSegment + 1].vert[2].vertIndex = 7 * nSegment - 1;
	
	//left right
	face[8 * nSegment + 2].nVerts = 4;
	face[8 * nSegment + 2].vert = new VertexID[face[8 * nSegment + 2].nVerts];
	face[8 * nSegment + 2].vert[0].vertIndex = 4 * nSegment;
	face[8 * nSegment + 2].vert[1].vertIndex = 5 * nSegment;
	face[8 * nSegment + 2].vert[3].vertIndex = 7 * nSegment - 1;
	face[8 * nSegment + 2].vert[2].vertIndex = 8 * nSegment - 1;
	
	//right right
	face[8 * nSegment].nVerts = 4;
	face[8 * nSegment].vert = new VertexID[face[8 * nSegment].nVerts];
	face[8 * nSegment].vert[0].vertIndex = nSegment - 1;
	face[8 * nSegment].vert[1].vertIndex = 2 * nSegment - 1;
	face[8 * nSegment].vert[3].vertIndex = 2 * nSegment;
	face[8 * nSegment].vert[2].vertIndex = 3 * nSegment;
	
	//right left
	face[8 * nSegment + 3].nVerts = 4;
	face[8 * nSegment + 3].vert = new VertexID[face[8 * nSegment + 3].nVerts];
	face[8 * nSegment + 3].vert[0].vertIndex = 5 * nSegment - 1;
	face[8 * nSegment + 3].vert[1].vertIndex = 6 * nSegment - 1;
	face[8 * nSegment + 3].vert[3].vertIndex = 6 * nSegment;
	face[8 * nSegment + 3].vert[2].vertIndex = 7 * nSegment;
	
	//right top
	face[8 * nSegment + 4].nVerts = 4;
	face[8 * nSegment + 4].vert = new VertexID[face[8 * nSegment + 4].nVerts];
	face[8 * nSegment + 4].vert[0].vertIndex = 5 * nSegment - 1;
	face[8 * nSegment + 4].vert[1].vertIndex = nSegment - 1;
	face[8 * nSegment + 4].vert[3].vertIndex = 6 * nSegment;
	face[8 * nSegment + 4].vert[2].vertIndex = 2 * nSegment;
	
	//left bot
	face[8 * nSegment + 5].nVerts = 4;
	face[8 * nSegment + 5].vert = new VertexID[face[8 * nSegment + 5].nVerts];
	face[8 * nSegment + 5].vert[3].vertIndex = nSegment;
	face[8 * nSegment + 5].vert[1].vertIndex = 8 * nSegment - 1;
	face[8 * nSegment + 5].vert[2].vertIndex = 4 * nSegment - 1;
	face[8 * nSegment + 5].vert[0].vertIndex = 5 * nSegment;
	
	//right bot
	face[8 * nSegment + 6].nVerts = 4;
	face[8 * nSegment + 6].vert = new VertexID[face[8 * nSegment + 6].nVerts];
	face[8 * nSegment + 6].vert[0].vertIndex = 2 * nSegment - 1;
	face[8 * nSegment + 6].vert[1].vertIndex = 3 * nSegment;
	face[8 * nSegment + 6].vert[3].vertIndex = 6 * nSegment - 1;
	face[8 * nSegment + 6].vert[2].vertIndex = 7 * nSegment;
	
	
		//Set mau
	for (int j =0;j<numFaces;j++){

		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j;

	}

}
void Mesh::CreateCylindrical (int nSegment, float fHeight, float fRadius)
{
	numVerts = 2 * nSegment + 2;
	GLfloat angle = 2 * PI / nSegment;
	pt = new Point3[numVerts];
	//Set tọa độ đỉnh phía trên
	for (int i = 0; i< nSegment; i++){
		pt[i].set(fRadius *cos(i*angle), fHeight / 2, fRadius*sin(i*angle));
	}
	//Set tọa độ đỉnh phía dưới
	for (int i = nSegment; i< 2 * nSegment; i++){
		pt[i].set(fRadius *cos((i - nSegment)*angle), -fHeight / 2, fRadius*sin((i - nSegment)*angle));
	}
	//Set toa độ tâm
	pt[2 * nSegment].set(0.0, fHeight / 2, 0.0);
	pt[2 * nSegment + 1].set(0.0, -fHeight / 2, 0.0);

	numFaces = 3 * nSegment;
	face = new Face[numFaces];
	//the top 
	for (int j = 0; j < nSegment; j++) {
		face[j].nVerts = 3;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j;
		face[j].vert[1].vertIndex = 2 * nSegment;
		if (j == nSegment - 1)
			face[j].vert[2].vertIndex = 0;
		else
			face[j].vert[2].vertIndex = j + 1;
		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j + 3 * i;
	}

	//the surround Faces
	for (int j = nSegment; j < 2 * nSegment; j++) {
		face[j].nVerts = 4;
		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = j - nSegment;
		if (j == 2 * nSegment - 1)
			face[j].vert[1].vertIndex = 0;
		else
			face[j].vert[1].vertIndex = j - nSegment + 1;
		if (j == 2 * nSegment - 1)
			face[j].vert[2].vertIndex = nSegment;
		else
			face[j].vert[2].vertIndex = j + 1;
		face[j].vert[3].vertIndex = j;
		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j + i - nSegment;
	}
	//bottom face
	for (int j = 2 * nSegment, k = nSegment; j < 3 * nSegment; j++, k++) {
		face[j].nVerts = 3;

		face[j].vert = new VertexID[face[j].nVerts];
		face[j].vert[0].vertIndex = k;
		face[j].vert[1].vertIndex = 2 * nSegment + 1;
		if (j == 3 * nSegment - 1)
			face[j].vert[2].vertIndex = 2 * nSegment + 1;
		else
			face[j].vert[2].vertIndex = k + 1;
		for (int i = 0; i<face[j].nVerts; i++)
			face[j].vert[i].colorIndex = j + 3 * i;
	}
}

//Code main
//-------------------------------------------------------------------------------------

using namespace std;

int		screenWidth = 600;
int		screenHeight = 600;

bool	bWireFrame = false;

float	baseRadius = 1;
float	baseHeight = 0.2;
float	baseRotateStep = 10;

float	columnSizeX = 0.25;
float	columnSizeZ = columnSizeX;
float	columnSizeY = 5;

float camera_angle;
float camera_height ;
float camera_dis ;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z ;

bool	b4View = false;
bool	autoplay = false;

Mesh	base;
Mesh	column;
Mesh	thanhngang;
Mesh    giadotren;
Mesh    giadoduoi;
Mesh    giadotren_c;
Mesh    giadoduoi_c;
Mesh    trucquay;
Mesh    diemquay;
Mesh    ovandaylen;
Mesh    thanhdaylen;
Mesh	ovandayngang;
Mesh    thangdayngang;


void mySpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		camera_height += 0.5;
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.5;
		break;
	case GLUT_KEY_LEFT:
		camera_angle += 10;
		break;
	case GLUT_KEY_RIGHT:
		camera_angle -= 10;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		base.rotateY += baseRotateStep;
		if (base.rotateY > 360)
			base.rotateY -= 360;
		break;
	case '2':
		base.rotateY -= baseRotateStep;
		if (base.rotateY < 0)
			base.rotateY += 360;
		break;
	case '3':
		trucquay.rotateZ += baseRotateStep;
		if (trucquay.rotateZ > 360)
			trucquay.rotateZ -= 360;
		break;
	case '4':
		trucquay.rotateZ -= baseRotateStep;
		if (trucquay.rotateZ < 0)
			trucquay.rotateZ += 360;
		break;
	case 'A':
	case 'a':
		autoplay = !autoplay;
		break;
	case 'R':
	case 'r':
		base.rotateY = 0;
		trucquay.rotateZ = 0;
		break;
	case 'W':
	case 'w':
		bWireFrame = !bWireFrame;
		break;
	case 'v':
		b4View = !b4View;
		break;
	case 'V':
		b4View = !b4View;
		break;
	case '+':
		camera_dis += 0.5;
		break;
	case '-':
		camera_dis -= 0.5;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}
void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);//x
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);//y
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);//z
	glVertex3f(0, 0, 4);
	glEnd();
}

void drawBase()
{
	glPushMatrix();

	glTranslated(0, baseHeight / 2.0, 0);
	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		base.DrawWireframe();
	else
		base.DrawColor();

	glPopMatrix();
}
void drawColumn()
{
	glPushMatrix();

	glTranslated(0, baseHeight / 2.0 + columnSizeY / 2, 0);
	glRotatef(base.rotateY, 0, 1, 0);

	if (bWireFrame)
		column.DrawWireframe();
	else
		column.DrawColor();

	glPopMatrix();
}
void drawThanhngang()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(columnSizeY/3 + columnSizeX/2 , columnSizeY * 4 / 12 , 0);
	if (bWireFrame)
		thanhngang.DrawWireframe();
	else
		thanhngang.DrawColor();
	glPopMatrix();
}
void drawGiadoduoi() {
	glPushMatrix();
	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated(columnSizeZ + baseHeight + columnSizeZ * 1 / 3, columnSizeY * 3 / 10 + columnSizeX / 2, -columnSizeY * 0.39);
	glRotatef(90, 1, 0, 0);

	if (bWireFrame)
		giadoduoi.DrawWireframe();
	else
		giadoduoi.DrawColor();

	glPopMatrix();
}
void drawGiadoduoi_c() {
	glPushMatrix();
	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated(columnSizeZ - (columnSizeX - baseHeight) / 2, columnSizeY * 3 / 10 + columnSizeX / 2, -columnSizeY * 0.39);

	if (bWireFrame)
		giadoduoi_c.DrawWireframe();
	else
		giadoduoi_c.DrawColor();

	glPopMatrix();
}
void drawGiadotren() {
	glPushMatrix();

	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated(columnSizeX * 1 / 3 + columnSizeX * 2 + baseHeight, columnSizeY * 11 / 16 + columnSizeX / 2, 0);

	if (bWireFrame)
		giadotren.DrawWireframe();
	else
		giadotren.DrawColor();

	glPopMatrix();
}

void drawGiadotren_c() {
	glPushMatrix();
	glRotatef(base.rotateY - 90, 0, 1, 0);
	glTranslated((columnSizeX * 2 + baseHeight) / 2, columnSizeY * 11 / 16 + columnSizeX / 2, 0);

	if (bWireFrame)
		giadotren_c.DrawWireframe();
	else
		giadotren_c.DrawColor();

	glPopMatrix();
}
void drawTrucquay() {
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, (columnSizeY * 3 / 10 + columnSizeX / 2), (columnSizeZ / 3 + baseHeight) / 2 + columnSizeX / 2);
	glRotatef(90, 1, 0, 0);
	glRotatef(trucquay.rotateZ, 0, 1, 0);		// quanh truc
	glTranslated((columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 10, 0, 0);

	if (bWireFrame)
		trucquay.DrawWireframe();
	else
		trucquay.DrawColor();

	glPopMatrix();
}
void drawDiemquay() {
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, (columnSizeY * 3 / 10 + columnSizeX / 2), (columnSizeZ / 3 + baseHeight) + columnSizeX / 2 + columnSizeZ * (7.f / 3) / 2);
	glRotatef(90, 1, 0, 0);
	glRotatef(trucquay.rotateZ, 0, 1, 0);		// quanh truc
	glTranslated((columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5, 0, 0);

	if (bWireFrame)
		diemquay.DrawWireframe();
	else
		diemquay.DrawColor();

	glPopMatrix();
}
void drawOvandaylen() {
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, (columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5 * sin(trucquay.rotateZ*DEG2RAD) + (columnSizeY * 3 / 10 + columnSizeX / 2), (columnSizeZ / 3 + baseHeight) + columnSizeX * 2);
	glRotatef(90, 1, 0, 0);

	if (bWireFrame)
		ovandaylen.DrawWireframe();
	else
		ovandaylen.DrawColor();

	glPopMatrix();
}
void drawOvandayngang() {
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated((columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5 * cos(trucquay.rotateZ*DEG2RAD), (columnSizeY * 3 / 10 + columnSizeX / 2), (columnSizeZ / 3 + baseHeight) + columnSizeX);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);

	if (bWireFrame)
		ovandayngang.DrawWireframe();
	else
		ovandayngang.DrawColor();

	glPopMatrix();
}
void drawThanhdaylen()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, (columnSizeY * 3 / 10 + columnSizeX / 2) + baseHeight + (columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5 * sin(trucquay.rotateZ*DEG2RAD) + columnSizeY * 3 / 10, (columnSizeZ / 3 + baseHeight) + columnSizeX * 2);

	if (bWireFrame)
		thanhdaylen.DrawWireframe();
	else
		thanhdaylen.DrawColor();

	glPopMatrix();
}
void drawThanhdayngang()
{
	glPushMatrix();
	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated((columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5 * cos(trucquay.rotateZ*DEG2RAD) + columnSizeY * 3 / 10 + baseHeight, (columnSizeY * 3 / 10 + columnSizeX / 2), (columnSizeZ / 3 + baseHeight) + columnSizeX);
	glRotatef(90, 0, 0, 1);

	if (bWireFrame)
		thangdayngang.DrawWireframe();
	else
		thangdayngang.DrawColor();

	glPopMatrix();
}
void drawAllShape(){
	
	drawAxis();
	drawBase();
	drawColumn();
	drawThanhngang();
	drawGiadotren();
	drawGiadoduoi();
	drawGiadotren_c();
	drawGiadoduoi_c();
	drawTrucquay();
	drawDiemquay();
	drawOvandaylen();
	drawOvandayngang();
	drawThanhdaylen();
	drawThanhdayngang();
}
void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);
	
	camera_X = camera_dis * sinf(camera_angle*DEG2RAD);
	camera_Y = camera_height;
	camera_Z = camera_dis * cosf(camera_angle*DEG2RAD);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (b4View) {
		glViewport(0, 0, screenWidth / 2, screenHeight / 2);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(6, 0, 0, 0, 0, 0, 0, 1, 0);
		drawAllShape();

		glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight / 2);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 6, 0, 0, 0, 0, 0, 0, -1);
		drawAllShape();

		glViewport(screenWidth / 2, screenHeight / 2, screenWidth / 2, screenHeight / 2);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);
		drawAllShape();

		glViewport(0, screenHeight / 2, screenWidth / 2, screenHeight / 2);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);
		drawAllShape();
	}
	else {
		glViewport(0, 0, screenWidth, screenHeight);
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);
		drawAllShape();
	}

	glFlush();
	glutSwapBuffers();

}

void myInit()
{
	float	fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 100);
	glMatrixMode(GL_MODELVIEW);
	//Khởi tạo giá trị mặc định cho camera
	camera_angle = 45;
	camera_height = 2.4;
	camera_dis = 6.5;
	lookAt_X = 0;
	lookAt_Y = 1;
	lookAt_Z = 0; 
}

void processTimer(int value){
	if(autoplay){
		base.rotateY += 5;
		if (base.rotateY < 360)
			base.rotateY -= 360;
		trucquay.rotateZ -= 5;
		if (trucquay.rotateZ < 0)
				trucquay.rotateZ += 360;
	}
	glutTimerFunc(100, processTimer, value);
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Assignment1- Cao Phúc- 1512500"); // open the screen window

	base.CreateCylindrical(20, baseHeight, baseRadius);
	base.SetColor(6);

	column.CreateCuboid(columnSizeZ, columnSizeY, columnSizeZ);
	column.SetColor(1);

	thanhngang.CreateCuboid(columnSizeY* 2 / 3, columnSizeZ, columnSizeZ);
	thanhngang.SetColor(0);
	
	giadoduoi.CreateCubeHole(columnSizeZ * 5 / 6, columnSizeZ * 5 / 6, columnSizeZ * 5 / 6, columnSizeZ / 3);
	giadoduoi.SetColor(6);


	giadoduoi_c.CreateCuboid(baseHeight, columnSizeZ, columnSizeZ);
	giadoduoi_c.SetColor(6);
	
	giadotren.CreateCubeHole(columnSizeZ * 5 / 6, columnSizeZ * 5 / 6, columnSizeZ * 5 / 6, columnSizeZ / 3);
	giadotren.SetColor(6);
	
	giadotren_c.CreateCuboid(columnSizeZ + baseHeight, columnSizeZ, columnSizeZ);
	giadotren_c.SetColor(6);

	trucquay.CreateOvan(columnSizeZ / 2, (columnSizeY * 3 / 10 + columnSizeX / 2) * 3 / 5, columnSizeZ / 3 + baseHeight);
	trucquay.SetColor(6);

	diemquay.CreateCylindrical(20, columnSizeZ*(7.f / 3), baseHeight / 2);
	diemquay.SetColor(4);

	ovandaylen.CreateOvanHole(baseHeight, (columnSizeY * 3 / 10 + columnSizeX / 2) * 6 / 5, columnSizeX, baseHeight / 2);
	ovandaylen.SetColor(0);

	thanhdaylen.CreateCuboid(columnSizeZ, columnSizeY * 3 / 5, columnSizeZ);
	thanhdaylen.SetColor(0);

	ovandayngang.CreateOvanHole(baseHeight, (columnSizeY * 3 / 10 + columnSizeX / 2) * 6 / 5, columnSizeX, baseHeight / 2);
	ovandayngang.SetColor(2);

	thangdayngang.CreateCuboid(columnSizeZ, columnSizeY * 3 / 5, columnSizeZ);
	thangdayngang.SetColor(2);

	myInit();
	glutTimerFunc(100, processTimer, 10);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}
