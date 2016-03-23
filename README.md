# FPGA_Based_Edge-Detection
Implementation of Sobel-based edge detection on the Altera DE2 development board. The algorithm  was implemented and analyzed in terms of throughput and memory footprint in the following 3 environments: 
1) Bare metal program running on a single Nios-2 core. 
2) µC/OS-II on a single Nios-2 core. 
3) Bare metal program running on a multi-core Nios-2 platform consisting of 5 CPUs.  

The information about the edges was used to convert the RGB images into ASCII images.The objective of the project was to get maximum throughput while maintaining a memory footprint as small as possible and compare the performance of the algorithm in the above-mentioned environments. 

Best run on IL2212 VM provided by KTH.  The following directories in the app directory contain templates to be run on the DE2 Board: 
1) hello_mpsoc - template for multi-core applications 
2) hello_ucosii - template for single-core applications running uC/OS-II 
3) hello_world - template for single-core bare metal applications 
4) image_proc_demo - Another template for single-core applications running uC/OS-II  

Use the .sh files in each template to download code.  The app/src directory contains the actual code developed in the project. Just copy-paste the contents into the templates  and run the .sh files to use them.  Do not make changes to the hardware or bsp directories. They contain VHDL files describing the underlying hardware. 
Feel free to contact me if you need help in adapting these algorithms into your projects or for running it without the VM.
