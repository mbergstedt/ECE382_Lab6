ECE382_Lab6
===========
##PWM - "Robot Motion"
###Purpose
Gain experience using the pulse-width modulation features of the MSP430.
###Preliminary Design
My flowchart for the required functionality is:
![alt text](https://github.com/mbergstedt/ECE382_Lab6/blob/master/Flowchart.JPG?raw=true)

Some psuedocode I had for required functionality is:
![alt text](https://github.com/mbergstedt/ECE382_Lab6/blob/master/Psuedocode.JPG?raw=true)

When I tried to implement the ir sensor, I needed to go back to get some values from my remote using code from Lab 4, but I was never able to get that to work.  Therefore, I did not adjust my designs to account for the ir sensor.
###Hardware Design
My hardware design for required functionality is:
![alt text](https://github.com/mbergstedt/ECE382_Lab6/blob/master/Hardware%20Design.JPG?raw=true)

My actual hardware setup is:
![alt text](https://github.com/mbergstedt/ECE382_Lab6/blob/master/Hardware_Setup.jpg?raw=true)
###Debugging
I used the debugger to help understand how exactly the code affected the wheels spinning.  After putting writing my code, I would run the debugger and place the robot on a stand to verify that my code made the robot do what I wanted it to do.  For the turns, I used a value that gave me a turn that I could see in the wheels, and then unplugged the robot and tested it on the ground.  After a few repetitions, I found a delay value that gave me a roughly 90 degree turn, and I just divided that number by two for my half turns.
###Testing Methodology/Results
After using the debugger to check that my wheels would spin the direction I wanted, I would unplug my robot and verify that it worked when I placed it on the ground unattached from my computer.  For testing the turns, I put the robot on the ground and observed how far it went on the turn.  If it did not turn the way I wanted, I would go back into my code and adjust the values in the direction that I needed it to change.  Eventually I wrote code that ran it through the required functionality in a loop, pausing between each movement.  The code would cycle through moving forward, moving backward, completing a 90 degree turn in both directions, and then completing a 45 degree turn in both directions.  In moving backward, something would catch in the right wheel and it would not always spin correctly, causing the robot to go back with a slight angle sometimes.
###Observations/Conclusions
Once I started understanding how the code affected the spinning and how to set up with the motor driver chip to allow the code to be used to easily switch between going forward and going backward, the rest became fairly simple.  I used functions for each of the movements, and the code for each could be copied over with a few adjustments.  I tried to implement the robot with the ir sensor, but I was unable to get the signal from the remote working.  I needed to get values for my directional remote buttons, but when I ran the test code for finding them from lab 4, the values that resulted were completely different from when I did it in lab 4, and I was unable to correct the issue.
###Documentation
Dr Coulston helped explain how to do the setup to allow for the motor to go either forwards or backwards.
