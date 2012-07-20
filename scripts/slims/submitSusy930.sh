#-------------------------
# Job parameters
#-------------------------
prefix=user
user=dans  ##bcbutler ##phansson ##dans
tag=v00
nFiles=4
options="--athenaTag=15.6.9.10,AtlasProduction --outputs Slim.root --excludedSite=ANALY_FREIBURG"

slimType=susy
isMC=false ##true

#-------------------------
# Datasets to be slimmed
#-------------------------
###############################
#SUSY D3PD v930, release 16

#periodB
datasets_periodB="
group10.phys-susy.data11_7TeV.PeriodB.physics_JetTauEtmiss.PhysCont.NTUP_SUSY.repro08_v01_p543/
"

#Wjets
datasets="
mc10_7TeV.107280.AlpgenJimmyWbbFullNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.107281.AlpgenJimmyWbbFullNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.107282.AlpgenJimmyWbbFullNp2_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.107283.AlpgenJimmyWbbFullNp3_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117284.AlpgenWccFullNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117285.AlpgenWccFullNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117286.AlpgenWccFullNp2_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117287.AlpgenWccFullNp3_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117288.AlpgenWcNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117289.AlpgenWcNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117290.AlpgenWcNp2_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117291.AlpgenWcNp3_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
mc10_7TeV.117292.AlpgenWcNp4_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
"

#MC10A
datasets_mc="
data11_7TeV.00178044.physics_JetTauEtmiss.merge.NTUP_SUSY.r2276_p516_p523_p543/
"
#data11_7TeV.00180614.physics_JetTauEtmiss.merge.NTUP_SUSY.f369_m812_p543/
#data11_7TeV.00180153.physics_JetTauEtmiss.merge.NTUP_SUSY.f368_m806_p543/
#data11_7TeV.00179804.physics_JetTauEtmiss.merge.NTUP_SUSY.f363_m801_p543/

#group10.phys-susy.data11_7TeV.periodE.physics_JetTauEtmiss.PhysCont.NTUP_SUSY.t0pro08_v01_p543/
#group10.phys-susy.data11_7TeV.periodD.physics_JetTauEtmiss.PhysCont.NTUP_SUSY.t0pro08_v01_p543/
#group10.phys-susy.data11_7TeV.PeriodB.physics_JetTauEtmiss.PhysCont.NTUP_SUSY.repro08_v01_p543/

#mc10_7TeV.106280.AlpgenJimmyWbbNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.106281.AlpgenJimmyWbbNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.106282.AlpgenJimmyWbbNp2_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.106283.AlpgenJimmyWbbNp3_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107680.AlpgenJimmyWenuNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107681.AlpgenJimmyWenuNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107682.AlpgenJimmyWenuNp2_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107683.AlpgenJimmyWenuNp3_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107684.AlpgenJimmyWenuNp4_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107685.AlpgenJimmyWenuNp5_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107690.AlpgenJimmyWmunuNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107691.AlpgenJimmyWmunuNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107692.AlpgenJimmyWmunuNp2_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107693.AlpgenJimmyWmunuNp3_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107694.AlpgenJimmyWmunuNp4_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107695.AlpgenJimmyWmunuNp5_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107700.AlpgenJimmyWtaunuNp0_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107701.AlpgenJimmyWtaunuNp1_pt20.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107702.AlpgenJimmyWtaunuNp2_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107703.AlpgenJimmyWtaunuNp3_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107704.AlpgenJimmyWtaunuNp4_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107705.AlpgenJimmyWtaunuNp5_pt20.merge.NTUP_SUSY.e760_s933_s946_r2215_r2260_p543/


#mc10_7TeV.106484.SU4_herwigpp_susy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.105009.J0_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105010.J1_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105011.J2_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105012.J3_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105013.J4_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105014.J5_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105015.J6_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105016.J7_pythia_jetjet.merge.NTUP_SUSY.e574_s934_s946_r2213_r2260_p543/
#mc10_7TeV.105017.J8_pythia_jetjet.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/

#mc10_7TeV.105200.T1_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.105204.TTbar_FullHad_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108340.st_tchan_enu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108341.st_tchan_munu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108342.st_tchan_taunu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108343.st_schan_enu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108344.st_schan_munu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108345.st_schan_taunu_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/
#mc10_7TeV.108346.st_Wt_McAtNlo_Jimmy.merge.NTUP_SUSY.e598_s933_s946_r2215_r2260_p543/

#mc10_7TeV.107650.AlpgenJimmyZeeNp0_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107651.AlpgenJimmyZeeNp1_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107652.AlpgenJimmyZeeNp2_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107653.AlpgenJimmyZeeNp3_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107654.AlpgenJimmyZeeNp4_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107655.AlpgenJimmyZeeNp5_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107660.AlpgenJimmyZmumuNp0_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107661.AlpgenJimmyZmumuNp1_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107662.AlpgenJimmyZmumuNp2_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107663.AlpgenJimmyZmumuNp3_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107664.AlpgenJimmyZmumuNp4_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107665.AlpgenJimmyZmumuNp5_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107670.AlpgenJimmyZtautauNp0_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107671.AlpgenJimmyZtautauNp1_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107672.AlpgenJimmyZtautauNp2_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107673.AlpgenJimmyZtautauNp3_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107674.AlpgenJimmyZtautauNp4_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107675.AlpgenJimmyZtautauNp5_pt20.merge.NTUP_SUSY.e737_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107710.AlpgenJimmyZnunuNp0_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107711.AlpgenJimmyZnunuNp1_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107712.AlpgenJimmyZnunuNp2_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107713.AlpgenJimmyZnunuNp3_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107714.AlpgenJimmyZnunuNp4_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.107715.AlpgenJimmyZnunuNp5_pt20_filt1jet.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109300.AlpgenJimmyZeebbNp0_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109301.AlpgenJimmyZeebbNp1_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109302.AlpgenJimmyZeebbNp2_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109303.AlpgenJimmyZeebbNp3_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109305.AlpgenJimmyZmumubbNp0_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109306.AlpgenJimmyZmumubbNp1_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109307.AlpgenJimmyZmumubbNp2_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109308.AlpgenJimmyZmumubbNp3_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109310.AlpgenJimmyZtautaubbNp0_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109311.AlpgenJimmyZtautaubbNp1_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109312.AlpgenJimmyZtautaubbNp2_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/
#mc10_7TeV.109313.AlpgenJimmyZtautaubbNp3_nofilter.merge.NTUP_SUSY.e600_s933_s946_r2215_r2260_p543/


#end of SUSYD3PD v930, release 16


#-------------------------
# Loop over datasets
#-------------------------
for ds in ${datasets}; do

 echo ${ds}
 
 #run="${ds:30:7}" #group10.phys-susy.data11_7TeV.
 run="${ds:14:6}" #data11_7TeV.00
 #run="${ds:10:6}"  #mc10_7TeV.
 echo ${run}

 prun --exec "echo %IN | root.exe -b -q susy930Slim.C\(${isMC}\)" ${options} --inDS ${ds} --outDS ${prefix}.${user}.susy930.${tag}.${ds} --nFilesPerJob ${nFiles} >& logfile_${run}.log
 
done

