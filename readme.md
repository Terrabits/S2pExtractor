R&S S2P Extractor
=================

Calculation
-----------

Using the 12-term VNA correction model for both calibrations, the S-Parameters can be calculated as follows.

denominator   = outer.reflTrack1 + (outer.sourceMatch1 * (inner.directivity1 - outer.directivity1))
s21 numerator = sqrt(outer.reflTrack1 * inner.reflTrack1)

S11 = (outer.reflTrack1 - inner.directivity1) / denominator
S21 = s21 numerator / denominator
S22 = inner.sourceMatch1 - ((outer.sourceMatch1 * inner.reflTrack1) / denominator)
