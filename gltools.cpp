#include "gltools.hpp"
#include <cstdio>

GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;

void Camera::look_at(Vector3 center, Vector3 up){
    Vector3 eye = get_pos();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();{
        glLoadIdentity();
        gluLookAt(eye.x, eye.y, eye.z, 
                center.x, center.y, center.z,
                up.x, up.y, up.z);
        glGetDoublev(GL_MODELVIEW_MATRIX, mat);
    }glPopMatrix();
}

void Camera::translate(Vector3 d){
    set_pos(get_pos() - d * mat[15]);
}

void Camera::translate_smooth(Vector3 d, double dt){
    aniPos.set_val(get_pos(), get_pos()-d, dt);
}

void Camera::rotate(double angle, Vector3 n){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();{
        glLoadMatrixd(mat);
        glRotated(-angle, n.x, n.y, n.z);
        glGetDoublev(GL_MODELVIEW_MATRIX, mat);
    }glPopMatrix();
}
void Camera::rotate_smooth(double angle, Vector3 n, double dt){
    aniAngle.set_val(0, angle, dt);
    lastAngle = 0;
    rotateNorm = n;
}

void Camera::update(double dt){
    aniPos.update(dt);
    aniAngle.update(dt);
    if(aniPos.is_active()){
        set_pos(aniPos.get_val());
    }
    if(aniAngle.is_active()){
        rotate(aniAngle.get_val() - lastAngle, rotateNorm);
        lastAngle = aniAngle.get_val();
    }
}

int _vscprintf (const char * format, va_list pargs) { 
    int retval; 
    va_list argcopy; 
    va_copy(argcopy, pargs); 
    retval = vsnprintf(NULL, 0, format, pargs); 
    va_end(argcopy); 
    return retval; 
}
//-------------------------------------------------------------------------
//  Draws a string at the specified coordinates.
//-------------------------------------------------------------------------
void printw(float x, float y, float z, const char* format, ...) {
    va_list args;	//  Variable argument list
    int len;		//	String length
    int i;			//  Iterator
    char * text;	//	Text

    //  Initialize a variable argument list
    va_start(args, format);

    //  Return the number of characters in the string referenced the list of arguments.
    //  _vscprintf doesn't count terminating '\0' (that's why +1)
    len = _vscprintf(format, args) + 1; 

    //  Allocate memory for a string of the specified size
    text = new char[len];

    //  Write formatted output using a pointer to the list of arguments
    vsnprintf(text, len, format, args);

    //  End using variable argument list 
    va_end(args);

    //  Specify the raster position for pixel operations.
    glRasterPos3f (x, y, z);

    //  Draw the characters one by one
    for (i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font_style, text[i]);

    //  Free the allocated memory for the string
    delete text;
}
