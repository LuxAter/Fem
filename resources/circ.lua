mesh = "../pslg/circ.poly"
mesh_angle = 20
mesh_area = 0.005

A = {{1, 0}, {0, 1}}
B = {0, 1}
C = 0


bndry = {}

function force(x, y)
  return math.sin(x)*math.sin(y)
end
