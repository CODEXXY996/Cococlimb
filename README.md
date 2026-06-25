🌴 CocoClimb – Semi-Automated Coconut Tree Climbing and Harvesting Robot

<img width="720" height="1280" alt="WhatsApp Image 2026-06-24 at 10 00 08 PM" src="https://github.com/user-attachments/assets/1f98445c-33b5-4e1e-9283-81648bfa555b" />

This repository contains the project files, design approach, and implementation flow for CocoClimb, a semi-automated coconut tree climbing and harvesting robot developed to reduce the difficulty, risk, and labor involved in manual coconut harvesting. The system was designed to climb coconut trees using a gravity-based self-clamping wheel mechanism and assist harvesting through a robotic arm with a cutting motor, making the process safer and more practical for farmers and plantation workers.

📂 Files and Components
* wheel_control
 . ESP8266-based webpage control for the tree climbing / wheel movement section
 . Handles forward, backward, stop, and motion control of the climbing mechanism
* robotic_arm_control
  . ESP8266-based webpage control for the robotic arm and cutting section
  . Used to operate arm movement and control the cutting blade mechanism
*  prototype images / hardware build
  .Includes the physical prototype of CocoClimb with the PVC frame, wheel section, and arm assembly.

  🛠️ Technologies Used
  
*   ESP8266 – wireless webpage-based control of the robot
*   12V Johnson geared DC motors – wheel drive for climbing
*  Servo motors – robotic arm joint movement
*  BLDC motor + ESC – coconut cutting mechanism
*  PVC structural frame – lightweight body for the prototype
*  Embedded web interface – remote operation of both climbing and harvesting modules.

🥥 Project Overview

CocoClimb was developed to solve a practical agricultural problem: manual coconut harvesting is risky, physically demanding, and highly dependent on skilled labor. Farmers often need to climb tall trees or hire trained climbers, which increases both cost and danger. Our goal was to create a semi-automated climbing and harvesting robot that can reduce manual effort while making coconut harvesting safer and more accessible.
Instead of building a fully autonomous machine, we focused on a remote-operated prototype that combines a tree climbing mechanism with a robotic cutting arm. The project was built as a proof-of-concept system to show how climbing, positioning, and harvesting can be integrated into one platform.

⚙️ How CocoClimb Works
CocoClimb is divided into two main functional sections:

1. Wheel / Climbing Section
   This section is responsible for helping the robot move along the tree trunk.
  The design uses a gravity-based self-clamping mechanism, where the structure grips the tree by      using its own weight and frame geometry to maintain contact between the wheels and the trunk.
   Use of this section:
           * Helps the robot hold onto the tree trunk
           * Supports controlled climbing movement
           * Reduces the need for a person to climb the tree manually.
2. Robotic Arm / Harvesting Section
   This section is responsible for positioning and cutting coconuts once the robot reaches the         required level. It uses a servo-based robotic arm for directional movement and a BLDC cutting       motor for harvesting.
   Use of this section:
           * Positions the cutting mechanism near the coconut bunch
           *  Allows controlled arm movement for better reach
           * Performs cutting using the blade motor.

💡 Why We Built CocoClimb 
     We chose CocoClimb because coconut harvesting still depends heavily on manual tree climbing,       which is dangerous, tiring, and increasingly difficult due      to   the shortage of skilled climbers.     Our aim was to create a safer and more practical alternative that could reduce physical risk and    support             farmers with a   low-cost semi-automated harvesting solution.
     
👥 Target Users / Customer Base
  CocoClimb is mainly intended for:
  * coconut farmers
  * plantation workers
  * small and medium farm owners
  * agricultural equipment innovators
  * rural farming communities looking for safer harvesting solutions.
    
🚀 Project Features:
  *  Semi-automated coconut tree climbing concept
  *  Gravity-based self-clamping wheel mechanism
  *  Wireless webpage control using ESP8266
  * Separate climbing and harvesting control modules
  *  Robotic arm for cutting-position adjustment
  *  BLDC motor-based cutting mechanism
  *  Prototype aimed at reducing harvesting risk and manual effort.

     📌 Prototype Outcome
     The CocoClimb prototype successfully demonstrated the core concept of a semi-automated tree climbing and harvesting system. The project showed how a robotic       platform could grip a coconut tree, be controlled wirelessly, and use a robotic arm for harvesting assistance. It served as a working proof of concept for         safer and more accessible coconut harvesting, while also highlighting areas for future improvement such as grip strength, climbing stability, and cutting          efficiency.

👨‍💻Team / Project Context
Developed as a student engineering project focused on applying embedded systems, robotics, motor control, and mechanical design to a real agricultural problem.
<img width="1560" height="1040" alt="WhatsApp Image 2026-06-24 at 9 59 21 PM" src="https://github.com/user-attachments/assets/65d89d20-49da-4e3a-82ff-d1e0d60730e2" />
Team members: Abhinav Nair, VL Padmanabam, Aswathy S, and Aparna Anand.

