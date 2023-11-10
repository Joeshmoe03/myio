Authors: Josef Liem and Hedavam Solano

Known bugs: none identified

References: 
- https://stackoverflow.com/questions/28533553/what-is-the-default-mode-for-open-calls-with-o-creat-and-how-to-properly-set-i
- https://stackoverflow.com/questions/17209087/i-wanna-know-the-internal-members-of-struct-file-the-latest-ones

Testing: do "make" and run "./myio". You will see a writefile0.txt with a write test result. readfile0.txt and readfile1.txt are dependencies for this test. 
