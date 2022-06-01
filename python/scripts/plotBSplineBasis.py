import pysplinekernel
import numpy
import matplotlib.pyplot as plt

n = 101;
p = 3;
U = [0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0]

x = numpy.linspace( 0.0, 1.0, n )

m = len( U )

# loop over all basis functions
for i in range( m - p - 1 ):
  y = numpy.zeros( n )

  # loop over all evaluation points
  for j, xj in enumerate( x ):
    y[j] = pysplinekernel.evaluateBSplineBasis( xj, i, p, U )
    
  plt.plot( x, y )
  
plt.show( )

