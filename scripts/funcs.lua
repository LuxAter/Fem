off_x = 0.2
off_y = 0.5

function forcing(x, y, t)
  return math.pow(x - off_x, 2.0) + math.pow(y - off_y, 2.0)
end

function soln(x, y, t)
  return 2 * (x - off_x) + 2 * (y - off_y) - 4
end
