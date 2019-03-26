rho = 1
cp = 1
u = {1, 1}
k = 1

function soln(x, y)
  return math.sin(3.0 * x) + math.cos(3.0 * y)
end

function force(x, y)
  return 3.0*(math.cos(3.0*x)-math.sin(3.0*y)) + 9.0*k*(math.sin(3.0*x)+math.cos(3.0*y))
end
