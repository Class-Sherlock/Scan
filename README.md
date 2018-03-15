# Scan
scan.sh that takes two arguments:
1. suffix – a string
2. N – a positive integer
Shell script will recursively scan the current directory for all files ending with the given suffix, then sort the files by their size, and then print the the filenames of the largest N files to the standard output, followed by the sum of their sizes. Each file reported is followed by the size of the file in bytes. If the total number of files found is less then N, report all files. The listed files will be sorted by the size, in descending order.
