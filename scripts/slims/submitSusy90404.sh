#-------------------------
# Job parameters
#-------------------------
prefix=user
user=bcbutler
tag=v0
nFiles=4
options="--athenaTag=15.6.9.10,AtlasProduction --outputs Slim.root"

slimType=susy
isMC=true

#-------------------------
# Datasets to be slimmed
#-------------------------
###############################
#SUSY D3PD v904-04, release 16

datasets_old="
mc10_7TeV.118208.gl360_b240_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118209.gl360_b340_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118210.gl360_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118211.gl360_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118212.gl400_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118213.gl400_b240_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118214.gl400_b380_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118215.gl400_b340_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118216.gl400_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118217.gl450_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118218.gl450_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118219.gl450_b240_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118220.gl450_b420_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118221.gl450_b380_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118222.gl450_b340_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118223.gl500_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118224.gl500_b240_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118225.gl500_b470_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118226.gl500_b420_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118227.gl500_b380_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118228.gl500_b340_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118229.gl500_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118230.gl600_b240_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118231.gl600_b530_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118232.gl600_b470_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118233.gl600_b420_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118234.gl600_b380_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118235.gl600_b340_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118236.gl600_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118237.gl600_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118238.gl700_b380_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118239.gl700_b310_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118240.gl600_b580_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118241.gl700_b580_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118242.gl700_b530_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118243.gl700_b470_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118244.gl700_b420_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118245.gl700_b650_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118273.gl380_b000_t180_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118274.gl400_b000_t160_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118275.gl400_b000_t180_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118276.gl400_b000_t210_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118277.gl400_b000_t230_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118278.gl450_b000_t160_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118279.gl450_b000_t180_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118280.gl450_b000_t210_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118281.gl450_b000_t230_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118282.gl450_b000_t250_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118283.gl500_b000_t160_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118284.gl500_b000_t180_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118285.gl500_b000_t210_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118286.gl500_b000_t230_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118287.gl500_b000_t250_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118288.gl500_b000_t290_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118289.gl600_b000_t180_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118290.gl600_b000_t210_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118291.gl600_b000_t250_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118292.gl600_b000_t290_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
mc10_7TeV.118293.gl600_b000_t350_c120_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
"

datasets="
mc10_7TeV.118212.gl400_b280_t000_c000_n060_sl000_hwppsusy.merge.NTUP_SUSY.e660_s933_s946_r1831_r2040_p494/
"
#end of SUSYD3PD v904-4, release 16



#-------------------------
# Loop over datasets
#-------------------------
for ds in ${datasets}; do

 echo ${ds}
 
 run="${ds:10:6}"
 echo ${run}

 prun --exec "echo %IN | root.exe -b -q susy90404Slim.C\(${isMC}\)" ${options} --inDS ${ds} --outDS ${prefix}.${user}.susy904.${tag}.${ds} --nFilesPerJob ${nFiles} >& logfile_${run}.log
 
done

