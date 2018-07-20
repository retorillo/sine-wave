@echo off
setlocal
set NAME=graph_1
gnuplot %NAME%.gpi ^
 && magick convert -delay 10 -loop 0 ^
      -coalesce -duplicate 1,-2-1 ^
      tmp/%NAME%*.png %NAME%.gif
