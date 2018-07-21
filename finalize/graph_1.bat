@echo off
setlocal
set name=graph_1
gnuplot %name%.gpi ^
  && magick convert -delay 4 -loop 0 ^
    tmp/%name%_*.png %name%.gif
