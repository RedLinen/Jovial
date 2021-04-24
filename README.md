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

Plans to program this protype for more advanced timer functions had be called off after the upper half of the B-register of the designers PIC18 stopped working. Therefore, we scaled back to a simple sixteen-minute counter with the ten LEDs seen to the right of the PIC18 in the above picture serving as a binary display. The six LEDs closest to the PIC18
display seconds and the four LEDs farthest from the PIC18 display minutes. The clock can be reset at any time by pressing both buttons at the same time.

### The Inspirational Message Button:

The button seen farthest from the PIC18 in the above picture was originally meant to play inspirational messages when pressed. Unfortunately, the code necessary to connect the PIC18 to audio files was not available soon enough to implement this. As a stand in, pressing this button currently plays a—very digital—rendition of a couple bars from the song “When We Were Young”. Changing this to an inspiration message would be fairly straight forward after the processes of connecting the PIC18 to audio files was complete.

### The Nutrition Information Button:

The button seen farthest from the PIC18 was intended to play messages giving the user nutritional information when pressed. As with the other button, the resources to do this where not available soon enough. Currently, pressing it will play the closing bars of “When We Were You”—again, this is a very digital rendition.

##How to Build This Prototype:

