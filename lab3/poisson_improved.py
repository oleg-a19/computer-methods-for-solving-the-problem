from dolfin import *

# Create mesh and define function space
L = 10
W = 1
mesh = BoxMesh(Point(0, 0, 0), Point(L, W, W), 10, 3, 3)
V = FunctionSpace(mesh, "Lagrange", 1)


def boundary(x):
    return  x[2] < DOLFIN_EPS  or x[2] > 0.5 - DOLFIN_EPS

# Define boundary condition
u0 = Expression("1 + x[0]*x[0] + 2*x[1]*x[1]", degree=2)
bc = DirichletBC(V, u0, boundary)

# Define variational problem
u = TrialFunction(V)
v = TestFunction(V)
f = Expression("10*exp(-(pow(x[0] - 0.5, 2) + pow(x[1] - 0.5, 2)) / 0.02)", degree=2)
g = Expression("sin(5*x[0])", degree=2)
a = inner(grad(u), grad(v))*dx
L = f*v*dx + g*v*ds

# Compute solution
u = Function(V)
solve(a == L, u, bc)

# Save solution in VTK format
file = File("poisson/poisson2.pvd")
file << u

# Plot solution
#import matplotlib.pyplot as plt
#plot(u)
#plt.show()
