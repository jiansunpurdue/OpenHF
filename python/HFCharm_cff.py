import FWCore.ParameterSet.Config as cms

# ----------------------------------------------------------------------
dstarDump = cms.EDAnalyzer(
    "HFDstar",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("hiSelectedVertex"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    D0Window           = cms.untracked.double(0.2),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(1), 
    isPbPb             = cms.untracked.bool(True)
    )
#

# ----------------------------------------------------------------------
d0Dump = cms.EDAnalyzer(
    "HFD0",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("hiSelectedVertex"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    D0Window           = cms.untracked.double(0.2),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(2),
    isPbPb             = cms.untracked.bool(True)
    )

# ----------------------------------------------------------------------
dsPhiPiDump = cms.EDAnalyzer(
    "HFDs2PhiPi",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("hiSelectedVertex"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    PhiWindow          = cms.untracked.double(0.1),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(3),
    isPbPb             = cms.untracked.bool(True)
    )
#
# ----------------------------------------------------------------------
dsKstarKaDump = cms.EDAnalyzer(
    "HFDs2KstarKaon",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("hiSelectedVertex"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowKaonPt         = cms.untracked.double(0.5),
    KstarWindow           = cms.untracked.double(0.5),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(4),
    isPbPb             = cms.untracked.bool(True)
    )
#
# ----------------------------------------------------------------------
dpmDump = cms.EDAnalyzer(
    "HFDpm2KaPiPi",
    verbose            = cms.untracked.int32(0), 
    tracksLabel        = cms.untracked.InputTag('hiGeneralTracks'),
    PrimaryVertexLabel = cms.untracked.InputTag("hiSelectedVertex"),
    trackPt            = cms.untracked.double(2.0),
    BeamSpotLabel      = cms.untracked.InputTag("offlineBeamSpot"),
    slowPionPt         = cms.untracked.double(0.5),
    maxDoca            = cms.untracked.double(0.1),
    maxD0              = cms.untracked.double(2.0),
    maxDz              = cms.untracked.double(99.0),
    type               = cms.untracked.int32(5),
    isPbPb             = cms.untracked.bool(True)
    )
#


# ######################################################################
# Sequences
# ######################################################################
d0Sequence     = cms.Sequence(d0Dump);
dstarSequence     = cms.Sequence(dstarDump);
d0dstarSequence     = cms.Sequence(d0Dump*dstarDump);
dpmSequence     = cms.Sequence(dpmDump);
dsSequence     = cms.Sequence(dsPhiPiDump*dsKstarKaDump);
dpmdsSequence = cms.Sequence(dpmDump*dsPhiPiDump*dsKstarKaDump);
charmSequence     = cms.Sequence(d0Dump*dstarDump*dpmDump*dsPhiPiDump*dsKstarKaDump);
