mesh = "../pslg/rect.poly"
mesh_angle = 20
mesh_area = 0.01

A = {{1, 0}, {0, 1}}
B = {0, 0}
C = 0

bndry = {}

function soln(x, y)
  if (x == 0 and y == 0) then
    return 1.0
  else
    return math.sin(10 * math.sqrt(x * x + y * y)) /
      (math.sqrt(x * x + y * y) * 10)
  end
end

-- bndry[0] = soln

function force(x, y)
  if (x == 0 and y == 0) then
    return 0.0
  else
    local r = x * x + y * y
    local sr = 10 * math.sqrt(r)
    return -10 * r * math.sin(sr) / math.pow(r, 3 / 2) -
      2 * math.sin(sr) / (10 * math.pow(r, 3 / 2)) +
      3 * r * math.sin(sr) / (10 * math.pow(r, 5 / 2)) +
      2 * math.cos(sr) / r -
      3 * r * math.cos(sr) / math.pow(r, 2)
  end
end
