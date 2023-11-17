# LTFP Scene File Manual

The `.json` scene file configs the simulation in default mode. The default values are set in `src/SceneLoader.hpp`.

### Time
- **minTimeStepSize**(float): lower bound of time step size
- **maxTimeStepSize**(float): upper bound of time step size
- **endTime**(float): time-based terminate condition of simulation
- **maxTimeSteps**(int): step-based terminate condition of simulation

### Mesh 
- **start**(float vector3): minimum corner of the box domain
- **end**(float vector3): maximum corner of the box domain
- **xCount**(int): Number of mesh in X direction
- **yCount**(int): Number of mesh in Y direction
- **zCount**(int): Number of mesh in Z direction
- **meshSize**(float): mesh size of cubic mesh, priority is lower than xCount+yCount+zCount
- **initialTemp"(float): initial temperature of domain
- **layerFile**(string): path to layer json file under scene dir

### Export
- **enableVtkExport**(bool): enable `.vtk` export of mesh data
- **printPeriod**(int): step period between printing info to log 
- **exportPeriod**(float): time period between data export

### Material Property
- **type**(int): type of material property
  - 0: Density
  - 1: Specific heat
  - 2: Enthalpy
  - 3: Conductivity
  - Note that enthalpy has higher priority than specific heat
- **tabulate**(bool): enable tabulate of polynomial
- **tempRange**(float vector): temperature range endpoints
- **polynomials**(float vector): piecewise polynomials
- **tabulateStep**(float vector): Step size during tabulation, default value is 0.1

### Boundary
- **type**(int): type of thermal boundary
    - 0: Neumann boundary
    - 1: Dirichlet boundary
    - 2: Radiation boundary
- **location**(int): location of thermal boundary
    - 0: X positive
    - 1: X negative
    - 2: Y positive
    - 3: Y negative
    - 4: Z positive
    - 5: Z negative
- **index**(int): integer value to help distinguish different boundary objects
- #### Neumann Boundary
  - **xFluxPoly**(float vector): polynomial of boundary flux depending on X position, ignored for boundary located in X direction
  - **yFluxPoly**(float vector): polynomial of boundary flux depending on Y position, ignored for boundary located in Y direction
  - **zFluxPoly**(float vector): polynomial of boundary flux depending on Z position, ignored for boundary located in Z direction
- #### Dirichlet Boundary
  - **xTempPoly**(float vector): polynomial of boundary temperature depending on X position, ignored for boundary located in X direction
  - **yTempPoly**(float vector): polynomial of boundary temperature depending on Y position, ignored for boundary located in Y direction
  - **zTempPoly**(float vector): polynomial of boundary temperature depending on Z position, ignored for boundary located in Z direction
- #### Convection Boundary
  - **ambientTemp**(float): ambient temperature
  - **convectionCoeff**(float): convection coefficient
- #### Radiation Boundary
  - **ambientTemp**(float): ambient temperature
  - **emissivityCoeff**(float): emissivity coefficient
- #### Mirror Boundary
  - No additional parameters

### Laser
- **index**(int): integer value to help distinguish different laser objects
- **type**(int): type of laser source
  - 0: 2D gaussian
  - 1: 2D top-head
  - 3: 3D gaussian (not implemented yet)
- **power**(float): power of laser source
- **absorptivity**(float): absorptivity of laser by the material
- **laserPath**(string): path to laser path json file under scene dir
- **radius**(float): radius of laser spot
- **depth**(float): depth of 3D gaussian profile