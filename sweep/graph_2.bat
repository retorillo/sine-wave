@echo off
setlocal
set name=graph_2
gnuplot %name%.gpi ^
  && magick convert -delay 10 -loop 0 ^
    -coalesce -duplicate 1,-2-1 ^
    tmp/%name%*.png %name%.gif
