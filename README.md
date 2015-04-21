Release notes 0.1

# cover2tiling

**Cover2Tiling** converts NGS data (from a sorted BAM file) to TilingScan input format.

Requirements:

- A minimum knowledge of using the Linux/Windows terminal.
- **SAMTOOLS latest version** (important!). You can download it and install here: http://www.htslib.org/download/
- **Cover2Tiling binary**. You can compile it from the source (see below), or download the latest release from https://github.com/TilingScan/cover2tiling/releases



## Compilation

### Linux

First you must download and install the **C++** compiler. You can do this opening a new terminal and executing:
```
sudo apt-get install g++
```

Now, 'cd' to the cover2tiling directory containing the source and type 

```
g++ cover2tiling.cpp -o cover2tiling 
```

This will generate an executable for **cover2tiling**.


### Windows

You can compile **cover2tiling** using your favorite IDE, such as **DevC++**. 



## How to use

First, you must generate the coverage from your BAM file with the data (**note that your BAM must be sorted**). For this, open a new terminal and use the SAMTOOLS *mpileup* command:

```
samtools mpileup your_bam_file.bam > cover.txt
```

This will generate you a *.txt* file with the coverage. Now, to use the **cover2tiling** to generate the TilingScan format from the cover file, execute:

```
./cover2tiling cover.txt output.txt **N**
```

Where:
- **cover.txt** is the output file of samtools mpileup comand.
- **output.txt** is the output file to be used in TilingScan.
- **N** is the average nucleotides for each hit. It must be odd.

That's all! Now you can use the output file generated in **TilingScan**.



## Get help

If you have any question, or want a personalized version of **cover2tiling**, feel free to contact with us:

V.Arnau: Vicente.Arnau@uv.es 
J.M.Juanes: jmjuanes@outlook.com 




