OpenGL Component System
=======================
Author: Ray.Liang 
Email: ray040123@gmail.com

Features
--------

* Object-Oriented interface to build up components.
* Reload animation file in runtime. Press 'r' to reload.
* Configure colors in '.ani' file.
* Implemented Camera class, support rotate and translate.
* Smooth transformation.

Build
-----
Invoke 'make' in the source directory.

How to use
----------
Run:

    ./main example.ani

If you have write other '.ani' file, say, custom.ani, then run:

    ./main custom.ani 

Use the following keys to control:

* w/s: Move camera forward/backward.
* a/d: rotate camera along the y axis.
* z/x: Move camera left/right.
* r: Reload '.ani' file.
* q: Quit.
