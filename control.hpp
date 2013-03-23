#pragma once
#include <fstream>
#include <sstream>
#include "compo.hpp"
#include <exception>

class ParseError:public std::exception{
public:
    virtual string what_s()const=0;
};

class ArgumentError:public ParseError{
    int lineNum;
    string line;
    string msg;
public:
    ArgumentError(const string& msg, int lineNum, const string& line):
        msg(msg),line(line),lineNum(lineNum){};
    string what_s()const{
        std::stringstream ss;
        ss << 
            "ParseError:\n"
            "  " << msg << "\n"
            "  at line " << lineNum << ":\n"
            "    " << line << '\n';
        return ss.str().c_str();
    }
};

class TargetError:public ParseError{
    string targetID;
public:
    TargetError(const string& targetID):targetID(targetID){}
    string what_s()const{
        std::stringstream ss;
        ss << 
            "TargetError:\n"
            "  Target '" << targetID << "' not found.\n";
        return ss.str().c_str();
    }
};

class Event{
    protected:
        PCompo target;
    public:
        string type;
        double start, end; // start and end time
        string targetID;
        Event(string type, double start, double end, string targetID):
            type(type),
            start(start),
            end(end),
            targetID(targetID), target(NULL){
            if(targetID.size()){
                if(not get_target()){
                    throw TargetError(targetID);
                }
            }
        }
        PCompo get_target(){
            if(not target){
                target = Component::select(targetID);
            }
            return target;
        }
        virtual void apply()=0;
        virtual ~Event(){}
        virtual string str()const{
            std::stringstream ss;
            ss << "Event(" << type << ", s=" << start << ", t=" << end 
                << ", targetID=" << targetID << ")";
            return ss.str();
        }
};

class RotateEvent: public Event{
    public:
        double angle;
        Vector3 norm;
        RotateEvent(double start, double end, string targetID, 
                double angle, const Vector3& norm):
            angle(angle),norm(norm),Event("rotate", start, end, targetID){ }
        void apply(){
            get_target()->rotate_smooth(angle, norm, end - start);
        }
};

class TranslateEvent: public Event{
    public:
        Vector3 dpos;
        TranslateEvent(double start, double end, string targetID, const Vector3& dpos):
            dpos(dpos), Event("translate", start, end, targetID){ }
        void apply(){
            get_target()->translate_smooth(dpos, end - start);
        }
};


class Controller{
    protected:
        PCompo body;
        double time;
        vector<Event*> events;
        int lineNum;

        Event* parse_line(std::ifstream& ist);
    public:
        Controller(PCompo body):body(body), time(0){
        }
        void load_events(const string& path);
        void update(double dt);
        void jump(double destTime);
};

class JumpEvent: public Event{
    protected:
        Controller* controller;
    public:
        JumpEvent(Controller* controller, double start, double end):
            controller(controller), Event("jump", start, end, ""){}
        void apply(){
            controller->jump(end);
        }
};
