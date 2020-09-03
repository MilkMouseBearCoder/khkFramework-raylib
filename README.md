<img align="center" src="https://github.com/kenhyokun/khkFramework-raylib/blob/master/khkFramework-raylib-logo.png" width=512>

Experimental C/C++ 2D game framework with **[raylib][1]** inspired by popular game engine like **[Ogre3D][2]**, **[libGDX][3]** and Unity3D.
</br>
**Note: Build and tested on Ubuntu 18.04. No cross platform makefile for now.**

# Features:
  - Load exported **.tmx** file from **[Tiled map editor](https://www.mapeditor.org/)** for tile mapping. (only orthogonal view for now)
  - box2d physics engine. (only box, circle, capsule, polygon, and edge collision for now)

# 3rd party library:
  - raylib v3.0.0 (https://github.com/raysan5/raylib)
  - box2d v2.4.0 (https://github.com/erincatto/box2d)
  - tinyxml2 (https://github.com/leethomason/tinyxml2)

# Todo for version 1.0 release:
  - Add sample project.
  - Add basic collision shape detection (rectangle, circle, point).
  - Add capsule collider, edge collider and contact callback function with box2d.
  - Node need to optimised, especially when removing child node.
  - Beta testing

#
<p>
Kevin Haryo Kuncoro </br>
kevinhyokun91@gmail.com </br>
2020 
</p>

[1]: https://github.com/raysan5/raylib
[2]: https://github.com/OGRECave/ogre
[3]: https://github.com/libgdx/libgdx