
echo restoring vdisk .........
cp ../vdisk ./vdisk

echo
echo adding init.c .........
mku init

echo
echo adding login.c .........
mku login

echo
echo adding cat.c .........
mku cat

echo
echo adding more.c .........
mku more

echo
echo adding grep.c .........
mku grep

echo
echo adding l2u.c .........
mku l2u

echo
echo adding cp.c .........
mku cp

echo
echo adding sh.c .........
mku sh

echo
echo done.
