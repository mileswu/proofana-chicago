#!/user/bin/env/python

import sys, os, string, re


def usage():

	print "python mergeMult.py target_file regexp [str1] [str2] [test]"
	sys.exit(1)



if(len(sys.argv)<3):
	usage()


target = sys.argv[1]
regexp = sys.argv[2]

str1=""
str2=""

if(len(sys.argv)>3):
	str1=sys.argv[3]

if(len(sys.argv)>4):
	str2=sys.argv[4]

test = ""
if(len(sys.argv)>5):
	test=sys.argv[5]

print "Target:    %s " % target
print "RegExp:    %s " % regexp
print "str1:      %s " % str1
print "str2:      %s " % str2
print "test?:     %s " % test



#list all files matching regexp


files = os.listdir(".")
selected = []
for f in files:
	#print "%s" % f
	matchObj = re.match(regexp,f)
	if matchObj:
		print "%s was selected" % f
		selected.append(f)

# if there are strings defined, go through each file and hadd those that match


if str1=="" and str2=="":
	print "Simply merge all %i files " % len(selected)	
	line = "hadd %s" % target
	for f in selected:
		if not '.root' in f:
			print "Is this really a root file? %s " % f
			sys.exit(1)
		line += " %s" % f
	print "Merge command:\n\"%s\"" % line
	os.system(line)
	sys.exit(0)


print "Merge matching strings"

for f in selected:
	if not 'root' in f:
		print "Is this really a root file? %s " % f
		sys.exit(1)
	if not str1 in f:
		continue
	#this file should be merged with another file with same name except str2 replaces str1
	f2 = f.replace(str1,str2)
	if not f2 in selected:
		print "Couldn't find %s created from %s" % (f2,f)
		sys.exit(1)
	line = "hadd M%s %s %s" % (f,f,f2)
	selected.remove(f)
	selected.remove(f2)
	print line
	if test=="":
		os.system(line)




