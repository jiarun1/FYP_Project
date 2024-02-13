import matplotlib.pyplot as plot
import numpy as np


xa = 1
ya = 2
xb = 5
yb = 3
xc = 2
yc = 6


# calculate the area of the triangle
At = abs((ya+yb)*(xb-xa) + (yb+yc)*(xc-xb) + (ya+yc)*(xa-xc))/2

print(At)


a = np.sqrt(np.square(xc-xb) + np.square(yc-yb))
b = np.sqrt(np.square(xa-xc) + np.square(ya-yc))
c = np.sqrt(np.square(xb-xa) + np.square(yb-ya))

print("a=",a)
print("b=",b)
print("c=",c)

def tc(At,s,t,u):
    m = 4*At + np.sqrt(3)*(s*s + t*t - u*u)
    n = 4*At + np.sqrt(3)*(s*s - t*t + u*u)
    v = 8*At*(12*At + np.sqrt(3)*(s*s + t*t + u*u))
    return (m*n)/v


x_fermat = tc(At,a,b,c) * xa + tc(At, b, c, a) * xb + tc(At, c, a, b) * xc
y_fermat = tc(At,a,b,c) * ya + tc(At, b, c, a) * yb + tc(At, c, a, b) * yc

print ("(",x_fermat,",",y_fermat,")")








plot.rc('font',family='Arial')
plot.ion

# Plotting
plot.figure("Fermat Point Display",figsize=(21/2.54,21/2.54),dpi=200)
plot.plot([xa,xb], [ya, yb])
plot.plot([xb,xc], [yb, yc])
plot.plot([xc,xa], [yc, ya])

plot.scatter(x_fermat,y_fermat)


plot.tight_layout()
plot.show()