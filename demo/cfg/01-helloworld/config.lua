-- The default game configuration file.

window = Vec2Di.new(800, 600)

-- Just for example - GUI layout better be done within RML
labelRect = Rect2Di.new(Vec2Di.new(150, 40))
labelRect:centrify(Rect2Di.new(window))
