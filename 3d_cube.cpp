#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <conio.h>
#include <math.h>

HWND hWnd;

struct vertex {
	float x,y,z;
};

enum { A, B, C, D, E, F, G, H };

vertex vert_buff[8] = {
	-4.0, -4.0, -4.0,	//A
	 4.0, -4.0, -4.0,	//B
	-4.0,  4.0, -4.0,	//C
	 4.0,  4.0, -4.0,	//D

 	-4.0, -4.0, 4.0,	//E
	 4.0, -4.0, 4.0,	//F
	-4.0,  4.0, 4.0,	//G
	 4.0,  4.0, 4.0 };	//H

vertex vert_buff_res[8];

/*
FONT SIDE	BACK SIDE
C - D		G - H
|	|		|	|
A - B		E - F
*/

unsigned int index_buff[24] = {
	//FRONT SIDE
	A, B,
	B, D,
	D, C,
	C, A,

	//BACK SIDE
	E, F,
	F, H,
	H, G,
	G, E,

	//LINES CONNECT FRONT AND BACK
	A, E,
	B, F,
	D, H,
	C, G };


typedef float matrix4x4[4][4];

vertex Vec3_Mat4x4_Mul(vertex v, matrix4x4 m)
{
	vertex t;

	t.x =	v.x * m[0][0] +
			v.y * m[1][0] +
			v.z * m[2][0] +
				  m[3][0];

	t.y =	v.x * m[0][1] +
			v.y * m[1][1] +
			v.z * m[2][1] +
				  m[3][1];

	t.z =	v.x * m[0][2] +
			v.y * m[1][2] +
			v.z * m[2][2] +
				  m[3][2];

	return t;
}

void Draw_Cube()
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	static float angle = 0.0;

	matrix4x4 mxRotateY = {
		cosf(angle),	0.0,	sinf(angle),	0.0,
		0.0,			1.0,	0.0,			0.0,
		-sinf(angle),	0.0,	cosf(angle),	0.0,
		0.0,			0.0,	0.0,			1.0 };

	angle = angle + (3.1415926f / 100.0f);
	if(angle > (3.1415926f * 2.0f) )
		angle = 0.0f;

	
	for ( int i = 0; i < 8; i++ )
	{

		//vertex v = vert_buff[i];
		vertex v = Vec3_Mat4x4_Mul(vert_buff[i], mxRotateY);

		v.x = v.x / ((float)rc.right / (float)rc.bottom);
		v.z = v.z + 15.0f;
		v.x = v.x / v.z;
		v.y = v.y / v.z;

		v.x = v.x * rc.right / 2.0f + rc.right / 2.0f;
		v.y =-v.y * rc.bottom / 2.0f + rc.bottom / 2.0f;

		vert_buff_res[i] = v;
	}

	HDC hDC = GetDC(hWnd);

	HBRUSH hBrush = CreateSolidBrush( RGB ( 0, 0, 0) );
	HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);

	Rectangle(hDC, 0, 0, rc.right, rc.bottom);

	HPEN hPen = CreatePen(PS_SOLID, 4, RGB( 255, 255, 127 ) );
	HPEN hOldPen = (HPEN) SelectObject(hDC, hPen);

	for ( int i = 0; i < 12; i++ )
	{
		vertex v1 = vert_buff_res[ index_buff[ i * 2 ] ];
		vertex v2 = vert_buff_res[ index_buff[ i * 2 + 1 ] ];

		MoveToEx(hDC, (int)v1.x, (int)v1.y, NULL);
		LineTo(hDC, (int)v2.x, (int)v2.y);
	}


	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	
	ReleaseDC(hWnd, hDC);

	Sleep(25);
}


int main ()
{
	hWnd = GetConsoleWindow();

	while ( !_kbhit() )
	{
		Draw_Cube();
	}

	return 0;
}


