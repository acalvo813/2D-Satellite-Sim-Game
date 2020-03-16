#ifndef __SPACE_AGENT__H
#define __SPACE_AGENT__H 

#include "enviro.h"
#include <math.h>


using namespace enviro;
using namespace std;



class SpaceController : public Process, public AgentInterface {
// controls a generic space object. Specifically catered to satellites but could be inherited for other space type objects
    public:
    SpaceController() : Process(), AgentInterface(), LEFT(false), RIGHT(false), UP(false), DOWN(false){}
    

    double height() {
        //return the botom sensors distance to whatever is beneath it. If it is not pointing a the planet it is at a its maximum or 
        // it may be pointing a satellite beneath it
        return sensor_value(0);
    }
    
    double g(double pos){
        //takes in a double representing the position and returns the gravity to be applied in the update function


        //cannot apply forces on after another it seems
        //omni_apply_force(-(x()*x())*G,-(y()*y())*G);
        //apply_force(0,cos(x())*100);
       // move_toward(0, 0, 10, 100);
       //std::cout<<(0-G*10000)/(pos*abs(pos));

        double g;
        g=-G*pos*abs(pos);
        // g= (0-G)/(pos*abs(pos));
        // if ( abs(g)>2500 ){
        //     std::cout<<"went in"<<"\n";
        //     g=-G*pos*0.0000001;
        //     //std::cout<< g <<"\n for robot "<< id()<< "\n";
        // }
        

        //std::cout<<g<<"\n";
        return g;//(0-G*10000)/(pos*abs(pos));
    }


    void addinputkeys(){
        //adds the input keys  in the update function. Depending on the state set from the init keys the velocty is updated
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

    }
    void initinputkeys(){
        //initializes the keyboard events and assigns them to the appropriate steps then used in add keys through update
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

    void init() {
        // init function run at the initialization
        //allow oratition and set default private states
        allow_rotation();
        STABILIZING=false;
        STABLE=true;
        initinputkeys();
    }
    void start() {}

    void update() {
        // run frequently
        // adds the input keys ( allows wasd to be read in from the keyboard)
        // based on the wasd values it applies directional forces
        // applies gravitational forces
        // applies rotational forces to ensure the system is pointing in the right direction
        double fx=0;
        double fy=0;
        double m=0;
        
        double actualfx;
        double actualfy;
         notice_collisions_with("Planet", [&](Event &e) {
            //teleport(0,-200,0);
            //std::cout<<angle();
        }); 
        //apply_gravity();
       addinputkeys();
            
            
            fx = -K_X*(0-vx);
            fy = -K_X*(0-vy);
            
            actualfx=fx*cos(angle())-fy*sin(angle());
            actualfy=fx*sin(angle())+fy*cos(angle());
        //omni_apply_force(g(x()),g(y()));    

        //when keys randomly stop working.... uncomment below to debug
        //label("actual fx "+ to_string(actualfx)+ " actual fy "+to_string(actualfy),0,20);
        //label("x "+ to_string(x())+ " y "+to_string(y()),0,20);
        label("Distance to earth "+ to_string(height()),0,20);
        //omni_apply_force(actualfx,actualfy);

        omni_apply_force(actualfx+g(x()),actualfy+g(y()));

        //label("\nreal angle "+ to_string(realangle())+" built in angle to origin "+to_string(angleoforigin()),0,20);
        //label("ag-atan is "+ to_string(realangle()-angleoforigin())+ " pointgatorigin is "+to_string(pointingatorigin()),0,-20);
        //label("angular velocity "+to_string(angular_velocity())+ " pointgatorigin is "+to_string(pointingatorigin()),20,0);

        if (!pointingatorigin()){
                //double fr=VEL_R*atan2(x(),-y());
                allow_rotation();
                double fr=-VEL_R*(realangle()-angleoforigin());
                double mag=0.01;
                //fr=-K_X*(angular_velocity()+0.001);
            
                // need to apply for based off its current angular velocity = -K_X*(0-vy);
                
                apply_force(0,fr*100);
                
                // if (x()>0){
                //     apply_force(0,fr*100);
                // }else{
                //     apply_force(0,-fr*100);
                // }
            //label("angular velocity "+to_string(angular_velocity())+ "is fr "+to_string(fr),20,0);

            // if (abs(angular_velocity())>mag){ 
            //     if (angular_velocity()<0)
            //     {
            //         apply_force(0,-angular_velocity());
            //     }else{
            //         apply_force(0,-angular_velocity());
            //     }
                 
            // }else{
            //     apply_force(0,-VEL_R*(realangle()-atan2(x(),-y())));
            // }
                
            
        }
        else{
            //prevent_rotation();
                if (abs(angular_velocity())>0.01 ){
                
                STABILIZING=true;
               // clear_label();
                //label("trying to stabilize ln 157",0,0);
                if (vr>0){

                    apply_force(0,-angular_velocity()*vr);
                    
                    vr=-vr/10;
                }else{
                    STABILIZING=false;
                    vr=10000;
                }
                
            }

        }

           //stabilize attempt     
        if (angular_velocity()>0.05){
            // sta has gone out of control
      
        //     STABLE=false;
        //     prevent_rotation();
        //     clear_label();
        //     label("trying to stabilize ln 178",0,0);
        //         if (vr>0.1){
        //             apply_force(0,-angular_velocity()*vr*100000000000000);
        //             vr=vr/10;
        //         }else{
        //             vr=100000000;
        //             count++;
        //             if(angular_velocity()>0.001 && count>3 ){
        //                 allow_rotation();
        //                 count=0;
        //                 STABLE=true;
        //             }
        //         }
        
         }



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
    double vr=1000000;
    double oldav=0;
    int count=0;
    const double G = 0.098;//9.8*1000000;
    const double VEL_X = 20;
    const double VEL_Y = 20;
    const double VEL_R = 2000;
    const double K_X = 1000;
    bool LEFT, RIGHT, UP,DOWN,STABLE,STABILIZING;


    private:
    bool pointingatorigin(){
        // returns a boolean which determines wether the satellite is pointing at the origin with a given tolerance
        bool a;
            a= abs(realangle()-angleoforigin())<0.15;
        return a;
    }
    double realangle(){
        // take the input angle and transform it to be from 0 to 2pi instead of infinite
        return remainder(angle(),2*3.14);
    }
    double angleoforigin(){
        // calculates the angle of the origin using the x and y location along with the real angle

       double a= atan2(x(),-y());
       a=remainder(a,2*3.14);
      if (abs(realangle()-a)>2){ // if at arc tan - pi pi point
        a= realangle()+(0.01*realangle()); // if neg want more neg if pos want more pos
        //a=3.14+3.14-(realangle()-a);
      }
       
       return a;
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