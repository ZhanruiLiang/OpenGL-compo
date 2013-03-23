#pragma once
#include<vector>
#include<GL/glut.h>
#include<string>
#include<stdarg.h>
#include<cmath>
// #define TR(it, v) for(typeof((v).begin()) it=(v).begin(); it != (v).end(); ++it)
#define TR(it, v) for(auto it=(v).begin(); it != (v).end(); ++it)
using std::vector;

struct Vector3{
    double x, y, z;
    Vector3(double x=0, double y=0, double z=0):x(x), y(y), z(z){}
    Vector3 operator*(double k)const{
        return Vector3(k*x, k*y, k*z);
    }
    Vector3 operator+(const Vector3& v)const{
        return Vector3(v.x + x, v.y + y, v.z + z);
    }
    Vector3 & operator+=(const Vector3& v){
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector3 operator-(const Vector3& v)const{
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3 & operator-=(const Vector3& v){
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    //TODO: cross and dot
};

// Vector3 operator*(double k, const Vector3& v){
//     return Vector3(k*v.x, k*v.y, k*v.z);
// }

struct Color{
    double r, g, b, a;
    Color(double r, double g, double b, double a=1):r(r), g(g),b(b),a(a){}
    Color():r(0),g(0),b(0),a(1){}
};

struct Face{
    vector<int>  vs;
    Color color;
    void add(int i){
        vs.push_back(i);
    }
};

class Animator{
    protected:
        int state;
        double cur, orig, dest, time, totalTime;
    public:
        Animator():state(0){
        }
        void set_val(double v0, double v1, double t){
            orig = v0;
            dest = v1;
            totalTime = t;
            cur = v0;
            time = 0;
            state = 1;
        }
        bool is_active()const{
            return state > 0;
        }
        double get_val()const{
            return cur;
        }
        void update(double dt){
            // lineaer update
            time += dt;
            if(time > totalTime){
                time = totalTime;
                if(state > 0){
                    state++;
                    if(state == 3) state = 0;
                }
            }
            // cur = orig + (dest - orig) * time / totalTime;
            cur = orig + (dest - orig) * pow(time / totalTime, 0.5);
        }
};

class VecAnimator{
    protected:
        Animator a[3];
    public:
        void set_val(Vector3 v0, Vector3 v1, double t){
            a[0].set_val(v0.x, v1.x, t);
            a[1].set_val(v0.y, v1.y, t);
            a[2].set_val(v0.z, v1.z, t);
        }
        Vector3 get_val()const{
            return Vector3(a[0].get_val(), a[1].get_val(), a[2].get_val());
        }
        void update(double dt){
            a[0].update(dt);
            a[1].update(dt);
            a[2].update(dt);
        }
        bool is_active()const{
            return a[0].is_active() and a[1].is_active() and a[2].is_active();
        }
};

class Camera{
    private:
        VecAnimator aniPos;
        Animator aniAngle;
        double lastAngle;
        Vector3 rotateNorm;
    protected:
        double mat[16];
        double scale;
    public:
        Camera(Vector3 pos){
            mat[0] = mat[5] = mat[10] = mat[15] = 1;
            mat[12] = pos.x; mat[13] = pos.y; mat[14] = pos.z;
        }
        Vector3 get_pos()const{
            return Vector3(mat[12]/mat[15], mat[13]/mat[15], mat[14]/mat[15]);
        }
        void set_pos(Vector3 pos){
            mat[12] = pos.x * mat[15];
            mat[13] = pos.y * mat[15];
            mat[14] = pos.z * mat[15];
        }
        void look_at(Vector3 center, Vector3 up);
        void translate(Vector3 d);
        void translate_smooth(Vector3 d, double dt);
        void rotate(double angle, Vector3 n);
        void rotate_smooth(double angle, Vector3 n, double dt);
        void copy(){
            glGetDoublev(GL_MODELVIEW_MATRIX, mat);
        }
        void put(){
            glLoadMatrixd(mat);
        }
        void update(double dt);
};

void printw(float x, float y, float z, const char* format, ...);
