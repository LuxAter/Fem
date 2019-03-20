function soln(x, y, t)
  return math.pow(x - 0.5, 2.0) + math.pow(y - 0.2, 2.0)
end

function forcing(x, y, t)
  return 0.0
  -- return 2 * (x + y - 1.7)
end
