# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFHistos")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

import sys
runNumber = sys.argv[2]
runType = int(sys.argv[3])
irunNumber = int(runNumber)
emap = "emap.txt"

process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring('file:./Data/USC_'+runNumber+'.root')
    #fileNames = cms.untracked.vstring('/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run'+runNumber+'/USC_'+runNumber+'.root')
)

process.hcalAnalyzer = cms.EDAnalyzer('HFHistoAnalyzer',
        OutFileName = cms.untracked.string('N_'+runNumber+'.root'),
	RunType = cms.int32(runType),
	histoFED =  cms.int32(60),
	driverFED = cms.int32(12)
)

process.load('Configuration.Geometry.GeometryIdeal_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond

from CondCore.CondDB.CondDB_cfi import *

process.GlobalTag.globaltag = autoCond['startup'] 

process.es_ascii = cms.ESSource('HcalTextCalibrations',
        input = cms.VPSet(
               cms.PSet(
                object = cms.string('ElectronicsMap'),
                file = cms.FileInPath('HCALCommissioning2017/HFHistoAnalyzer/test/'+emap)
               )
        )
)

process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')
process.p = cms.Path(process.hcalAnalyzer)

