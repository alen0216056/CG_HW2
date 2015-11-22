#include "view.h"
#include <math.h>
#define PI 3.14159265

view::view(const char* file_name)
{
	string tmp;
	char buffer[100];
	fstream f_view;
	f_view.open(file_name, ios::in);

	if (!f_view)
	{
		cout << "can not open the file\n";
		return;
	}

	while (f_view>>tmp)
	{
		if (tmp=="eye")
		{
			f_view >> eye[0] >> eye[1] >> eye[2];
		}
		else if (tmp=="vat")
		{
			f_view >> at[0] >> at[1] >> at[2];
		}
		else if (tmp=="vup")
		{
			f_view >> up[0] >> up[1] >> up[2];
		}
		else if (tmp=="fovy")
		{
			f_view >> fovy;
		}
		else if (tmp == "dnear")
		{
			f_view >> near;
		}
		else if (tmp == "dfar")
		{
			f_view >> far;
		}
		else if (tmp == "viewport")
		{
			f_view >> viewport[0] >> viewport[1] >> viewport[2] >> viewport[3];
		}	
	}
	f_view.close();
}

view::~view()
{
}

void view::viewing()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (float)viewport[2] / viewport[3], near, far);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void view::zoom(bool flag)
{
	double scale = flag ? 0.01 : -0.01;
	eye[0] += (at[0] - eye[0]) * scale;
	eye[1] += (at[1] - eye[1]) * scale;
	eye[2] += (at[2] - eye[2]) * scale;
}

void view::rotate(bool flag)
{
	double new_eye[3];
	double theta = flag ? -10.0 * PI / 180 : 10.0 * PI / 180;

	//double vx = at[0] - eye[0], vy = at[1] - eye[1], vz = at[2] - eye[2];
	double vx = up[0], vy = up[1], vz = up[2];
	double tmp = sqrt(pow(vx, 2.0) + pow(vy, 2.0) + pow(vz, 2.0));
	vx /= tmp;
	vy /= tmp;
	vz /= tmp;

	for (int i = 0; i < 3; i++)
		eye[i] = eye[i] - at[i];

	new_eye[0] = (cos(theta) + (1 - cos(theta))*pow(vx, 2.0))*eye[0] + ((1 - cos(theta))*vx*vy - sin(theta)*vz)*eye[1] + ((1 - cos(theta))*vx*vz + sin(theta)*vy)*eye[2];
	new_eye[1] = ((1 - cos(theta))*vy*vx + sin(vz))*eye[0] + (cos(theta) + (1 - cos(theta))*pow(vy, 2.0))*eye[1] + ((1 - cos(theta))*vy*vz - sin(theta)*vx)*eye[2];
	new_eye[2] = ((1 - cos(theta))*vz*vx - sin(theta)*vy)*eye[0] + ((1 - cos(theta))*vz*vy + sin(theta)*vx)*eye[1] + (cos(theta) + (1 - cos(theta))*pow(vz, 2.0))*eye[2];
	
	for (int i = 0; i < 3; i++)
		eye[i] = new_eye[i] + at[i];
}