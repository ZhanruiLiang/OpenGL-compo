#include "compo.hpp"

void Component::draw_tree(){
    double baseM[] = {
        /*col1*/ baseX.x, baseX.y, baseX.z, 0,
        /*col2*/ baseY.x, baseY.y, baseY.z, 0,
        /*col3*/ baseZ.x, baseZ.y, baseZ.z, 0,
        /*col4*/ pos.x, pos.y, pos.z, 1
    };

    glPushMatrix();{
        // printf("<draw>\n");
        glMultMatrixd(baseM);
        draw_self();
        TR(it, childs){
            (*it)->draw_tree();
        }
    } glPopMatrix();
}

void Component::translate(const Vector3& d){
    pos += d;
}

void Component::rotate(double angle, const Vector3& norm){
    double M[16] = {
        /*col1*/ baseX.x, baseX.y, baseX.z, 0,
        /*col2*/ baseY.x, baseY.y, baseY.z, 0,
        /*col3*/ baseZ.x, baseZ.y, baseZ.z, 0,
        /*col4*/ 0, 0, 0, 1
    };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();{
        glLoadIdentity();
        glRotated(angle, norm.x, norm.y, norm.z);
        glMultMatrixd(M);
        glGetDoublev(GL_MODELVIEW_MATRIX, M);
    }glPopMatrix();
    baseX.x = M[0]; baseX.y = M[1]; baseX.z = M[2];
    baseY.x = M[4]; baseY.y = M[5]; baseY.z = M[6];
    baseZ.x = M[8]; baseZ.y = M[9]; baseZ.z = M[10];
    // pos.x = M[12]; pos.y = M[13]; pos.z = M[14];
}

void Component::translate_smooth(const Vector3& d, double dt){
    aniPos.set_val(pos, pos+d, dt);
}

void Component::rotate_smooth(double angle, const Vector3& norm, double dt){
    aniAngle.set_val(0, angle, dt);
    lastAngle = 0;
    rotateNorm = norm;
}

void Component::update(double dt){
    // printf("%s updated\n", id.c_str());
    if(aniPos.is_active()){
        aniPos.update(dt);
        pos = aniPos.get_val();
    }
    if(aniAngle.is_active()){
        aniAngle.update(dt);
        rotate(aniAngle.get_val() - lastAngle, rotateNorm);
        lastAngle = aniAngle.get_val();
    }
    TR(ci, childs){
        (*ci)->update(dt);
    }
}

void Component::draw_self(){
    glutWireCube(0.1);
    TR(fi, faces){
        glColor3f(fi->color.r, fi->color.g, fi->color.b);
        glBegin(GL_POLYGON);{
        // glBegin(GL_LINE_LOOP);{
            TR(vi, fi->vs){
                glVertex3d(vertices[*vi].x, vertices[*vi].y, vertices[*vi].z);
            }
        }glEnd();
    }
}

void Box::draw_self(){
    glPushMatrix();{
        // glutWireCube(0.2);
        glScaled(xl, yl, zl);
        glColor3f(color.r, color.g, color.b);
#ifdef SOLID
        glutSolidCube(1);
        glColor3f(0, 0, 0);
        glutWireCube(1);
#else
        glutWireCube(1);
#endif
    }glPopMatrix();
}

void Box::dump_xml(std::ostream& ost)const{
    ost << "<Box" 
        << " xl=" << xl
        << " yl=" << yl
        << " zl=" << zl
        << " />\n";
}

