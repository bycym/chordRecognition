 #!/bin/sh

cmdname=`basename $0`
if [ $# -ne 1 ]
then
        echo "Usage: `cmdname` outdir"
        exit $WRONG_ARGS
fi


outdir="$1"

for i in $( ls *.wav ); do
	echo "$i" "..."
#	sox "$i"  -r 22050  "$outdir"/"$i" resample -qs
	sox "$i" "$outdir"/"$i" trim 0 60
#	sox "$i"  "$outdir"/"$i" filter 0-1000
#	sox "$i"  "$outdir"/"$i" filter 1000-0
#   java -jar /home/zenz/src/Genchords/lib-src/beatroot-0.5.3.jar -b -o "$outdir/$i" "$i"
done
exit

