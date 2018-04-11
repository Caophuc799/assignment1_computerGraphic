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


float	ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0}, 
								{1.0, 1.0,  0.0}, { 1.0, 0.0, 1.0},{ 0.0, 1.0, 1.0}, 
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};




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
};


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
			
			ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]); 
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

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
void Mesh::CreateCubeHole(float fSizeX, float fSizeY, float fSizeZ, float fThick)
{
	int i, j = 0;
	numVerts = 8 * 2;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX, fSizeY, fSizeZ);
	pt[1].set(fSizeX, fSizeY, fSizeZ);
	pt[2].set(fSizeX, fSizeY, -fSizeZ);
	pt[3].set(-fSizeX, fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY, fSizeZ);
	pt[5].set(fSizeX, -fSizeY, fSizeZ);
	pt[6].set(fSizeX, -fSizeY, -fSizeZ);
	pt[7].set(-fSizeX, -fSizeY, -fSizeZ);
	float neX, neY, neZ;
	neX = fSizeX - fThick;
	neY = fSizeY;
	neZ = fSizeZ - fThick;
	pt[8].set(-neX, neY, neZ);
	pt[9].set(neX, neY, neZ);
	pt[10].set(neX, neY, -neZ);
	pt[11].set(-neX, neY, -neZ);
	pt[12].set(-neX, -neY, neZ);
	pt[13].set(neX, -neY, neZ);
	pt[14].set(neX, -neY, -neZ);
	pt[15].set(-neX, -neY, -neZ);


	numFaces = 16;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face left
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 9;
	face[2].vert[3].vertIndex = 8;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//top face right
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 2;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 11;
	face[3].vert[3].vertIndex = 10;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
	//------------
	//top face top
	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[6].nVerts];
	face[6].vert[0].vertIndex = 0;
	face[6].vert[1].vertIndex = 3;
	face[6].vert[2].vertIndex = 11;
	face[6].vert[3].vertIndex = 8;
	for (i = 0; i<face[6].nVerts; i++)
		face[6].vert[i].colorIndex = 6;
	//top face bottom
	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[7].nVerts];
	face[7].vert[0].vertIndex = 1;
	face[7].vert[1].vertIndex = 2;
	face[7].vert[2].vertIndex = 10;
	face[7].vert[3].vertIndex = 9;
	for (i = 0; i<face[7].nVerts; i++)
		face[7].vert[i].colorIndex = 7;
	int n = 8;
	//bottom face left
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 4;
	face[n].vert[1].vertIndex = 5;
	face[n].vert[2].vertIndex = 13;
	face[n].vert[3].vertIndex = 12;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//bottom face right
	n = 9;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 7;
	face[n].vert[1].vertIndex = 6;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 15;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//bottom face top
	n = 10;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 4;
	face[n].vert[1].vertIndex = 7;
	face[n].vert[2].vertIndex = 15;
	face[n].vert[3].vertIndex = 12;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//bottom face bottom
	n = 11;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 5;
	face[n].vert[1].vertIndex = 6;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 13;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;

	//inner left
	n = 12;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 8;
	face[n].vert[1].vertIndex = 9;
	face[n].vert[2].vertIndex = 13;
	face[n].vert[3].vertIndex = 12;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//inner right
	n = 13;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 11;
	face[n].vert[1].vertIndex = 10;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 15;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//inner top
	n = 14;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 8;
	face[n].vert[1].vertIndex = 11;
	face[n].vert[2].vertIndex = 15;
	face[n].vert[3].vertIndex = 12;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;
	//inner bottom
	n = 15;
	face[n].nVerts = 4;
	face[n].vert = new VertexID[face[n].nVerts];
	face[n].vert[0].vertIndex = 9;
	face[n].vert[1].vertIndex = 10;
	face[n].vert[2].vertIndex = 14;
	face[n].vert[3].vertIndex = 13;
	for (i = 0; i<face[n].nVerts; i++)
		face[n].vert[i].colorIndex = n;

}


//Code main
//-------------------------------------------------------------------------------------

using namespace std;
int angle = 0;
int		screenWidth = 600;
int		screenHeight= 300;

Mesh	tetrahedron;
Mesh	cube;
Mesh	cuboid;
Mesh	cylinder;
Mesh    holecuboid;
Mesh    oval;
Mesh    holeoval;
int		nChoice = 1;

void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(4, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 4, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 4);
	glEnd();
}
void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4.5*cos(angle*3.14159f/180.f), 4, 4.5 * sin(angle*3.14159f / 180.f), 0, 0, 0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glViewport(0, 0, screenWidth/2, screenHeight);
	
	drawAxis();

	glColor3f(0, 0, 0);
	if(nChoice == 1)
		tetrahedron.DrawWireframe();
	else if(nChoice == 2)
		cube.DrawWireframe();
	else if (nChoice == 3)
		cuboid.DrawWireframe();
	else if (nChoice == 4)
		cylinder.DrawWireframe();
	else if (nChoice == 5)
		holecuboid.DrawWireframe();
	else if (nChoice == 6)
		oval.DrawWireframe();
	else if (nChoice == 7)
		holeoval.DrawWireframe();

	glViewport(screenWidth/2, 0, screenWidth/2, screenHeight);

	drawAxis();
	if(nChoice == 1)
		tetrahedron.DrawColor();
	else if(nChoice == 2)
		cube.DrawColor();
	else if (nChoice == 3)
		cuboid.DrawColor();
	else if (nChoice == 4)
		cylinder.DrawColor();
	else if (nChoice == 5)
		holecuboid.DrawColor();
	else if (nChoice == 6)
		oval.DrawColor();
	else if (nChoice == 7)
		holeoval.DrawColor();
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
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000, 1000);
}
void processTimer(int value) {
	angle += (GLfloat)value;
	if (angle > 360) angle = angle - 360.0f;
	glutTimerFunc(25, processTimer, 1);
	glutPostRedisplay();
}
int main(int argc, char* argv[])
{
	cout << "1. Tetrahedron" << endl;
	cout << "2. Cube" << endl;
	cout << "3. Cuboid" << endl;
	cout << "4. Cylinder" << endl;
	cout << "5. HoleCuboid" << endl;
	cout << "6. Oval" << endl;
	cout << "7. HoleOval" << endl;

	cout << "Input the choice: " << endl;
	cin  >> nChoice;

	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Assignment1- Cao Phúc- 1512500"); // open the screen window

	tetrahedron.CreateTetrahedron();
	cube.CreateCube(1);
	cuboid.CreateCuboid(2, 0.8, 0.8);
	/*cylinder.CreateCylinder(20, 3, 1);
	holecuboid.CreateCuboidHole(2, 0.8, 0.8, 0.35);
	oval.CreateOval(2, 2, 0.8);
	holeoval.CreateHoleOval(2, 2, 0.8, 1);*/
	myInit();
    glutDisplayFunc(myDisplay);
	glutTimerFunc(25, processTimer, 1);
	glutMainLoop();
	return 0;
}

