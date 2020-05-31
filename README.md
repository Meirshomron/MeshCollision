# Mesh_Collision
3D collision detection, axis-aligned bounding boxes (AABB) 

The main runs in the MeshCollision/main.cpp, we added the function checkCollision() that will find if exists,
a collision between the shapes and will draw the smallest bounding boxes that collide.

In order to detect a collision we used a kd-tree, built from the vertices.
Each node stores in addition to the vertices also the center, width, height and depth of the bounding box for the node, calculated from the sub-tree.

In addition to the drawing of the smallest bounding boxes in a collision, 
every shape holds it's mesh and a cube bounding mesh that is constructed from the root of the kdtree for that shape. Every frame we'll draw both meshes.


## Project structure:

### engine
*This is the under-the-hood part that enables rendering meshes.*
- mesh.cpp
  - *Mesh represention via openGL.*
- shader.cpp
  - *Shader manager.*
- obj_lodaer.cpp
  - *.obj File parser.*
  - *Fast mesh simplification.*
- Scene.cpp 
  - *Scene represention.*
- CollisionDetection.h
  - *Collision Detection algorithm calculations.*
- KDTree.cpp
  - *Adjusted kd-tree for collision detection.*
  
### MeshCollision
*The actual MeshCollision implementation.*
- main.cpp
  - *Entry point.*


##  Usage:
### Keyboards controls:

**Use arrows to control the movable shape around.**

**Use W,A,S,D to rotate the camera.**

**Q** - Exit the program.

**'Z'** - Move the camera left.

**'X'** - Move the camera right.

**'C'** - Zoom into the scene.

**'V'** - Zoom out of the scene.

## Future Possible Upgrades
- Use other structures for collision detection such as a BVH.
- Calculate the collsion enter,exit and stay.
- Have the calculation take less time.

##  Images:
<img  src="Images/CollisionDetection_1.png" width="400" >
<img  src="Images/CollisionDetection_2.png" width="400" >
<img  src="Images/CollisionDetection_3.png" width="400" >
