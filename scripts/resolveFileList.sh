#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: sh resolveFileList.sh input_file_list.txt [multiple]"
    exit 1
fi

if [ `echo ${1} | grep -c .txt` -lt 1 ]; then
    echo "Input file needs to end in .txt"
    exit 1
fi

locatecmd="locateall"
if [ -z $2 ]; then
    locatecmd="locatesingle"
fi

filelist=${1}
filelistip=${1/.txt/.ip.txt}
filelistresolved=${1/.txt/.resolved.txt}

shift
shift

iplist=""

if [ ! -e ${filelistip} ]; then
    for file in `cat ${filelist}`; do
	echo "${file}"
	redirector=`echo ${file} | awk -F"//" '{print $2}'`
	redirectorhost=`echo ${redirector} | awk -F":" '{print $1}'`
	port=`echo ${redirector} | awk -F":" '{print $2}'`
	filename=`echo ${file} | sed "s@root://${redirectorhost}:${port}/@@"`
	serverip=`xrd ${redirector} ${locatecmd} ${filename} | grep Location: | awk '{print $2}' | awk -F"'" '{print $2}' | awk -F":" '{print $1}'`
	echo -e "root://${serverip}:${port}/${filename}" >> ${filelistip}
	iplist="${iplist}${serverip}\n"
    done
else
    for file in `cat ${filelistip}`; do
        echo "${file}"
        serveripport=`echo ${file} | awk -F"//" '{print $2}'`
        serverip=`echo ${serveripport} | awk -F":" '{print $1}'`
        iplist="${iplist}${serverip}\n"
    done
fi

substcommand="cat ${filelistip}"
for ip in `echo -e ${iplist} | sort | uniq`; do
    servername=`nslookup ${ip} | grep "name =" | awk '{print $4}'`
    servername=${servername%.*}
    substcommand="${substcommand} | sed -e s@${ip}@${servername}@g"
done

eval ${substcommand} > ${filelistresolved}

