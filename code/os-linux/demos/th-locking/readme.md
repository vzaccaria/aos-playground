
# Introduction

This demonstration is aimed at showing concurrency issues in the kernel when creating multiple kthreads sharing a variable. 

It is an educational example in two parts: the "Broken" section demonstrates concurrency issues, such as race conditions, when manipulating a shared variable without proper synchronization, and the "Fixed" section shows how to address these issues using atomic operations. 

This is a useful illustration of the importance of proper synchronization in kernel-level code to prevent data corruption and unpredictable behavior in concurrent scenarios.
