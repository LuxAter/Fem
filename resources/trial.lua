mesh = "../pslg/trial.poly"
mesh_angle = 20
mesh_area = 5.0

A = {{1, 0}, {0, 1}}
B = {1, 1}
C = 1

bndry = {}

dt = 0.05
tmax = 1.0

function init(x, y)
  if (math.abs(x) <= 0.2 and math.abs(y) <= 0.2) then
    return 10.0
  end
  return 0.0
end

function force(x, y)
  return 0.0
end
