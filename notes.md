# Mesh #

* Keep representation simple:
  * Points
  * Triangles
  * Adjacency/Edges

Keeping things simple will be good, also then I can just use a pointer to the
class, reducing copies. The PSLG should be represented as a vector of points,
edges (with attributes), and holes.

Note that only the points are needed to construct the unconstrained mesh. Then
Edges and holes are used to construct the constrained mesh.

# Matrix #

Create a sparse matrix class using the systems that I learned from last
semester. There is no need for a sparse vector class, just use a variable sized
vector class that I can then use for the matrix multiplication. Both of these
classes must be variable dimension, because the program does not know the size
of the mesh when it is compiled. This will make things slower, but is
unfortunately necessary.

# LAS #

I have several implementation of the LAS, I just need to determine which method
will be best. I think that the conversion to the new implementation should be
pretty simple.

# Lambda #

Avoid the use of lambda functions, they make things much slower, which I don't
like very much. But they may be necessary. Lets see how I can handle it.
