import sys
import os
import pwd

force = False
treename = ""
filename = ""
server = ""

if (len(sys.argv) < 3):
	print "Usage: python registerDataset.py server file_list.txt [treename] [force]"
	sys.exit(0)
elif (len(sys.argv) >= 3):
	server = sys.argv[1]
	filename = sys.argv[2]
if (len(sys.argv) > 3):
	if (sys.argv[3] == "force"):
		force = True
	else:
		treename = sys.argv[3]
if (len(sys.argv) > 4):
	if (sys.argv[4] == "force"):
		force = True
	else:
		treename = sys.argv[4]

sys.argv.append("-b")
from ROOT import TProof, TFileCollection

user = pwd.getpwuid(os.getuid())[0]
if(len(server.split("@"))!=2):
	server = "%s@%s" % (user,server)

if(filename.endswith(".txt") == False):
	print "Input file list",filename,"not of form xxxxxx.txt"
	sys.exit(1)

removetxt = filename.replace(".txt","").split("/")
dataset = removetxt[len(removetxt)-1]

proof = TProof.Open(server)

# check dataset's existence
if (proof.ExistsDataSet(dataset) == True):
	if(force == True):
		proof.RemoveDataSet(dataset)
	elif(treename != ""):
		print "Dataset already exists, setting tree name only"
		proof.SetDataSetTreeName(dataset,treename);
		sys.exit(0)
	else:
		print "Dataset already exists, use 'force' option to overwrite"
		sys.exit(1)

dset = TFileCollection(dataset,"",filename)

print "Adding new dataset ",dataset,", this may take a while for large datasets..."
proof.RegisterDataSet(dataset,dset,"V")
if(treename != ""):
	proof.SetDataSetTreeName(dataset,treename);
		
proof.ShowDataSets();
