#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <map>
#include <memory>
#include "gltools.hpp"
#include "config.hpp"
using std::string;

class Component;
// typedef std::shared_ptr<Component> PCompo;
typedef Component* PCompo;
class Component{
    protected:
        vector<PCompo> childs;
        vector<Vector3> vertices;
        vector<Face> faces;
        Vector3 baseX, baseY, baseZ, pos;
        Color color;
        string id;
        PCompo parent;

        Animator aniAngle;
        double lastAngle;
        VecAnimator aniPos;
        Vector3 rotateNorm;

        static std::map<string, PCompo> idmap;
    public:
        static PCompo select(const string& id){
            // assert(idmap.find(id) != idmap.end());
            if(idmap.find(id) != idmap.end())
                return idmap[id];
            else
                return PCompo(NULL);
        }
        virtual void set_id(const string& id){
            if(this->id.size()){
                idmap.erase(this->id);
            }
            this->id = id;
            if(id.size()){
                idmap[id] = PCompo(this);
            }
        }
        Component():baseX(1, 0, 0), baseY(0, 1, 0), baseZ(0, 0, 1), pos(0, 0, 0){
        }
        void add_child(PCompo child){
            childs.push_back(child);
            child->parent = PCompo(this);
        }
        void set_color(Color color){
            this->color = color;
            TR(ci, childs){
                (*ci)->set_color(color);
            }
        }

        virtual void dump_xml(std::ostream& ost)const{
            ost << "<compo id='" << id << "'";
            if(childs.size()){
                ost << ">\n";
                TR(ci, childs){
                    (*ci)->dump_xml(ost);
                }
                ost << "</compo>\n";
            }else{
                ost << " />\n";
            }
        }

        virtual ~Component(){
            TR(ci, childs){
                delete *ci;
            }
        }

        void translate(const Vector3& d);
        void translate_smooth(const Vector3& d, double dt);
        void rotate(double angle, const Vector3& norm);
        void rotate_smooth(double angle, const Vector3& norm, double dt);
        virtual void draw_tree();
        virtual void draw_self();
        void update(double dt);
};


class Box:public Component{
protected:
    double xl, yl, zl;
public:
    Box(double xl, double yl, double zl):xl(xl),yl(yl),zl(zl){};
    void draw_self();
    void dump_xml(std::ostream& ost)const;
};
