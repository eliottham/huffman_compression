# Check files if they have been collected #
echo "RUNNING SUBMISSION SCRIPT"
echo
if [[ -f Makefile ]] ; then
	echo "Makefile exists in directory"
else
	echo "MISSING REQUIRED FILE: Makefile. Exiting"
	exit
fi
if [[ -f compress.cpp ]] ; then
	echo "compress.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: compress.cpp. Exiting"
	exit
fi
if [[ -f uncompress.cpp ]] ; then
	echo "uncompress.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: uncompress.cpp. Exiting"
	exit
fi
if [[ -f HCNode.h ]] ; then
	echo "HCNode.h exists in directory"
else
	echo "MISSING REQUIRED FILE: HCNode.h. Exiting"
	exit
fi
if [[ -f HCNode.cpp ]] ; then
	echo "HCNode.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: HCNode.cpp. Exiting"
	exit
fi
if [[ -f HCTree.h ]] ; then
	echo "HCTree.h exists in directory"
else
	echo "MISSING REQUIRED FILE: HCTree.h. Exiting"
	exit
fi
if [[ -f HCTree.cpp ]] ; then
	echo "HCTree.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: HCTree.cpp. Exiting"
	exit
fi
if [[ -f BitInputStream.h ]] ; then
	echo "BitInputStream.h exists in directory"
else
	echo "MISSING REQUIRED FILE: BitInputStream.h . Exiting"
	exit
fi
if [[ -f BitInputStream.cpp ]] ; then
	echo "BitInputStream.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: BitInputStream.cpp . Exiting"
	exit
fi
if [[ -f BitOutputStream.h ]] ; then
	echo "BitOutputStream.h exists in directory"
else
	echo "MISSING REQUIRED FILE: BitOutputStream.h . Exiting"
	exit
fi
if [[ -f BitOutputStream.cpp ]] ; then
	echo "BitOutputStream.cpp exists in directory"
else
	echo "MISSING REQUIRED FILE: BitOutputStream.cpp . Exiting"
	exit
fi
if [[ -f Report.pdf ]] ; then
	echo "Report.pdf exists in directory"
else
	echo "MISSING REQUIRED FILE: Report.pdf . Exiting"
	exit
fi
# Check compilation #
make clean > temp.txt
make all > temp.txt 2>&1
if [[ $? != 0 ]] ; then
	echo "Code doesn't compile. Exiting"
	make clean > temp.txt
	rm -f temp.txt
	exit
fi 
echo "Code compiles successfully! (warnings may still exist)"
echo
ASNLIB='./asnlib'
cp $ASNLIB/binary.dat ./
cp $ASNLIB/check1.txt ./
# Check compress and uncompress on ascii and binary file works
./compress check1.txt check1_comp
./uncompress check1_comp check1_uncomp.txt
diff -s check1.txt check1_uncomp.txt > temp.txt
if [[ $? != 0 ]] ; then
    echo "Error, result of compress and uncompress on check1.txt did not give original file."
	echo "Diff results:"
	echo "########"
	cat temp.txt
	echo -e "########\nExiting"
	make clean > temp.txt
	rm -f temp.txt check1.txt check1_comp check1_uncomp.txt
	exit
fi
# Don't need these files anymore and just cleaner to get rid of them now
rm -f check1.txt check1_comp check1_uncomp.txt
./compress binary.dat bin_comp
./uncompress bin_comp bin_uncomp.dat
diff -s binary.dat bin_uncomp.dat > temp.txt
if [[ $? != 0 ]] ; then
    echo "Error, result of compress and uncompress on binary.dat did not give original file. Exiting."
	make clean > temp.txt
	rm -f temp.txt binary.dat bin_comp bin_uncomp.dat
	exit
fi
# Check for memory leaks
echo
valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./compress binary.dat bin_comp \
	>& temp.txt
grep -q "no leaks are possible" temp.txt
if [[ $? != 0 ]] ; then
    echo "Memory leaks detected when running compress. Exiting"
	cat temp.txt
	make clean > temp.txt
	rm -f temp.txt binary.dat bin_comp bin_uncomp.dat
	exit
fi
valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./uncompress bin_comp bin_uncomp.dat \
	>& temp.txt
grep -q "no leaks are possible" temp.txt
if [[ $? != 0 ]] ; then
    echo "Memory leaks detected when running uncompress. Exiting"
	make clean > temp.txt
	rm -f temp.txt binary.dat bin_comp bin_uncomp.dat
	exit
fi
echo "compress and uncompress passed initial check. No memory leaks detected."
make clean > temp.txt
rm -f temp.txt binary.dat bin_comp bin_uncomp.dat
echo "All submission checks passed. Now creating PA3.tar.gz."


tar_file="PA3.tar.gz"
input_files="Makefile compress.cpp uncompress.cpp HCNode.h HCNode.cpp HCTree.h HCTree.cpp BitInputStream.h BitInputStream.cpp BitOutputStream.h BitOutputStream.cpp Report.pdf"

tar -zcvf $tar_file $input_files
if [[ $? != 0 ]] ; then
    echo "Unable to create $tar_file! Exit Code: $?"
    exit
else
    echo "Successfully created $tar_file. Please submit this file to TritonEd in your new PA3 group!"
fi
