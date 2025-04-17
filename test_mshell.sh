export 'LESSER<'='>'
export LESSER=<
cat $LESSER< infile
cat <$LESSER infile
cat>outfile1</dev/stdin|cat>outfile2
cat > /dev/stdin < /dev/stdout | cat
cat > /dev/stderr < /dev/stderr | cat
cat<infile3<infile1</dev/stdin
rev < infile | cat -e
rev | < infile cat -e
echo o >> outfile | echo p > outfile && cat outfile
<< $USER cat
<< e'e' cat

> $USER
> ed
> ee
$USER
ed

<< "ee" cat
> $USER
> ed
> ee
$USER
ed

<< ee cat
> $USER
> ed
sflechel
ed

export zz="1     2"
echo $zz
echo "$zz"

$USEr echo y

$USER echo y

cat > $USEr
