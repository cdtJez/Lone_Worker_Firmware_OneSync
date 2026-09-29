# buildinc.awk
#
# Increment the build number in a file called inc\buildnum.h
#
# Looks for line starting "#define BUILDNUM" then increments the next element
#

BEGIN {
	FS = " "															# Set field separator to a space 
	i = 0																	# Line counter
	while ((getline line < "inc/buildnum.h") > 0)
	{
		lines[i++] = line
	}

	close ("inc/buildnum.h")							# File read into array so close

	j = 0
	while (j != i)
	{
		split (lines[j], words)							# Split line into words

# Test for line starting '#define BUILDNUM'
		if ((words[1] == "#define") && (words[2] == "BUILDNUM"))
		{
			build_number = words[3]
			print "#define BUILDNUM " build_number+1 > "inc/buildnum.h"
		}
		else																# Otherwise just print the line
		{
			print lines[j] > "inc/buildnum.h"
		}
		j++
	}

# Print build number so Makefile can capture it if necessary
	print build_number
}


