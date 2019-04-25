mesh = "../pslg/circ.poly"
mesh_angle = 20
mesh_area = 0.01

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function force(x, y)
  return 1/((x*x)+(y*y)+1)
end
