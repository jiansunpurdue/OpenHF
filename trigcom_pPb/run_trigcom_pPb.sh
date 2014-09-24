#!/bin/sh
echo `hostname`
export SCRAM_ARCH=slc6_amd64_gcc472
source /apps/osg/cmssoft/cmsset_default.sh
#source /grp/cms/tools/glite/setup.sh
export X509_USER_PROXY=/home/sun229/.myproxy
DIR="/DmesonAna/sl6ana/CMSSW_5_3_20/src/UserCode/OpenHF/trigcom_pPb"
cd /home/sun229//DmesonAna/sl6ana/CMSSW_5_3_20/
eval `scramv1 runtime -sh`
cd /home/sun229//$DIR

startFile=$1
endFile=$2
filelist=$3
decaychannel=$4

root -l -b<<EOF
gSystem->Load("../../../../lib/slc6_amd64_gcc472/libUserCodeOpenHF.so");
.x run_trigcom_pPb.C($startFile, $endFile, "$filelist",$decaychannel)
.q
EOF

