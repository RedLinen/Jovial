# Jovial

## Operation Instructions and Some Notes:

The operation instructions included here are for prototype three of Jovial—the most advanced prototype that we were able to build. This prototype was still very early stage and lacked many of the capabilities that a more advanced prototype would have had. Instructions detailing how a more advanced version of this device might work are not included here because, in the opinion of this designer, the idea of how a product should be operated needs to move and shift as the design advances and it is possible to see, touch, carry, and drop increasingly sophisticated prototypes.

### Prototype Three:
![my link is broken?](Jovial.jpg)
### Power:
This prototype requires a power source between four and five volts with five volts being a little better. Three AA batteries in series are what was typically used during testing. However, a five-volt lithium-ion battery would also work, as would a five-volt wall supply. Regardless of the source used, one needs only to hook it to the power and ground rails of the breadboard to make the circuit operational.

### Starting the PIC18:

This protype needs to be reprogrammed by the PIC Kit 4 after every start up. The source code necessary for this can be found in this repository in the file “Jovial.X”.

### The Clock:

Plans to program this protype for more advanced timer functions had be called off after the upper half of the B-register of the designers PIC18 stopped working. Therefore, we scaled back to a simple sixteen-minute counter with the ten LEDs seen to the left of the PIC18 in the above picture serving as a binary display. The six LEDs closest to the PIC18
display seconds, and the four LEDs farthest from the PIC18 display minutes. The clock can be reset at any time by pressing both buttons at the same time.

### The Inspirational Message Button:

The button seen farthest from the PIC18 in the above picture was originally meant to play inspirational messages when pressed. Unfortunately, the code necessary to connect the PIC18 to audio files was not available soon enough to implement this. As a stand in, pressing this button currently plays a—very digital—rendition of a couple bars from the song “When We Were Young”. Changing this to an inspirational message would be fairly straight forward after the process of connecting the PIC18 to audio files was complete.

### The Nutrition Information Button:

The button seen closest to the PIC18 was intended to play messages giving the user nutritional information when pressed. As with the other button, the resources to do this where not available soon enough. Currently, pressing it will play the closing bars of “When We Were Young”—again, this is a very digital rendition.



# How to Create Jovial Prototype 3:

### Building It:

Below is an image of the circuit design. This schematic is only meant to represent the circuits of the Jovial protype themselves and is not meant to suggest layout.
![I hope my link doesn’t break…](Jovial(2).jpg)

To build the prototype simply create this combination of circuits, arranged according to personal preference, on one or more breadboards. When we created our copy of this protype we chose to arrange it as can be seen in the photograph at the top of the page. This was done to make handling it as close as possible to how it would be to handle a finished version of Jovial. However, this arrangement proved be a little impractical since it left so little room on the breadboard that it was hard to experiment with the design without pulling the whole thing apart.

### Programing It: 

The PIC18 that serves as the brain of the Jovial prototype is programed using MPLAB and a PIC Kit 4. The source code used to run this prototype can be found within this repository in the file “Jovial.X”. Specifically, most of the code related to the functionality of the protype is located in the function “UserAppRun()” which can be found by opening “Jovial.X” then opening “Source Files” and finally opening “user_app.c” and scrolling to the bottom.

