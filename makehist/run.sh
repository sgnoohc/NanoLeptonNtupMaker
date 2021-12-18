rm -rf hists/2018/dy.root    ; mkdir -p hists/2018/; ./doAnalysis -i /nfs-7/userdata/phchang/tnp_output/v2/DY2018.root          -t variable -o hists/2018/dy.root     > hists/2018/dy.log     2>&1
rm -rf hists/2018/mudata.root; mkdir -p hists/2018/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2018*.root\" -t variable -o hists/2018/mudata.root > hists/2018/mudata.log 2>&1
rm -rf hists/2018/eldata.root; mkdir -p hists/2018/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/EGamma2018*.root\"   -t variable -o hists/2018/eldata.root > hists/2018/eldata.log 2>&1

rm -rf hists/2017/dy.root    ; mkdir -p hists/2017/; ./doAnalysis -i /nfs-7/userdata/phchang/tnp_output/v2/DY2017.root          -t variable -o hists/2017/dy.root     > hists/2017/dy.log     2>&1
rm -rf hists/2017/mudata.root; mkdir -p hists/2017/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2017*.root\" -t variable -o hists/2017/mudata.root > hists/2017/mudata.log 2>&1
rm -rf hists/2017/eldata.root; mkdir -p hists/2017/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleEl2017*.root\" -t variable -o hists/2017/eldata.root > hists/2017/eldata.log 2>&1

rm -rf hists/2016/dy.root    ; mkdir -p hists/2016/; ./doAnalysis -i /nfs-7/userdata/phchang/tnp_output/v2/DY2016.root          -t variable -o hists/2016/dy.root     > hists/2016/dy.log     2>&1
rm -rf hists/2016/mudata.root; mkdir -p hists/2016/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2016F.root,/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2016G.root,/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2016H.root\" -t variable -o hists/2016/mudata.root > hists/2016/mudata.log 2>&1
rm -rf hists/2016/eldata.root; mkdir -p hists/2016/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleEl2016F.root,/nfs-7/userdata/phchang/tnp_output/v2/SingleEl2016G.root,/nfs-7/userdata/phchang/tnp_output/v2/SingleEl2016H.root\" -t variable -o hists/2016/eldata.root > hists/2016/eldata.log 2>&1

rm -rf hists/2016APV/dy.root    ; mkdir -p hists/2016APV/; ./doAnalysis -i /nfs-7/userdata/phchang/tnp_output/v2/DY2016APV.root            -t variable -o hists/2016APV/dy.root     > hists/2016APV/dy.log     2>&1
rm -rf hists/2016APV/mudata.root; mkdir -p hists/2016APV/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleMu2016*HIPM*.root\" -t variable -o hists/2016APV/mudata.root > hists/2016APV/mudata.log 2>&1
rm -rf hists/2016APV/eldata.root; mkdir -p hists/2016APV/; ./doAnalysis -i \"/nfs-7/userdata/phchang/tnp_output/v2/SingleEl2016*HIPM*.root\" -t variable -o hists/2016APV/eldata.root > hists/2016APV/eldata.log 2>&1
