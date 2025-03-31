from DDSim.DD4hepSimulation import DD4hepSimulation
from g4units import mm, GeV, MeV, deg
SIM = DD4hepSimulation() 

SIM.gun.thetaMin = 135*deg
SIM.gun.thetaMax = 177*deg
SIM.gun.distribution = "cos(theta)"
SIM.gun.momentumMin = 1*GeV
SIM.gun.momentumMax = 1*GeV
SIM.gun.particle = "e-"
SIM.enableGun = True
SIM.numberOfEvents = 1000
SIM.outputFile = "e_1GeV_Ecal_1e3.edm4hep.root"