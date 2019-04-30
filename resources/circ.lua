mesh = "../pslg/circ.poly"
mesh_angle = 20
mesh_area = 0.005

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

function bndry_func(x, y, t)
  return math.atan2(x,y) + t
end

bndry = {}
bndry[0] = bndry_func

function force(x, y)
  return 0.0
  -- return math.sin(x)*math.sin(y)
end
