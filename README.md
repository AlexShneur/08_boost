# 08_boost
 Utility to search duplicate files using boost

#Parameters description:

--help = Show help message.

--include    = Directories to scan.

--exclude    = Directories to exclude from scanning.

--level      = Scanning level (1 - all directories, 0 - only chosen directory without subdirs). 

--min_size 	 = Minimum file size(checks all files more then 1 byte by default).

--masks    	 = File name masks allowed for comparison.

--block_size = File read block size.

--hash		 = Hash algorithm (crc16, crc32, ccitt, xmodem).


#Usage:
--include \test_objects\test --exclude \test_objects\test\folder2 --level 1 -- min_size 1 --masks *\.txt --block_size 5 --hash crc16
