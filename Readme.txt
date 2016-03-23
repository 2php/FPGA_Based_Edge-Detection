Best run on IL2212 VM

The following directories in the app directory contain templates to be run on the DE2 Board:
1) hello_mpsoc - template for multi-core applications
2) hello_ucosii - template for single-core applications running uC/OS-II
3) hello_world - template for single-core bare metal applications
4) image_proc_demo - Another template for single-core applications running uC/OS-II

Use the .sh files in each template to download code.

The app/src directory contains the actual code developed in the project. Just copy-paste the contents into the templates  and run the .sh files to use them.

Do not make changes to the hardware or bsp directories. They contain VHDL files describing the underlying hardware.