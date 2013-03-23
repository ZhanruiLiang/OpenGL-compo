#include "control.hpp"
#include <sstream>

Event* Controller::parse_line(std::ifstream& ist){
    string type, targetID;
    double start, end; 
    string line;
    getline(ist, line);
    std::stringstream ssline;
    ssline << line;
    const char defaultMsg[] = "Argument number or type mismatch";
    if(ssline >> type){
        try{
            if(type == "#"){
            }else if(type == "r" or type == "rotate"){
                Vector3 norm;
                double angle;
                type = "rotate";
                ssline >> targetID >> start >> end >> angle >> norm.x >> norm.y >> norm.z;
                if(not ssline) throw ArgumentError(defaultMsg, lineNum, line);
                Event* event = new RotateEvent(start, end, targetID, angle, norm);
                return event;
            }else if(type == "t" or type == "translate"){
                Vector3 dpos;
                type = "translate";
                ssline >> targetID >> start >> end >> dpos.x >> dpos.y >> dpos.z;
                if(not ssline) throw ArgumentError(defaultMsg, lineNum, line);
                Event* event = new TranslateEvent(start, end, targetID, dpos);
                return event;
            }else if(type == "j" or type == "jump"){
                type = "jump";
                ssline >> start >> end;
                if(not ssline) throw ArgumentError(defaultMsg, lineNum, line);
                Event* event = new JumpEvent(this, start, end);
                return event;
            }
        }catch(const ParseError& error){
            std::cerr << error.what_s() << std::endl;
        }
    }
    return NULL;
}

void Controller::load_events(const string& path){
    TR(it, events){
        delete (*it);
    }
    events.clear();
    std::ifstream fin(path);
    lineNum = 0;
    while(fin){
        lineNum++;
        Event* event = parse_line(fin);
        if(event){
#ifdef DEBUG
            // std::cout << "parsed: " << event->str() << std::endl;
#endif
            events.push_back(event);
        }
    }
    std::cout << "loaded " << path << std::endl;
}

void Controller::update(double dt){
    double oldTime = time;
    time += dt;
    // printf("time: %.2lf\n", time);
    TR(ei, events){
        Event* event = *ei;
        if(event->start >= oldTime and event->start < time){
#ifdef DEBUG
            // std::cout << "apply: " << event->str() << " at " << time << std::endl;
#endif
            event->apply();
        }
    }
    body->update(dt);
}
void Controller::jump(double destTime){
    time = destTime;
}
