#!/usr/bin/python

pitches=["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
pitchesB=["C", "Db", "D", "Eb", "Fb", "F", "Gb", "G", "Ab", "A", "Bb", "B"]

def makepitchdict():
	pitchesDict={}
	for skala in [pitches, pitchesB]:
		i=0
		for pitch in skala:
			pitchesDict[pitch.upper()] = i
			i+=1
	return pitchesDict

pitchesDict=makepitchdict()

def transpose(pitchname, steps, b=0):
	newindex =  (pitchesDict[pitchname.upper()]+steps) %12
	if pitchname[0].isupper():
		if b:
			return pitchesB[newindex]
		else:
			return pitches[newindex]
	else:
		if b:
			return pitchesB[newindex].lower()
		else:
			return pitches[newindex].lower()
		


import getopt, sys
def main():
	"""
	Transposes the Chord Denotations of a label-file for the specified steps.

	parameters:
	-b       use b's, instead of sharps (#)
	-l       output long chordnames (root with a suffix Maj or Min)
	-s       output short chordnames (Uppercase = Maj, Lowercase = Min) 
	step     steps to transpose. 	steps must be positive (to tune down one step use step 11 instead of -1)
	file     filename of the label-file to transpose. label files have the format \"something[SPACES]chords	\"
	"""

	########################
	# Check Arguments
	########################
	try:
		opts, args = getopt.getopt(sys.argv[1:], "bls")
	except getopt.GetoptError:
		usage()
		
	b = False
	longname = False
	shortname = False
	for o, a in opts:
		if o=="-b":
			b= True
		elif o=="-l":
			longname=True
		elif o=='-s':
			shortname=True
		else:
			print "unsupported option %s" % (o)
			usage()

	if len(args) != 2  or (shortname and longname):
		usage()

	
	steps = int(args[0])
	tfilename = args[1]
	tfile = ''
	try:
		tfile = file(tfilename, "r")
	except IOError:
		print "Cannot open file %s" % (tfilename)
		usage()

	#print "steps: %s" % steps
	#print "filename: %s" % tfilename
	#print "use bs: %d" % b

	#########################
	# Scan File and transpose
	#########################

	output = ""
	error = 0
	errorlines = []
	linenr=0
	for line in tfile:
		try:
			secs, chords = line.split()
			#print "secs: .%s. chords: .%s." % (secs, chords)
			#continue
			newline = secs + "\t"
			chords = chords.split('/')
			for chord in chords:
				x=1
				pitch = chord[0]
				if len(chord)>1 and chord[1] in ['#', 'b']:
					pitch += chord[1]
					x = 2
				newpitch = transpose(pitch.strip(), steps, b)
				if longname:
					if newpitch[0].islower():
						newpitch = newpitch.upper() + "Min"
					else:
						newpitch = newpitch + "Maj"
				newchord = "%s%s" % (newpitch, chord[x:])
				if shortname and len(chord)>=x+3:
					if  chord[x:x+3]=='Maj':
						newchord  = newpitch[0].upper() + newpitch[1:]
					else:
						newchord = newpitch[0].lower() + newpitch[1:]
				newline += newchord
				if chords.index(chord)+1 != len(chords):
					newline += "/"
			output += newline + "\n"
		except Exception, e:
			output += line
			errorlines.append(linenr)
		linenr+=1


	print output[:-1],

	if errorlines:
		print >> sys.stderr, "" 
		print >> sys.stderr,  "" 
		print >> sys.stderr,  "There were %d lines, that couldn't be processed: line nr %s. These lines were copied without chagens to the output" % (len(errorlines), errorlines)
		


def usage():
	print "%s [-b [-l|-s ]] steps filename" % sys.argv[0]
	sys.exit(2)

	
	
	
	
if __name__ == '__main__': main()
