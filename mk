
echo restoring vdisk .........
cp ../vdisk ./vdisk

echo
echo adding init.c .........
mku init

echo
echo adding login.c .........
mku login

echo
echo adding l2u.c .........
mku l2u

echo
echo done.
