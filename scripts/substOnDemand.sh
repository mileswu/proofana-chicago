#!/bin/bash

# simple script to replace direct access to MakeClass/MakeSelector variables with calls to the on-demand functions (Get<type>())
# hopefully it saves you time, but it will NOT typically do everything right...you will still have to do plenty of editing (sorry)

if [ -z $3 ]; then
	echo "Usage: sh substOnDemand.sh declarations.h input.cxx output.cxx"
	exit 1
fi

ptrnum=0
num=0
#awk '{for (i=NF; i>0; i--) printf("%s ",i);printf ("\n")}'

## read declarations, sort into pointer/non-pointer type
lines="`cat ${1}`"
lines=`echo ${lines} | sed "s@unsigned @unsigned@g" | sed "s@> >@>>@g" | awk 'BEGIN { RS = ";" } {for (i=NF; i>0; i--) printf("%s%%",$i);printf ("\n")}'`
#echo "${lines}"
for line in ${lines}; do
	#echo ${line}
	if [ `echo ${line} | grep -c "*"` -gt 0 ]; then
		ptrtype[${ptrnum}]=`echo ${line} | awk -F"%" '{print $2}' | sed "s@*@@g" | sed "s@unsigned@unsigned @g" | sed "s@>>@> >@g"`
		ptrname[${ptrnum}]=`echo ${line} | awk -F"%" '{print $1}' | sed "s@*@@g" | sed "s@unsigned@unsigned @g" | sed "s@>>@> >@g"`
		echo "Pointer Type: ${ptrtype[${ptrnum}]} Name: ${ptrname[${ptrnum}]}"
		if [ "${ptrtype[${ptrnum}]}" != "" ]; then let ptrnum+=1; fi
	else
		type[${num}]=`echo ${line} | awk -F"%" '{print $2}' | sed "s@unsigned@unsigned @g" | sed "s@>>@> >@g"`
		name[${num}]=`echo ${line} | awk -F"%" '{print $1}' | sed "s@unsigned@unsigned @g" | sed "s@>>@> >@g"`
		echo "Type: ${type[${num}]} Name: ${name[${num}]}"
		if [ "${type[${num}]}" != "" ]; then let num+=1; fi
	fi
done


## read in input source, do replacements, write output source
src=`cat ${2}`

ptrcntr=0
cntr=0	
	
while [ ${ptrcntr} -lt ${ptrnum} ]; do
	src=`echo "${src}" | sed "s@${ptrname[${ptrcntr}]}->@Get<${ptrtype[${ptrcntr}]} >(\"${ptrname[${ptrcntr}]}\").@g"`
	src=`echo "${src}" | sed "s@*${ptrname[${ptrcntr}]}@Get<${ptrtype[${ptrcntr}]} >(\"${ptrname[${ptrcntr}]}\")@g"`
	let ptrcntr+=1
done

while [ ${cntr} -lt ${num} ]; do
	#echo ${name[${cntr}]}
	#echo ${type[${cntr}]}
	src=`echo "${src}" | sed "s@\"${name[${cntr}]}\"@IAmAPlaceHolderString@g"`
	src=`echo "${src}" | sed "s@${name[${cntr}]}@Get<${type[${cntr}]}>(\"${name[${cntr}]}\")@g"`
	src=`echo "${src}" | sed "s@IAmAPlaceHolderString@\"${name[${cntr}]}\"@g"`	
	let cntr+=1
done
	
echo "${src}" >> ${3}


