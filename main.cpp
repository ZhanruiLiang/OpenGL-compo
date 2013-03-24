#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>
#include <cstdio>
#include "config.hpp"
#include "gltools.hpp"
#include "compo.hpp"
#include "body.hpp"
#include "control.hpp"

using namespace std;

const double PI = acos(-1);
int gWinW, gWinH;
int gWinID;
Color bgcolor(1, 1, 1);
string aniFileName;

PCompo body = NULL;
Camera* camera = NULL;
Controller* controller = NULL;

pthread_mutex_t mutexStage;

class Exit: public exception{
};

void reset_camera(){
    if(camera) delete camera;
    camera = new Camera(Vector3(
                INIT_CAMERA_POS[0],
                INIT_CAMERA_POS[1], 
                INIT_CAMERA_POS[2]));
    camera->look_at(Vector3(
                INIT_CAMERA_CENTER[0],
                INIT_CAMERA_CENTER[1],
                INIT_CAMERA_CENTER[2]), Vector3(0, 1, 0));
}

void reset_stage(){
    if(body) delete body;
    body = PCompo(new Body());
#ifdef DEBUG
    std::ofstream fout("body.xml");
    body->dump_xml(fout);
#endif
    // reset controller
    if(controller) delete controller;
    controller = new Controller(body);
    controller->load_events(aniFileName);
}


void handle_key(unsigned char key, int x, int y){
    // printf("key %c pressed\n", key);
    pthread_mutex_lock(&mutexStage);
        if(key == 'q'){
            pthread_mutex_unlock(&mutexStage);
            glutLeaveMainLoop();
        }else if(key == 'w'){
            camera->translate_smooth(Vector3(0, 0, -5), 30./FPS);
        }else if(key == 's'){
            camera->translate_smooth(Vector3(0, 0, 5), 30./FPS);
        }else if(key == 'z'){
            camera->translate_smooth(Vector3(-5, 0, 0), 30./FPS);
        }else if(key == 'x'){
            camera->translate_smooth(Vector3(5, 0, 0), 30./FPS);
        }else if(key == 'a'){
            camera->rotate_smooth(-45, Vector3(0, 1, 0), 30./FPS);
        }else if(key == 'd'){
            camera->rotate_smooth(45, Vector3(0, 1, 0), 30./FPS);
        }else if(key == 'r'){
            reset_stage();
        }
    pthread_mutex_unlock(&mutexStage);
}

void draw_axises(double r){
    glPushMatrix();
    glBegin(GL_LINES);{
        // x
        glColor3f(0, 0, 0);
        glVertex3d(-r, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3d(r, 0, 0);
        // y 
        glColor3f(0, 0, 0);
        glVertex3d(0, -r, 0);
        glColor3f(0, 1, 0);
        glVertex3d(0, r, 0);
        // z
        glColor3f(0, 0, 0);
        glVertex3d(0, 0, -r);
        glColor3f(0, 1, 0);
        glVertex3d(0, 0, r);
    }glEnd();
    glColor3f(0, 0, 0);
    printw(r, 0, 0, "x");
    printw(0, r, 0, "y");
    printw(0, 0, r, "z");
    glPopMatrix();
}

void display(){
    pthread_mutex_lock(&mutexStage);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera->put();

        // light
#ifdef SOLID
        glLightfv(GL_LIGHT0, GL_POSITION, LIGHT0_POS);
#endif

        draw_axises(5);

        body->draw_tree();
        glutSwapBuffers();
    pthread_mutex_unlock(&mutexStage);
}

void reshape(int w, int h){
    gWinW = w;
    gWinH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (double)w/h, 1, 500);
    // int R = 24;
    // glOrtho(-R, R, -R*h/w, R*h/w, 1, 100);
}

void update(){
    usleep(1e6/FPS/SPEED);
    pthread_mutex_lock(&mutexStage);
    // camera->rotate(1, Vector3(0, 1, 0));
        camera->update(1./FPS);
        controller->update(1./FPS);
        glutPostRedisplay();
    pthread_mutex_unlock(&mutexStage);
}

void init(){
    glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_LINE_SMOOTH);
    pthread_mutex_init(&mutexStage, NULL);

    reset_stage();
    reset_camera();
}

int main(int argc, char**argv){
    if(argc > 1){
        aniFileName = argv[1];
    }else{
        aniFileName = DEFAULT_ANI_FILE;
    }
    /*
     * Init glut
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    gWinW = 640;
    gWinH = 360;
    glutInitWindowSize(gWinW, gWinH);
    glutInitWindowPosition(20, 20);
    /*
     * create window
     */
    int gWinID = glutCreateWindow("GL - Assignment 3");
    // bind callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handle_key);
    glutIdleFunc(update);
    // init and start
    init();
    glutMainLoop();
}
