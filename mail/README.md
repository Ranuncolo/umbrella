# email file

How this file works and how to compile it

## Requirements

Two libraries are required:
1. libcurl
2. quickmail

They are both free and if not installed they are available in github.

## How to compile
To compile it is required to call not only the compiler but also the libraries needed. 

`gcc mail.c -lcurl -lquickmail`
