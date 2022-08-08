# ATLTileCalTB plots

## Sampling Fraction

![[PDF](./sampling_fraction.pdf)](./sampling_fraction.pdf)

The sampling fraction is the fraction of the energy that is deposited in the tiles compared to the
energy deposited in the entire calorimeter. To estimate this, 10 GeV electrons were simulated and
the energy deposit in the tiles collected. The plot shows the energy (in MeV) on the x-axis, which
shows an average of about 3.3% for the sampling fraction.
The simulation was done with Geant4 10.7.p03 and FTFP_BERT.

## Correction for photoelectron conversion

![[PDF](./pe_conversion_correction.pdf)](./pe_conversion_correction.pdf)

The conversion from deposited energy to photoelectrons (pe) is given as 70 pe per GeV. Since this
is an effective result, two corrections need to be made in the simulation: one is the above
mentioned sampling fraction, the other is an estimation of the effect from Birk's Law and the
U-Shape of the tiles. To estimate this correction, 10 GeV electrons were simulated and the amount
of created photoelectrons was counted. In the plot the count is plotted on the x-axis. The
correction is obtained by dividing the expect 700 pe by the fitted mean.
The simulation was done with Geant4 10.7.p03 and FTFP_BERT.

## PMT electric noise

![[PDF](./pmt_noise.pdf)](./pmt_noise.pdf)

The electronic noise of the PMT is given as an effective uncertainty of 12 MeV by ATLAS. To convert
this to ADC units, 10 GeV electrons were simulated and the signal linearly scaled to 12 MeV. The
noise level is about 0.85 ADC units.
The simulation was done with Geant4 10.7.p03 and FTFP_BERT.
