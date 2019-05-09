mesh = "../pslg/circ.poly"
mesh_angle = 20
mesh_area = 0.01

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function soln(x, y)
  return math.sin(x) * math.sin(y)
end

bndry[0] = soln

function force(x, y)
  return -2*math.sin(x)*math.sin(y)
end
