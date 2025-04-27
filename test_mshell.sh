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
< $USER cat

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

<< e cat
> $zz
> '$zz'
> e

<< ee cat
> $USER
> ed
sflechel
ed

export zz="  1     2   "
echo $zz
echo s$zz
echo "$zz"
echo '$zz'

export zz="o test"
ech$zz

$USEr echo y
$USER echo y

cat > $USEr

echo s > o'$USER'tfile ; echo s > o"$USER"tfile

echo > /dev/stdin test | cat -e
echo > /dev/stdout test | cat -e

export tests=tests | echo $tests
export tests=tests
echo $tests

#builtin -> echo
echo -nnnnn -nnnnn -nnnnnnnnnnnnnnnnnnnnnnn
echo -nnnnn -nnnnn -nnnnnnnnnnnnnnnnnnnnnnnm -nnnn
echo -
echo "" -
echo "" -n
echo "" -n - a ""
echo -n ""
echo -nnnnnn ""

#builtin -> export
export 1test
export =test
export test='$testtttt'
export $test=testo
export test=test=testttt
export $test
export 1test
echo $?