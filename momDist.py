import uproot as ur
import awkward as ak
import numpy as np
import matplotlib.pyplot as plt
from particle import Particle

# Load the file
fname     = 'e_10GeV_Ecal.edm4hep.root'
tree_name = "events"
tree      = ur.open(fname)[tree_name]
print(f"Read {fname}:{tree_name}. \n{tree.num_entries} events in total")

# Exercise 1.2: Inspect available branches
#print("\nAvailable branches:")
#print(tree.keys(filter_name="*", recursive=False))

# Extract MCParticles branch to DataFrame
bname = "MCParticles"
df    = tree[bname].array(library="ak")
df    = ak.to_dataframe(df)

# Show a preview
#print("\nFirst few rows of MCParticles:")
#print(df.head())

# Select electrons
part   = Particle.from_name("e-")
pdg_id = part.pdgid.abspid
condition1 = df["MCParticles.PDG"] == pdg_id

# Select primary electrons
condition2 = df["MCParticles.generatorStatus"] == 1

# Compute momentum for all electrons
#df_all = df[condition1]
#mom_all = np.sqrt(df_all["MCParticles.momentum.x"]**2 +
#                  df_all["MCParticles.momentum.y"]**2 +
#                  df_all["MCParticles.momentum.z"]**2)

# Compute momentum for primary electrons
df_primary = df[condition1 & condition2]
mom_primary = np.sqrt(df_primary["MCParticles.momentum.x"]**2 +
                      df_primary["MCParticles.momentum.y"]**2 +
                      df_primary["MCParticles.momentum.z"]**2)

# Plot histograms
bins = np.linspace(0, 20, 40)
#plt.hist(mom_all, bins=bins, alpha=0.5, label="All electrons")
plt.hist(mom_primary, bins=bins, histtype="step", color='r', label="Primary electrons")

# Add labels and grid
plt.xlabel("Momentum [GeV/c]")
plt.ylabel("Counts")
plt.title("Electron Momentum Distribution")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("momentum_distribution.png", dpi=150)
print("Plot saved as 'momentum_distribution.png'")