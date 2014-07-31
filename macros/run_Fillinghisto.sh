#!/bin/sh
echo `hostname`
export SCRAM_ARCH=slc5_amd64_gcc434
source /apps/osg/cmssoft/cmsset_default.sh
#source /grp/cms/tools/glite/setup.sh
export X509_USER_PROXY=/home/sun229/.myproxy
DIR="/DmesonAna/CMSSW_4_4_4/src/UserCode/OpenHF/rereco_speactra"
cd /home/sun229//DmesonAna/CMSSW_4_4_4
eval `scramv1 runtime -sh`
cd /home/sun229//$DIR

startFile=$1
endFile=$2
filelist=$3
decaychannel=$4

root -b<<EOF
gSystem->Load("../../../../lib/slc5_amd64_gcc434/libAna00.so");
.x Fillinghisto.C++($startFile, $endFile, "$filelist",$decaychannel)
.q
EOF

