# 2D-Satellite-Sim-Game
## Overview:
Sattelite Sim game is a game based around a 2 dimensional satellite simulator. The project has a dual purpose. It functions as a game and simulator. The game is meant to both provide a source of entertainment for players and also provide an environment to run a quick test of digital control systems. 
The purpose of the game is to use the satellite's thrusters to keep them in orbiting safely. Since these objects are subject to gravity the player has to use the thrusters to keep them in orbit. In order to do so the player must use thrusters to balance between crashing into the planet, completely escaping the planets pull, and spinning out of control. The code is based off of the enviro reposistory. More infomration can be found at: https://github.com/klavinslab/enviro

## How to install and run the code
(This assumes you Have installed docker)

Clone this repository into a local directory or readily accessible location. 

Using the terminal run: 
    docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.6 bash
        Note:where PWD is the local path to where the repo has been saved
        Note: this sets up a docker container with the necessary files to run the 2D game. 
        This should change your terminal to "root@....."

Open up a web browser and go to http://localhost. You should a message displayed that says "Enviro Error: No connection. Is the server running? See here for help"

Then in the terminal navigate to 2DGame with CD. 
Run:
    make
    enviro

Open up a web browser and go to http://localhost and you should see circle in the center of the screen with square satellites surrounding it. 

Press Ctrl-C in the terminal to stop enviro.

## How to run and/or use the project

Once the project is up and running the game has begun. If you do not see the project in your web browser go back to "How to install and run the code."

Make sure to click in the web browser if not some of the keys may not work. 

Upon opening the game the satellites will slowly land onto the planet because of gravity. You can use the WASD keys to control the direction of the satellites. 

Each of the satellites is controlled globally via the keys. 
    USe W to move further from the planet (up with respect to the satellite)
    Use S to move closer to the planet (up with respect to the satellite)
    Use A and D to move left and right

Remember that because of the physics involved moving left and right also increase your distance from the planet. 

The overall goal is to safely take off and slowly move toward sustaining a controlled orbit around the planet. Due to the limited friction it should require minimal thrust to keep the satellites moving around the planet. 

## Key Challenges

There were a number of challenges involved in the development of this project. 

One challenge is that enviro is based around rectangular coordinate while the environment that is being simulated is more readily controlled with polar coordinates. To address this the force of the thrusters applied with the WASD keys is transformed into components prior to beign applied. In addition gravity is added based on the position of the robot by utilizing the distance components from the planet. 

Another challenge is the way that forces need to be applied. In order to have the robots move based off of their own unique directions, it required that each be rotating themselves to account for their revolution around the planet. Since the omni_direction function does not also have rotations it was required to utilize a seperate apply force function inorder to rotate the satellites appropriately. This led to the discovery of the overaching challenge that the forces applied cannot be done one after the other. For example when running omni-direction then applying force they do not both affect the system. To adress this most forces are added into variables and placed into one function. Although this is a solution that solves the immediate problem it still leaves several other open. This  removes the solution of trackign velocity's or moving to direction of (0,0). To account for this each of the satellites is created already pointing at the origin and then it applies the omni_force. If at some point it is not pointing at the orgin then the torques will be applied on that run of the update function. Once the rotation has taken place and it is pointing at the origin the omni force function can then be applied again. The challenge with this solution is that the rotation needs to take place quickly and precisely if not gravity and the WASD keys may not be applied. This means that the player has the additional challenge of ensuring they do not add excessive angular velocity to the satellite. 

To rotate the satellites correctly it calls upon a function which returns the direction to the planet. In order to do so it takes in the x and y values and applies arc trig functions to get the appropriate theta value. The apply force is then used to move toward the correct theta. This creates two main challenges:
    1. Since the system relies on arc trig function their is gauranteed to be a point of discontinuity. The one selected was arc tangent which means that it return values from -pi to pi. However when the satellite crosses this specific threshold it quickly tries to reaccount for the large difference and rotates all the way around. One potential solution would have been to only allow for orbits to rotate in one direction and then go from 0 to 2pi to 4pi and so on... However to maintain the functionality of going in both directions the system checks to see if you are at the discontinuity. If so it manually applies  the force toward the desired by taking the current angle and using it to calculate the desired angle. 
    2. Since the system applies a force inorder to continue looking at the planet and is in a frictionless environment, that force makes it overshoot the desired angle. Inorder to mitigate this a control system is applied that applies the force followed by a reduced force in the opposite direction on the following update. This creates a tradeoff  due to the previosuly described challenge of not being able to apply a force torque and omni_force on the same update iteration. This means that the more time the control system takes to stabilize the longer the environment goes without having applied gravity. To handle this tradeoff a quick control sytem is used so that gravity is be applied, but there is stil some reduction in the angular velocity.  


# Acknowledgements
Thankyou to Professor Klavins along with TA for running the course. 

The Project is simply a specific usage of the Elma (https://github.com/klavinslab/elma) / enviro systems (https://github.com/klavinslab/enviro).

