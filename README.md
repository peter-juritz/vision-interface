# Requirements
OpenCV
SDL
v4l

#### What
![Drawing](https://raw.githubusercontent.com/peter-juritz/vision-interface/master/images/drawing.png)

This is a project I wrote while in university to allow a user with a head mounted webcam (facing back at the monitor) to control the computer using their fingers as pointers, Furthermore – the 3d relationship to the users head and the monitor is computed – although not really used in this demo.
This was done as some research and therefore it is very much _academic code_, and messy as hell.

# Algorithms

#### Monitor detection

![Segmentation](https://raw.githubusercontent.com/peter-juritz/vision-interface/master/images/hand1.png)
* **Edge detection and Luminosity thresholding** – build up the edge points which satisfy some brightness diference thresholds (as we really are only looking for the edges on the border of the monitor and bezel)
* **Hough transform** – finds all lines from our marked set of edge points
* **Quadrangle detection** – Detects all the sets of four (or three if some are offscreen) lines which form a reasonably shaped quadrangle (or rect).
* **Quad Validation** – these candidate quads are then ranked according to some heuristics on whether they could form the correct monitor config – including running lines around the bezel too look for a specific bezel colour and skewness.
![Multiple detections](https://raw.githubusercontent.com/peter-juritz/vision-interface/master/images/finger2.png)

#### Hand & Fingertip detection
* **Hand detection** - Once the monitor was detected, some checks would be run along the edge of the monitor to look for an occluding hand (or forearm usually). This was done by looking for a contrasting tone (different to the monitor bezel). Te median of this segment is then flood filled to try discover the full area of the hand. This results in a countor of the hand area.

![Fingertips](https://raw.githubusercontent.com/peter-juritz/vision-interface/master/images/finger.png)
* **Finger tip detection** – Curvature analysis is then done on the hand countour to find the probable finger tip points.
* **Homography mapping** – Since the geometry of the monitor is known as well as its relationship to the viewer, the homography mapping the perceived fingertip point location to the actualy location on the screen can be computed – this allows use to place a pointer which maps with where the user perceives their fingertip to actually be on the monitor.
* **Drawing** – Drawing can then be done by using the thumb as a “click” - when the thumb is present the user can draw and to stop drawing the thumb 

#### Performance 
A huge amount of work went in to making the system perform in real time and achieve all the necessary calculations for a given frame in <= 20ms.
![Performance](https://raw.githubusercontent.com/peter-juritz/vision-interface/master/images/perf.png)

