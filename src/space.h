#ifndef __SPACE_AGENT__H
#define __SPACE_AGENT__H 

#include "enviro.h"
#include <math.h>


using namespace enviro;
using namespace std;



class SpaceController : public Process, public AgentInterface {

    public:
    SpaceController() : Process(), AgentInterface(), LEFT(false), RIGHT(false), UP(false), DOWN(false){}

    double height() {
        return sensor_value(0);
    }
    
    double g(double pos){
        //cannot apply forces on after another it seems
        //omni_apply_force(-(x()*x())*G,-(y()*y())*G);
        //apply_force(0,cos(x())*100);
       // move_toward(0, 0, 10, 100);
        return -pos*abs(pos)*G;
    }



    void init() {
        allow_rotation();
        watch("keydown", [&](Event& e) {
            std::string k = e.value()["key"];
            if ( k == "w" ) {
                UP = true;
            }else if ( k == "s" ) {
                DOWN = true;
            } else if ( k == "a" ) {
                LEFT = true;
            } else if ( k == "d" ) {
                RIGHT = true;
            } 
        });
        watch("keyup", [&](Event& e) {
            std::string k = e.value()["key"];
            if (k== "w"){
                UP= false;
            }else if ( k == "s" ) {
                DOWN = false;
            }else if( k == "a" ) {
                LEFT = false;
            } else if ( k == "d" ) {
                RIGHT = false;
            }
        });      
    }
    void start() {}

    void update() {
        double fx=0;
        double fy=0;
        
        double actualfx;
        double actualfy;
         notice_collisions_with("Planet", [&](Event &e) {
            //teleport(0,-200,0);
            //std::cout<<angle();
        }); 
        //apply_gravity();
       
        
        if ( RIGHT ) {
            vx = VEL_X;
        } if ( LEFT ) {
            vx = -VEL_X;
        } else if ( !RIGHT && !LEFT ) {
            vx = 0;
        }

        if ( UP ) {
            vy = -VEL_Y;
        }else if ( DOWN ) {
           vy = VEL_Y;
        } else if ( !UP && !DOWN ) {
            vy = 0;
        }else{

        }
            
            
            fx = -K_X*(0-vx);
            fy = -K_X*(0-vy);
            actualfx=fx*cos(angle())-fy*sin(angle());
            actualfy=fx*sin(angle())+fy*cos(angle());
        //omni_apply_force(g(x()),g(y()));    

        //when keys randomly stop working.... uncomment below to debug
        // label("actual fx "+ to_string(actualfx)+ " actual fy "+to_string(actualfy),0,20);
        // omni_apply_force(actualfx,actualfy);

        omni_apply_force(actualfx+g(x()),actualfy+g(y()));

        // label("ag-atan is "+ to_string(realangle()-atan2(x(),-y()))+ " pointgatorigin is "+to_string(pointingatorigin()),0,15);
        // label("angular velocity "+to_string(angular_velocity())+ " pointgatorigin is "+to_string(pointingatorigin()),20,0);

        // if (!pointingatorigin()){
        //         //double fr=VEL_R*atan2(x(),-y());
        //         double fr=-VEL_R*(realangle()-atan2(x(),-y()));
        //         double mag=0.001;
        //         //fr=-K_X*(angular_velocity()+0.001);
            
        //         // need to apply for based off its current angular velocity = -K_X*(0-vy);
        //         //apply_force(0,fr);
        //     if (abs(angular_velocity())>mag){ 
        //         if (angular_velocity()<0)
        //         {
        //             apply_force(0,-angular_velocity());
        //         }else{
        //             apply_force(0,-angular_velocity());
        //         }
                 
        //     }else{
        //         apply_force(0,-VEL_R*(realangle()-atan2(x(),-y())));
        //     }
            
        // }
        
        
        //apply_force(100,0);

        // if (x()!=0){
        // track_velocity(0,atan(y()/x()));
        // }
        // else{
        //    // track_velocity(0,);
        // }
        //omni_apply_force(actualfx,actualfy);
        //std::cout<<fx;

       // omni_move_toward(0,0,0.1);
        //omni_apply_force(actualfx,actualfy);
       // label("fx is" + to_string(actualfx)+" fy is "+ to_string(actualfy), 0, 20 );
        
    }

    void stop() {}

   
    double vx;
    double vy;
    const double G = 0.098;
    const double VEL_X = 20;
    const double VEL_Y = 20;
    const double VEL_R = 2000;
    const double K_X = 1500;
    bool LEFT, RIGHT, UP,DOWN;

    private:
    bool pointingatorigin(){
        bool a;
            a= abs(realangle()-atan2(x(),-y()))<0.2;
        return a;
    }
    double realangle(){
        return remainder(angle(),2*3.14);
    }

};

class Space : public Agent {
    public:
    Space(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    SpaceController c;
};

DECLARE_INTERFACE(Space)

#endif