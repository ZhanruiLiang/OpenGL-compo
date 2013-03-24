#pragma once
#include "compo.hpp"

class Body:public Component{
public:
    Body();
};

class Head:public Component{
public:
    Head();
};

class LeftArm:public Component{
public:
    LeftArm();
};

class LeftLowerArm:public Component{
public:
    LeftLowerArm();
};

class RightArm:public Component{
public:
    RightArm();
};

class RightLowerArm:public Component{
public:
    RightLowerArm();
};

class LeftLeg:public Component{
public:
    LeftLeg();
};

class LeftLowerLeg:public Component{
public:
    LeftLowerLeg();
};

class LeftFoot:public Component{
public:
    LeftFoot();
};

class RightLeg:public Component{
public:
    RightLeg();
};

class RightLowerLeg:public Component{
public:
    RightLowerLeg();
};

class RightFoot:public Component{
public:
    RightFoot();
};

class Hand:public Component{
public:
    // side = -1 for left, 1 for right
    Hand(int side=-1);
    virtual void set_id(const string& id);
    /* 0 for thumb, 1 for index, etc.
     */
    PCompo get_finger(int idx)const{
        return childs[idx+1];
    }
};

class FingerSec:public Component{
public:
    FingerSec(double len);
    FingerSec* next_sec()const;
};

class Finger:public Component{
public:
    Finger(const vector<double>& lens);
    virtual void set_id(const string& id);
};

