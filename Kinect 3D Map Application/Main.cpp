// (your name here)
// Chess animation starter kit.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;
#include "glut.h"

#include "Parse.h"
#include "Traverse.h"
#include "Sphere.h"


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = {0.7, 0.1, 0.2, 1.0};
GLdouble greenMaterial[] = {0.1, 0.7, 0.4, 1.0};
GLdouble brightGreenMaterial[] = {0.1, 0.9, 0.1, 1.0};
GLdouble blueMaterial[] = {0.1, 0.2, 0.7, 1.0};
GLdouble whiteMaterial[] = {1.0, 1.0, 1.0, 1.0};

double screen_x = 600;
double screen_y = 500;

//class to change location
Traverse traverse;

//vector of spheres
vector<Sphere> g_spheres;
double g_radius = 5.0;

double Get_delta_time()
{
	//using static here tells it to set the value the first time it has been hit
	static clock_t start_time = clock();
	static int current_frame = 0;
	clock_t current_time = clock();
	current_frame++;
	double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
	if(total_time == 0)
	{
		total_time = .00001;
	}
	double frames_per_second = (double)current_frame / total_time;
	double dt = 1.0 / frames_per_second;

	return dt;
}

// Outputs a string of text at the specified location.
void text_output(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}

// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal vector n[0], n[1], n[2].
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];
	
	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1]*v2[2] - v1[2]*v2[1];
	n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n[2] = v1[0]*v2[1] - v1[1]*v2[0];

	double size = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}

// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void DrawPiece(char filename[])
{
	// Try to open the given file.
	char buffer[200];
	ifstream in(filename);
	if(!in)
	{
		cerr << "Error. Could not open " << filename << endl;
		exit(1);
	}

	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	do
	{
		in.getline(buffer, 200); // get one line (point) from the file.
		int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		done = in.eof();
		if(!done)
		{
			if(count == 3) // if this line had an x,y,z point.
			{
				verts++;
			}
			else // the line was empty. Finish current polygon and start a new one.
			{
				if(verts>=3)
				{
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for(int i=0; i<verts; i++)
					{
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
			}
		}
	}
	while(!done);

	if(verts>0)
	{
		cerr << "Error. Extra vertices in file " << filename << endl;
		exit(1);
	}

}

// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = {4500, 8000, -4000}; // pick a nice vantage point.
double at[3]  = {4500, 0,     4000};
//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	double delta_time = Get_delta_time();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = {0.8, 0.9, 0.5, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);

	glPushMatrix();
	glTranslatef(3000, 0, 1000);
	DrawPiece("BISHOP.POL");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, 1000);
	DrawPiece("KING.POL");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5000, 0, 1000);
	DrawPiece("QUEEN.POL");
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6000, 0, 1000);
	DrawPiece("BISHOP.POL");
	glPopMatrix();

	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = {0.1, 0.5, 0.8, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);

	glPushMatrix();
	glTranslatef(4000, 0, 8000);
	DrawPiece("KING.POL");
	glPopMatrix();

	for(int x=1000; x<=8000; x+=1000)
	{
		glPushMatrix();
		glTranslatef(x, 0, 7000);
		DrawPiece("PAWN.POL");
		glPopMatrix();
	}

	//display and update spheres
	for(int i = 0; i < g_spheres.size(); i++)
	{
		g_spheres[i].Update(delta_time, 1);//add correct current time
	}

	for(int i = 0; i < g_spheres.size(); i++)
	{
		g_spheres[i].Draw();
	}

	glutSwapBuffers();
}

//create a point
Point Create_point(double x, double y, double z, double r, double g, double b)
{
	Point point;
	point.X = x;
	point.Y = y;
	point.Z = z;
	point.R = r;
	point.G = g;
	point.B = b;

	return point;
}

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;

		case 'w': //MOVE EYE FORWARD
			traverse.Move_forward(eye, at, .5);
			break;

		case 'W': //MOVE EYE FORWARD
			traverse.Move_forward(eye, at, .5);
			break;

		case 'a': //MOVE EYE LEFT
			traverse.Move_left(eye, at, .5);
			break;

		case 'A': //MOVE EYE LEFT
			traverse.Move_left(eye, at, .5);
			break;

		case 's': //MOVE EYE BACK
			traverse.Move_back(eye, at, .5);
			break;

		case 'S': //MOVE EYE BACK
			traverse.Move_back(eye, at, .5);
			break;

		case 'd': //MOVE EYE RIGHT
			traverse.Move_right(eye, at, .5);
			break;

		case 'D': //MOVE EYE RIGHT
			traverse.Move_right(eye, at, .5);
			break;

		case 'p':
			Point point = Create_point(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
			Sphere sphere(point);
			g_spheres.push_back(sphere);
			break;

		case 'P':
			Point point = Create_point(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
			Sphere sphere(point);
			g_spheres.push_back(sphere);
			break;

		case 'i':
			g_radius -= .5;
			break;

		case 'I':
			g_radius -= .5;
			break;

		case 'o':
			g_radius += .5;
			break;

		case 'O':
			g_radius += .5;
			break;


		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 38.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ 100, /* Z far */ 30000.0);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	// set material's specular properties
	//GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	//GLfloat mat_shininess[] = {50.0};
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//// set light properties
	//GLfloat light_position[] = {eye[0], eye[1], eye[2],1};
	//GLfloat white_light[] = {1,1,1,1};
	//GLfloat low_light[] = {.3,.3,.3,1};
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	//glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

	//glEnable(GL_DEPTH_TEST); // turn on depth buffering
	//glEnable(GL_LIGHTING);	// enable general lighting
	//glEnable(GL_LIGHT0);	// enable the first light.
}


int main(int argc, char **argv)
{
	/*glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Shapes");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(1,1,1,1);	
	InitializeMyStuff();

	glutMainLoop();*/

	Parse parse("100_PANA_0.ply");
	
	if(parse.Read())
	{
		cout << "Valid file" << endl;
	}
	else
	{
		cout << "Invalid file" << endl;
	}


	vector<Point> temp = parse.Get_points();

	for (int i = 0; i < temp.size(); i++)
	{
		cout << temp[i].X << " " << temp[i].Y << " " << temp[i].Z << endl;
	}

	int stop = 0;
	cout << "Press any key to continue..";
	cin >> stop;

	return 0;
}
