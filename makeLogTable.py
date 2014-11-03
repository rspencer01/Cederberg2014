### Utility for working out the logarithm mesh points
#
#
from math import log
# The x values used
a = [20, 22, 24, 26, 28, 30, 33, 36, 39, 42, 45, 48, 50, 55, 60, 65, 70, \
75, 80, 85, 90, 95, 100, 110, 120, 130, 140, 150, 160, 170, 185, 200, \
220, 240, 260, 280, 300, 320, 340, 360, 390, 420, 450, 480, 510, 550, \
590, 630, 680, 700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, \
1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2250, \
2400, 2650, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4500]
# How many points (used in the array declaration)
print len(a),"values"
# Get the scalar by which the input is multiplied
presc = float(raw_input("Prescaler: "))
# Get the scalar by which the output is multiplied
postsc = float(raw_input("Postscaler: "))
# Compute values
b = map(lambda x: postsc*log(presc*x),a)
# Print the outputs.
print ",".join(map(lambda x:"{:5.0f}".format(x),a))
print ",".join(map(lambda x:"{:5.0f}".format(x),b))
