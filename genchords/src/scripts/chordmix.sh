#!/bin/sh

# *****************************************************************************
# takes a csound scorefile and a wav file
# processes the scorefile to ouput another wavfile
# merges the two wav files to a single stereo file that contains the music from the 
# scorefile in the left, and the original wavfile in the left channel
# the resulting wavfiles are saved in </path/to/soundfile>/chords/
# and are named <soundfile>__chords.wav and <soundfile>__mixed.wav
#
# Author: veronika.zenz@google.com
# *****************************************************************************


# ---------------------- INITS ------------------------

# TODO: pfad verallgemeinern
ORC_FILE=`dirname $0`"/comp.orc"


# ------------------ HELPER FUNCTIONS ------------------

mybasename() {
	REPLY=${1##*/}
}

mygetpath() {
	REPLY=${1%/*}
}

# -----------------------------
# takes as first argument a filename
# filename may be 
# file.txt, ../file.txt, /path/to/file/file.txt
# saves absolute path in variable REPLY
# ------------------------------
myabsolutefilename (){

	mybasename "$1"
	local filename=$REPLY
	mygetpath $1
	local dir=$REPLY
	
	if [ -d $dir ]
	then
		pushd $dir &> /dev/null
	fi
	local absolutedir=`pwd`
	REPLY="$absolutedir/$filename"
	popd &> /dev/null
}

fileok() {
if [ ! -e "$1" ]
then
	echo "File $1 does not exists"
	echo "exiting"
	exit 1
fi
}

bailout() {
echo "$cmdname: $1"
exit 1
}

checkexitcode() {
if [ $? -ne 0 ]
then
	bailout "$1 failed"
fi
}


# ---------------------------------------
# ------------ CHECK PARAMETERS ---------
# ---------------------------------------

cmdname=`basename $0`
if [ $# -ne 3 ]
then
	echo "Usage: `cmdname` scorefile soundfile outputdir"
	exit $WRONG_ARGS
fi

# check that the files given as arguments exist
fileok "$1" && fileok "$2"

# save the files with their absolute pathname

myabsolutefilename "$1"
scorefile="$REPLY"
myabsolutefilename "$2"
soundfile="$REPLY"

#/outputdir=${soundfile%/*}/chords
outputdir="$3"
chordsoundfile="$outputdir/`basename \"${soundfile%.*}\"`__chords.wav"
mixedsoundfile="$outputdir/`basename \"${soundfile%.*}\"`__mixed.wav"

echo "-------- FILES OKAY ------------"
echo "scorefile: $scorefile"
echo "soundfile: $soundfile"
echo "created chordsoundfile: $chordsoundfile"
echo && echo

if [ ! -e $outputdir ]
then
	mkdir $outputdir || bailout "mkdir $outputdir failed"
	echo "Created directory $outputdir for output."
fi;


# ---------------------------------------
# ------------ REAL FUNCTIONALITY -------
# ---------------------------------------

# autocomp
#echo "--------- AUTOCOMP ------------"
#pushd $AUTOCOMP_DIR &> /dev/null
#autocomp.pl -b 0 -d 0 -i "$textfile" -o "$textfile".sco || bailout "autocomp failed"
#echo && echo

# csound
echo "----------- CSOUND ------------"
echo "we ar in directoy `pwd`"

# if [ -e "$chordsoundfile" ]; then
# 	echo "chordsoundfile exists already. No generation"
# else
	echo "csound -W -o $chordsoundfile -d -m 0 $ORC_FILE $scorefile"
    #csound -W -o "/home/zenz/src/genchords/test/files/froesche.wav" -d -m 0 $AUTOCOMP_DIR/comp.orc #"$scorefile"  || bailout "csound failed"
	csound -W -o "$chordsoundfile" -d -m 0 $ORC_FILE "$scorefile"  || bailout "csound failed"
# fi

echo && echo


# sox
echo "----------- SOX ------------"									

# make the output stereo with one channel containing the chords, the other the original song
# thus users can ajoust the volume themselves

# taken from http://lists.agnula.org/pipermail/users/2005-September/008282.html
#1) convert both mono to stereo sox file1.wav -c2 file1_st.wav
#2) use avg to mix them into single channel stereo files
#   sox file1_st.wav out1.wav avg 1.0,0.0,0.0,0.0
#   sox file2_st.wav out2.wav avg 0.0,1.0,0.0,0.0
#3) mix them
#   soxmix out1.wav  out2.wav final.wav

echo "Create single channel stereo from chordsoundfile"
sox "$chordsoundfile" "$chordsoundfile"_tmp.wav avg 1.0,0.0,0.0,0.0 || bailout "sox failed to create single channel stereo"


echo "Create single channel stereo from input-soundfile"
# __TODO__: it this failes try to create stereo file first.
# is there any way to get the nr of channels from sox?
sox "$soundfile" "$soundfile"_tmp.wav avg 0.0,1.0,0.0,0.0 || bailout "sox failed to create single channel
 stereo"



# if the output of csound is too long trim it to the inputsong-size with
# sox chord.wav chordnew.wav trim 0s <nofsample>s

len1=`shntool info "$chordsoundfile" | egrep "^data size:" | awk '{print $3}'`
len2=`shntool info "$soundfile" | egrep "^data size:" | awk '{print $3}'`
#echo "."$len1"."
#echo "."$len2"."

if [ -n "$len1" -a -n "$len2" ]; then
	if [ "$len1" -lt "$len2" ]; then
		echo "trim soundfile from $len2 to $len1"
		sox "$soundfile"_tmp.wav "$soundfile"_tmp2.wav trim 0s "$len1"s || bailout "trimming the soundfile failed"
		mv "$soundfile"_tmp2.wav "$soundfile"_tmp.wav
	else
		echo "trim chordfile from $len1 to $len2"
		sox "$chordsoundfile"_tmp.wav "$chordsoundfile"_tmp2.wav trim 0s "$len1"s || bailout "trimming the chordsoundfile failed"
		mv "$chordsoundfile"_tmp2.wav "$chordsoundfile"_tmp.wav
	fi
else
	echo "no trimming needed"
fi





echo "Mix original and chordfile. Chords are left, original right channel"
soxmix -v 0.5 "$soundfile"_tmp.wav -v 0.5 "$chordsoundfile"_tmp.wav "$mixedsoundfile" || bailout "mixing the soundfiles with sox failed"

echo "Delete temporary data"
rm "$chordsoundfile"_tmp.wav || bailout "Deletion of ${chordsoundfile}_tmp.wav failed"
rm "$soundfile"_tmp.wav || bailout "Deletion of ${soundfile}_tmp.wav  failed"
rm "$chordsoundfile" || bailout "Deletion of ${chordsoundfile}  failed"

echo && echo
echo "${mixedsoundfile} generated successfully"
echo "Bye..."
echo && echo

exit 0