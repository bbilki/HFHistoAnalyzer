#!/bin/sh

if [ ! -d Data ]
then
	mkdir Data
fi

if [ ! -d Plots ]
then
	mkdir Plots
fi

if [ ! -d Plots/$1 ]
then
	mkdir Plots/$1
fi

if [ ! -d Histos ]
then
	mkdir Histos
fi

if [ ! -d Histos/$1 ]
then
	mkdir Histos/$1
fi

if [ ! -d NTuples ]
then
	mkdir NTuples
fi

# eos cp /eos/cms/store/group/dpg_hcal/comm_hcal/USC/run$1/USC_$1.root Data
# wait

cmsRun hfhistos_cfg.py $1 2
wait
mv N_$1.root NTuples
wait
# rm Data/USC_$1.root
 
# cd Files
# ./plotter $1
# wait
# # mv *.png ../Plots/$1/
# wait
# mv Results_$1.root ../Histos
# wait
# cd -
# 
# if [ -f Plots/$1/HFPADCMean.png ]
# then
# 	cd Plots
# 	tar -cf plots.tar $1
# 	wait
# 	gzip plots.tar
# 	wait
# 	scp -P 53222 plots.tar.gz hfSX5@feynman.physics.uiowa.edu:/var/www/html/HFSourcing
# 	wait
# 	ssh -p 53222 hfSX5@feynman.physics.uiowa.edu "cd /var/www/html/HFSourcing ; tar -zxf /var/www/html/HFSourcing/plots.tar.gz ; rm plots.tar.gz ; ./makePlotList.sh"
# 	wait
# 	rm plots.tar.gz
# 	cd -
# fi




