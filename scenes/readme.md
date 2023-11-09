# LTFP Scene File Manual

The `.json` scene file configs the simulation in default mode. The default values are set in `src/SceneLoader.hpp`.

### Time
- minTimeStepSize(float): lower bound of time step size
- maxTimeStepSize(float): upper bound of time step size
- endTime(float): time-based terminate condition of simulation
- maxTimeSteps(int): step-based terminate condition of simulation

### Mesh 
- start(float vector3): minimum corner of the box domain
- end(float vector3): maximum corner of the box domain
- xCount(int): Number of mesh in X direction
- yCount(int): Number of mesh in Y direction
- zCount(int): Number of mesh in Z direction
- meshSize(float): mesh size of cubic mesh, priority is lower than xCount+yCount+zCount
- incrementThickness(float): thickness of each domain increment
- incrementPeriod(float): time period between domain increments

### Export
- enableVtkExport(bool): enable `.vtk` export of mesh data
- printPeriod(int): step period between printing info to log 
- exportPeriod(float): time period between data export

### Boundary
- type(int): type of thermal boundary
    - 0: Neumann boundary
    - 1: Dirichlet boundary
    - 2: Radiation boundary
- location(int): location of thermal boundary
    - 0: X positive
    - 1: X negative
    - 2: Y positive
    - 3: Y negative
    - 4: Z positive
    - 5: Z negative
- **incomplete**

### Material Property
- type(int): type of material property
    - 0: Density
    - 1: Specific heat
    - 2: Enthalpy
    - 3: Conductivity
    - Note that enthalpy has higher priority than specific heat
- tabulate(bool): enable tabulate of polynomial
- tempRange(float vector): temperature range endpoints
- polynomials(float vector): piecewise polynomials