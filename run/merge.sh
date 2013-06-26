command -v hadd >/dev/null 2>&1 || { echo >&2 "I require hadd. Aborting."; exit 1; }

for i in `ls *.[0-9]*.root | sed 's/.[0-9]*.root//g' | uniq `
do
     echo "Merging ${i}"
     hadd ${i}.root ${i}.*.root
     rm ${i}.*.root
done

