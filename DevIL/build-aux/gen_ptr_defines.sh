# Source with bash (we use shell arrays here)

# Examine configure.ac
# 	for arguments of the TEST_FORMAT macro
ALL_FORMATS=$(grep TEST_FORMAT configure.ac | \
	sed -e 's/^TEST_FORMAT(\[[^[]*\[\([A-Z0-9]*\)\].*/\1/')

srcdir="."
PTR_DEFINES_H_FILENAME="ptr_defines.h"
PTR_DEFINES_H_PATH="$srcdir/include/IL"
mkdir -p $PTR_DEFINES_H_PATH
PTR_DEFINES_H="$PTR_DEFINES_H_PATH/$PTR_DEFINES_H_FILENAME"
PTR_DEFINES_H_TEMP="${PTR_DEFINES_H}.temp"
# Let's tell the user. This is not an easy task.
echo "Generating function pointer #defines to the $PTR_DEFINES_H file. This will take some time..."
# Let's make a nice header guard
PTR_DEFINES_H_CODE=$(echo $PTR_DEFINES_H_FILENAME | tr 'a-z' 'A-Z' | tr '/.' '_')
# Putting the first header guard in place...
# We have to quote it because '#' is a nasty m4 character
echo "#ifndef $PTR_DEFINES_H_CODE" > $PTR_DEFINES_H_TEMP
echo "#define $PTR_DEFINES_H_CODE" >> $PTR_DEFINES_H_TEMP
echo >> $PTR_DEFINES_H_TEMP
# We even have to include something in our header. Because of typedefs...
echo '#include "IL/devil_internal_exports.h"' >> $PTR_DEFINES_H_TEMP
echo >> $PTR_DEFINES_H_TEMP
# We have to take care of C++ compilers since we are outputting declarations..
echo "#ifdef __cplusplus" >> $PTR_DEFINES_H_TEMP
echo 'extern "C" {' >> $PTR_DEFINES_H_TEMP
echo '#endif /* __cplusplus */' >> $PTR_DEFINES_H_TEMP
echo >> $PTR_DEFINES_H_TEMP
# Now we will take a look what return type and what parameters do those functions have.
OPERATIONS=( 	"ilLoad" 	"ilLoadF" 	"ilLoadL" \
	"ilSave" 	"ilSaveF" 	"ilSaveL" \
	"ilIsValid" 	"ilIsValidF" 	"ilIsValidL" )
for (( op=0; $op < ${#OPERATIONS[@]}; op++ ))
do
	# We do this by investigating declaration of typedefs of pointers to those functions
	OPERATION_PTR_TYPEDEF=$(sed -n "/typedef.*${OPERATIONS[$op]}_ptr/p" $srcdir/src-IL/include/il_internal.h | sed -e 's/.*typedef\s*\(.*\);/\1/')
	# Just extract return types nad parameters (in brackets)
	RETURN_TYPES[$op]=$(echo $OPERATION_PTR_TYPEDEF | sed -e 's/^\(\w*\).*/\1/')
	PARAMETERS[$op]=$(echo $OPERATION_PTR_TYPEDEF | sed -e 's/.*\(([^()]*)\)\s*$/\1/')
done
# Yes, we have to take care even of formats that we won't be compiling in...
for FORMAT in $ALL_FORMATS
do			
	# Let's limit the number of files where we search for every format
	FORMAT_FILES=$(grep -l "_${FORMAT}" \
		'--exclude=*il_io.c' '--exclude=*il_stack.c' '--exclude=*il_states.c' '--exclude=*il_internal.c' $srcdir/src-IL/src/*)
echo '/* Now examining format '"$FORMAT"' */' >> $PTR_DEFINES_H_TEMP
echo "#ifndef IL_NO_$FORMAT"' /* we support that format */' >> $PTR_DEFINES_H_TEMP

for (( op=0; $op < ${#OPERATIONS[@]}; op++ ))
do
	PTR_DEFINE="${OPERATIONS[$op]}_${FORMAT}_PTR"
	# Do we compile the format in? Is that format function mentioned in the source code?
	# We really need the "\<${FORMAT}\>" stuff here since for example BMP is contained in WBMP...
	if grep -q "${OPERATIONS[$op]}_${FORMAT}" $FORMAT_FILES
	then
		# OK, we will make a declaration and #definition of pointer to that function
		echo "${RETURN_TYPES[$op]} ${OPERATIONS[$op]}_${FORMAT}${PARAMETERS[$op]};" >> $PTR_DEFINES_H_TEMP
		echo "#define $PTR_DEFINE & ${OPERATIONS[$op]}_${FORMAT}" >> $PTR_DEFINES_H_TEMP
		echo >> $PTR_DEFINES_H_TEMP
		defines="$defines $op"
	else
		# OK, we will just say that the pointer to that function is NULL.
		nulls="$nulls $op"
	fi
done
# We aim at making an index set containing indexes of operations that may be supported by DevIL
null_seq=$(seq 0 8)
# This removes indexes of operation that are not supported at all in DevIL from the index sequence
null_seq=$(echo $null_seq | tr "$nulls" ' ')
# ifdef IL_NO_xxx
echo '#else /*'" we don't support format $FORMAT" '*/' >> $PTR_DEFINES_H_TEMP
for op in $null_seq
do
	echo "#define ${OPERATIONS[$op]}_${FORMAT}_PTR NULL" >> $PTR_DEFINES_H_TEMP
done
echo '#endif /* '"IL_NO_$FORMAT"' */' >> $PTR_DEFINES_H_TEMP
# And now we output operations that are NULL no matter what
if test "x$nulls" != "x"
then
	echo >> $PTR_DEFINES_H_TEMP
	echo '/* These are always NULL */' >> $PTR_DEFINES_H_TEMP
	for op in $nulls
	do
		echo "#define ${OPERATIONS[$op]}_${FORMAT}_PTR NULL" >> $PTR_DEFINES_H_TEMP
	done
fi
echo >> $PTR_DEFINES_H_TEMP
echo '/* Finished examining '"$FORMAT"' */' >> $PTR_DEFINES_H_TEMP
echo >> $PTR_DEFINES_H_TEMP
defines=
nulls=
done
# End of the C++ compiler caretaking...
echo "#ifdef __cplusplus" >> $PTR_DEFINES_H_TEMP
echo '} /* end extern "C" */' >> $PTR_DEFINES_H_TEMP
echo '#endif /* __cplusplus*/' >> $PTR_DEFINES_H_TEMP
# End of the header guard...
echo '#endif /* '"$PTR_DEFINES_H_CODE"' */' >> $PTR_DEFINES_H_TEMP
echo >> $PTR_DEFINES_H_TEMP
# Now check whether the temp and possibly old PTR_DEFINES_H files are different.
# There is no reason to overwrite the old one if they are the same...
if cmp -s $PTR_DEFINES_H $PTR_DEFINES_H_TEMP
then
	# They are the same... 
	rm $PTR_DEFINES_H_TEMP
	echo "File $PTR_DEFINES_H didn't have to be replaced"
else
	# The new one is better :-) 
	mv $PTR_DEFINES_H_TEMP $PTR_DEFINES_H
	echo "File $PTR_DEFINES_H has been upadated to an up-to-date state"
fi
