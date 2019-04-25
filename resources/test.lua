mesh = "../resources/rect.poly"
mesh_angle = 20
mesh_area = 0.05

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function force(x, y)
  return x + y
  -- return -(6.0 * (A[1][1] * x - A[1][2] * y - A[2][1] * y - A[2][2] * x) +
  --   B[1] * (3 * math.pow(x, 2.0) - 3.0 * math.pow(y, 2.0)) -
  --   B[2] * 6.0 * x * y +
  --   C * (math.pow(x, 3.0) - 3.0 * x * math.pow(y, 2.0)))
  -- return 0.0
end
