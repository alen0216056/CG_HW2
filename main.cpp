#include "scene.h"
#include "light.h"
#include "view.h"

#include "FreeImage.h"
#include "glew.h"
#include "glut.h"

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse_motion(int x, int y);

int width = 1024, height = 768;
scenes* s;
lights* l;
view* v;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW2");

	l = new lights("park.light");
	v = new view("park.view");
	s = new scenes("park.scene");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutMainLoop();
	return 0;
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      
	glClearDepth(1.0f);                        
	glEnable(GL_DEPTH_TEST);                  
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	v->viewing();
	l->lighting();
	s->scene_setting();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			v->zoom(1);
		break;
		case 'a':
			v->rotate(1);
		break;
		case 's':
			v->zoom(0);
		break;
		case 'd':
			v->rotate(0);
		break;
		default:
			if (key >= '0' && key <= '9')
				s->select(key - '0');
		break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state)
				s->set_x_y(-1, -1);
			else
				s->set_x_y(x, y);
		break;
		case GLUT_RIGHT_BUTTON :
			if (state)
				s->set_x_y(-1, -1);
			else
				s->set_x_y(x, y);
		break;
	}
}

void mouse_motion(int x, int y)
{
	s->move(x, y, v->viewport[2], v->viewport[3]);
	s->set_x_y(x, y);
	glutPostRedisplay();
}
