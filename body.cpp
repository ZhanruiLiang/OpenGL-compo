#include "body.hpp"
#include "config.hpp"

/* hierarchy
 *
 * - Body:
 *   - Head:
 *      - Eyes
 *   - LeftArm:
 *      - LeftLowerArm:
 *         - LeftHand
 *   - RightArm:
 *      - RightLowerArm:
 *         - RightHand
 *
 *   - LeftLeg:
 *      - LeftLowerLeg:
 *         - LeftFoot
 *
 *   - RightLeg:
 *      - RightLowerLeg:
 *         - RightFoot
 */
std::map<string, PCompo> Component::idmap;

Body::Body(){
    translate(Vector3(0, 3, 0));
    set_id("body");

    add_child(PCompo(new Box(BODY_WIDTH, BODY_HEIGHT, BODY_THICK)));

    add_child(PCompo(new Head()));
    add_child(PCompo(new LeftArm()));
    add_child(PCompo(new RightArm()));
    add_child(PCompo(new LeftLeg()));
    add_child(PCompo(new RightLeg()));
}

Head::Head(){
    set_id("head");
    translate(Vector3(0, BODY_HEIGHT/2 + HEAD_HEIGHT/2, 0));

    add_child(PCompo(new Box(HEAD_WIDTH, HEAD_HEIGHT, HEAD_THICK)));
}

LeftArm::LeftArm(){
    set_id("left-arm");
    rotate(-90, Vector3(0, 0, 1));
    translate(Vector3(BODY_WIDTH/2, BODY_HEIGHT/2, 0));

    // self box
    PCompo box(new Box(ARM_RADIUS*2, ARM_LENGTH, ARM_RADIUS*2));
    box->translate(Vector3(0, ARM_LENGTH/2, 0));
    add_child(box);
    // lower arm
    PCompo lowerArm(new LeftLowerArm());
    lowerArm->translate(Vector3(0, ARM_LENGTH, 0));
    add_child(lowerArm);
}

LeftLowerArm::LeftLowerArm(){
    set_id("left-lower-arm");
    PCompo box(new Box(LOWER_ARM_RADIUS*2, LOWER_ARM_LENGTH, LOWER_ARM_RADIUS*2));
    box->translate(Vector3(0, LOWER_ARM_LENGTH/2, 0));
    add_child(box);
    // hand
    PCompo hand(new Hand(-1));
    hand->rotate(90, Vector3(0, 1, 0));
    hand->translate(Vector3(0, LOWER_ARM_LENGTH, 0));
    add_child(hand);
}

RightArm::RightArm(){
    set_id("right-arm");
    rotate(90, Vector3(0, 0, 1));
    translate(Vector3(-BODY_WIDTH/2, BODY_HEIGHT/2, 0));

    // self box
    PCompo box(new Box(ARM_RADIUS*2, ARM_LENGTH, ARM_RADIUS*2));
    box->translate(Vector3(0, ARM_LENGTH/2, 0));
    add_child(box);
    // lower arm
    PCompo lowerArm(new RightLowerArm());
    lowerArm->translate(Vector3(0, ARM_LENGTH, 0));
    add_child(lowerArm);
}

RightLowerArm::RightLowerArm(){
    set_id("right-lower-arm");
    PCompo box(new Box(LOWER_ARM_RADIUS*2, LOWER_ARM_LENGTH, LOWER_ARM_RADIUS*2));
    box->translate(Vector3(0, LOWER_ARM_LENGTH/2, 0));
    add_child(box);
    // hand
    PCompo hand(new Hand(1));
    hand->rotate(-90, Vector3(0, 1, 0));
    hand->translate(Vector3(0, LOWER_ARM_LENGTH, 0));
    add_child(hand);
}

LeftLeg::LeftLeg(){
    set_id("left-leg");
    rotate(180, Vector3(0, 0, 1));
    translate(Vector3(BODY_WIDTH/2-LEG_RADIUS, -BODY_HEIGHT/2, LEG_RADIUS/4));
    // self box
    PCompo box(new Box(LEG_RADIUS*2, LEG_LENGTH, LEG_RADIUS*2));
    box->translate(Vector3(0, LEG_LENGTH/2, 0));
    add_child(box);
    // lower leg
    PCompo lowerLeg(new LeftLowerLeg());
    lowerLeg->translate(Vector3(0, LEG_LENGTH, 0));
    add_child(lowerLeg);
}

LeftLowerLeg::LeftLowerLeg(){
    set_id("left-lower-leg");
    PCompo box(new Box(LOWER_LEG_RADIUS*2, LOWER_LEG_LENGTH, LOWER_LEG_RADIUS*2));
    box->translate(Vector3(0, LOWER_LEG_LENGTH/2, 0));
    add_child(box);
    // foot
    PCompo foot(new LeftFoot());
    foot->translate(Vector3(0, LOWER_LEG_LENGTH, 0));
    add_child(foot);
}
LeftFoot::LeftFoot(){
    set_id("left-foot");
    rotate(180, Vector3(0, 0, 1));
    PCompo box(new Box(FOOT_WIDTH, FOOT_HEIGHT, FOOT_LENGTH));
    box->translate(Vector3(0, -FOOT_HEIGHT/2, FOOT_LENGTH*.25));
    add_child(box);
}

RightLeg::RightLeg(){
    set_id("right-leg");
    rotate(180, Vector3(0, 0, 1));
    translate(Vector3(-BODY_WIDTH/2+LEG_RADIUS, -BODY_HEIGHT/2, LEG_RADIUS/4));
    // self box
    PCompo box(new Box(LEG_RADIUS*2, LEG_LENGTH, LEG_RADIUS*2));
    box->translate(Vector3(0, LEG_LENGTH/2, 0));
    add_child(box);
    // lower leg
    PCompo lowerLeg(new RightLowerLeg());
    lowerLeg->translate(Vector3(0, LEG_LENGTH, 0));
    add_child(lowerLeg);
}

RightLowerLeg::RightLowerLeg(){
    set_id("right-lower-leg");
    PCompo box(new Box(LOWER_LEG_RADIUS*2, LOWER_LEG_LENGTH, LOWER_LEG_RADIUS*2));
    box->translate(Vector3(0, LOWER_LEG_LENGTH/2, 0));
    add_child(box);
    // foot
    PCompo foot(new RightFoot());
    foot->translate(Vector3(0, LOWER_LEG_LENGTH, 0));
    add_child(foot);
}

RightFoot::RightFoot(){
    set_id("right-foot");
    rotate(180, Vector3(0, 0, 1));
    PCompo box(new Box(FOOT_WIDTH, FOOT_HEIGHT, FOOT_LENGTH));
    box->translate(Vector3(0, -FOOT_HEIGHT/2, FOOT_LENGTH*.25));
    add_child(box);
}

Hand::Hand(int side){
    PCompo plam(new Box(HAND_WIDTH, HAND_LENGTH, HAND_THICK));
    plam->translate(Vector3(0, HAND_LENGTH/2, 0));
    add_child(plam);
    // thumb
    vector<double> thumbLens = {THUMB_LENGTH * .55, THUMB_LENGTH * .45};
    PCompo thumb(new Finger(thumbLens));
    thumb->translate(Vector3(side*(HAND_WIDTH/2+FINGER_RADIUS), HAND_LENGTH/2, 0));
    // thumb->set_color(Color(0, 0.5, 0));
    add_child(thumb);
    // fingers
    vector<double> fingerLens = {
        FINGER_LENGTH * .5, FINGER_LENGTH * .25, FINGER_LENGTH * .25};
    double sep = (HAND_WIDTH - (N_FINGERS - 1)*FINGER_RADIUS*2)/(N_FINGERS-2);
    for(int iFinger = 0; iFinger < N_FINGERS-1; iFinger++){
        PCompo finger(new Finger(fingerLens));
        finger->translate(Vector3(
                side*(HAND_WIDTH/2-FINGER_RADIUS-iFinger*(sep + 2*FINGER_RADIUS)), 
                HAND_LENGTH,
                HAND_THICK/2-FINGER_RADIUS));
        add_child(finger);
    }
    set_id((side == -1?"left-hand":"right-hand"));
}

void Hand::set_id(const string& id){
    Component::set_id(id);
    if(id.size()){
        char buf[20+id.size()];
        for(int iFinger = 0; iFinger < N_FINGERS; iFinger++){
            sprintf(buf, "%s-finger-%d", id.c_str(), iFinger);
            get_finger(iFinger)->set_id(buf);
        }
    }
}

Finger::Finger(const vector<double>& lens){
    double len = 0;
    PCompo prev(this);
    TR(li, lens){
        PCompo sec(new FingerSec(*li));
        sec->rotate(20, Vector3(1, 0, 0));
        sec->translate(Vector3(0, len, 0));
        prev->add_child(sec);
        len = *li;
        prev = sec;
    }
}

void Finger::set_id(const string& id){
    Component::set_id(id);
    if(id.size()){
        char buf[10 + id.size()];
        FingerSec* sec = (FingerSec*)childs[0];
        for(int iSec = 0; sec; iSec++){
            sprintf(buf, "%s-sec-%d", id.c_str(), iSec);
            sec->set_id(buf);
            sec = ((FingerSec*)sec)->next_sec();
        }
    }
}

FingerSec::FingerSec(double len){
    PCompo box(new Box(FINGER_RADIUS*2, len, FINGER_RADIUS*2));
    box->translate(Vector3(0, len/2, 0));
    add_child(box);
}

FingerSec* FingerSec::next_sec()const{
    if(childs.size()>1){
        return (FingerSec*)childs[1];
    }
    return NULL;
}

