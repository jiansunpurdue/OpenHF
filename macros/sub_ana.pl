#!/usr/local/bin/perl

sub produce_small_file_segment;

$njobs = $ARGV[0];
$start_from = $ARGV[1];
$type = $ARGV[2];
$filelist = $ARGV[3];
$cut_m_low = $ARGV[4];
$cut_m_high = $ARGV[5];
$cut_m_dau_tight_low = $ARGV[6];
$cut_m_dau_tight_high = $ARGV[7];
$cut_m_dau_loose_low = $ARGV[8];
$cut_m_dau_loose_high = $ARGV[9];
$cut_ffls3d = $ARGV[10];
$cut_falpha0 = $ARGV[11];
$cut_fprob = $ARGV[12];
$cut_fdr = $ARGV[13];
$cut_fchi2 = $ARGV[14];

#$HOME = "/home/sun229";
#$BASE = "/usr/rmt_share/scratch96/w/wxie";
$BASE = "/home/sun229/";
$workarea = "$BASE/DmesonAna/sl6ana/CMSSW_5_3_20/src/UserCode/OpenHF/macros";

open(FILE, "$filelist");
@lines = <FILE>;
close(FILE);

$itotal = @lines;

print "number of files: $itotal \n";


$ntot = 0;
$nfiles = int($itotal/$njobs)+1;
for ($ifile = 0; $ifile < $njobs ; $ifile ++) {

    $istart = $ifile*$nfiles;
    $iend = $nfiles + $istart;

    if($iend >$itotal) {
        $iend = $itotal;
    }

    if($istart < $start_from) {
        next;
    }

    #-- produce job exec file ...
    $job_execfile = "run\_$type\_$istart\_$iend";
    &produce_execfile;

    #-- produce condor configuration file ...
    $config_file = "condor\_$type\_$istart\_$iend.job";
    &produce_config_file;

    system("condor_submit $config_file");
#    system("./$job_execfile");

    if($iend == $itotal) {
        last;
    }
}


#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_execfile {
    open(OUTPUTFILE,">$job_execfile");

    print OUTPUTFILE ("#!/bin/sh \n");
	print OUTPUTFILE ("echo `hostname` \n");
	print OUTPUTFILE ("SCRAM_ARCH=slc6_amd64_gcc472 \n");
#	print OUTPUTFILE ("setenv CMS_PATH /apps/02/cmssoft/cms \n");
	print OUTPUTFILE ("source /apps/osg/cmssoft/cmsset_default.sh \n");
#	print OUTPUTFILE ("source /apps/02/cmssoft/cms/cmsset_default.sh \n");
#    print OUTPUTFILE ("source /opt/osg/setup.sh \n");
	print OUTPUTFILE ("export X509_USER_PROXY=/home/sun229/.myproxy  \n");
    print OUTPUTFILE ("DIR=\"/DmesonAna/sl6ana/CMSSW_5_3_20/src/UserCode/OpenHF/macros\" \n");
    print OUTPUTFILE ("cd $BASE/DmesonAna/sl6ana/CMSSW_5_3_20 \n");
    print OUTPUTFILE ("eval `scramv1 runtime -sh` \n");
    print OUTPUTFILE ("cd $BASE/\$DIR \n");
    print OUTPUTFILE ("root -b<<EOF\n");
    print OUTPUTFILE ("gSystem->Load\(\"\.\.\/\.\.\/\.\.\/\.\.\/lib\/slc6_amd64_gcc472\/libUserCodeOpenHF.so\"\)\; \n");
    print OUTPUTFILE (".x read_hftree.C($istart, $iend, \"$filelist\", $type, $cut_m_low, $cut_m_high, $cut_m_dau_tight_low, $cut_m_dau_tight_high, $cut_m_dau_loose_low, $cut_m_dau_loose_high, $cut_ffls3d, $cut_falpha0, $cut_fprob, $cut_fdr, $cut_fchi2) \n");
    print OUTPUTFILE (".q \n");
    print OUTPUTFILE ("EOF\n");

    system("chmod u+x $job_execfile");
}
#-----------------------------------------------------
# produce condor configuation file for each file list  

sub produce_config_file {
    open(OUTPUTFILE,">$config_file");

    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("#\n");
    print OUTPUTFILE ("# condor_submit condor_job\n");
    print OUTPUTFILE ("#\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("universe             = vanilla\n");
    print OUTPUTFILE ("executable           = $job_execfile\n");
    print OUTPUTFILE ("initialdir           = $workarea\n");
    print OUTPUTFILE ("should_Transfer_Files = NO\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("# get the environment (path, etc.)\n");
    print OUTPUTFILE ("Getenv         = True \n");
    print OUTPUTFILE ("# prefer to run on fast computers\n");
#    print OUTPUTFILE ("Rank           = kflops\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("output               = $job_execfile.out\n");
    print OUTPUTFILE ("error                = $job_execfile.err\n");
    print OUTPUTFILE ("log                  = $job_execfile.log\n");
	print OUTPUTFILE ("Getenv               = True\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("requirements =  Arch == \"X86_64\" && TARGET.UidDomain == \"rcac.purdue.edu\" && TARGET.Machine != \"airplay-render1.rcac.purdue.edu\" && TARGET.Machine != \"airplay-render2.rcac.purdue.edu\"\n");
    print OUTPUTFILE ("#======================================================================\n");
    print OUTPUTFILE ("+CMSJob = True\n");
    print OUTPUTFILE ("#+LENGTH=\"SHORT\"\n");
    print OUTPUTFILE ("\n");
    print OUTPUTFILE ("queue\n");
    print OUTPUTFILE ("\n");
}
