# Hw1: My Kernel Module
* contributed by < `詹子毅 F04066028` >  
* course: [NCKU Operating Systems 2021](http://class-qry.acad.ncku.edu.tw/syllabus/online_display.php?syear=0110&sem=1&co_no=F720800&class_code=)  
* course instructor: Prof. 張大緯 Chang, Ta-Wei  

## Configuration
* environment: Oracle VM VirtualBox  
* operating system: Ubuntu 18.04.3 LTS  
* Linux version: 4.15.0-72-generic  

## Build & Run
Clone the repository to your local machine and `cd` into the repo  
```
$ cd hw1-tzuyichan
```
Compile `app` (the user interface) and `my_info` (the kernel module)  
```
$ make                // app
$ cd module
$ make                // my_info
```
Insert `my_info` into the Linux kernel (root privileges required)  
```
$ sudo insmod my_info.ko
```
Run `app` with  
```
$ ./../app
```
To remove `my_info` from the Linux kernel, use  
```
$ sudo rmmod my_info
```

## Sample Output
```
$ ./app

Which information do you want?
Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?
v

Version: Linux version 4.15.0-72-generic

------------------------------------------------------------

Which information do you want?
Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?
t

Time information:
Uptime		: 6224.31 (s)
Idletime	: 10951.40 (s)

------------------------------------------------------------

Which information do you want?
Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?
a

=============Version=============
Linux version 4.15.0-72-generic

=============CPU=============
processor	: 0
model name	: Intel(R) Core(TM) i5-7360U CPU @ 2.30GHz
physical id	: 0
core id		: 0
cpu cores	: 2
cache size	: 4096 KB
clflush size	: 64
cache_alignment : 64
address sizes	: 39 bits physical, 48 bits virtual

processor	: 1
model name	: Intel(R) Core(TM) i5-7360U CPU @ 2.30GHz
physical id	: 0
core id		: 1
cpu cores	: 2
cache size	: 4096 KB
clflush size	: 64
cache_alignment : 64
address sizes	: 39 bits physical, 48 bits virtual

=============Memory=============
MemTotal    : 1009889 kB
MemFree     : 659792 kB
Buffers     : 12930 kB
Active      : 226419 kB
Inactive    : 80122 kB
Shmem       : 4645 kB
Dirty       : 8 kB
Writeback   : 0 kB
KernelStack : 0 kB
PageTables  : 0 kB

=============Time=============
Uptime      : 6226.94 (s)
Idletime    : 10955.88 (s)

------------------------------------------------------------

Which information do you want?
Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?
e
```
