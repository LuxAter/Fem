time_dep = false
soln_dep = false

plsg="circle.pslg"
min_angle=20
max_area=0.01

A = {{1, 1}, {1, 1}}
B = {1, 1}
C = 1

function solution(x, y, t)
  return math.pow(x, 3.0) - 3.0 * x * math.pow(y, 2.0)
end

boundary_conditions = {}
boundary_conditions[0] = solution

function forcing(x, y, t, u)
  return -(6.0 * (A[1][1] * x - A[1][2] * y - A[2][1] * y - A[2][2] * x) +
    B[1] * (3 * math.pow(x, 2.0) - 3.0 * math.pow(y, 2.0)) -
    B[2] * 6.0 * x * y +
    C * (math.pow(x, 3.0) - 3.0 * x * math.pow(y, 2.0)))
end

function initial(x, y)
  return 0.0
end
