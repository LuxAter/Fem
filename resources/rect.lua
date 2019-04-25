mesh = "../pslg/rect.poly"
mesh_angle = 20
mesh_area = 0.01

A = {{1, 0}, {0, 1}}
B = {1, 1}
C = 1

bndry = {}

function bndry_func(x, y)
  return x ^ 3 - 3 * x * y ^ 2
end

bndry[0] = bndry_func

function force(x, y)
  return 3 * x ^ 2 - 3 * y ^ 2 - 6 * x * y + x ^ 3 - 3 * x * y ^ 2
end
