mesh = "../pslg/circ.poly"
mesh_angle = 20
mesh_area = 0.1683

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function force(x, y)
  return -2*math.sin(x)*math.sin(y)
end
