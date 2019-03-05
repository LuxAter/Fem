off_x = 0.2
off_y = 0.5

function zero(x, y, t)
  return 0.0
end

function bowl(x, y, t)
  return math.pow(x - off_x, 2.0) + math.pow(y - off_y, 2.0)
end

function sine(x, y, t)
  return math.sin(x) * math.sin(y)
end

function dev(x, y, t)
  return 2 * (x - off_x) + 2 * (y - off_y) - 4
end
