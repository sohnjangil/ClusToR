# ClusTo-RC (Clustering Tool for Read and Contigs)

This is a tool for clustering sequencing reads and assembled scaffolds (or contigs)
to groups of reference genome's choromosomes (or scaffolds). 

This tool can help to assembly more accurate and long scaffolds by reducing topological complexity in each group.

NOTE
----

The species of reference genome must be same or at least very closely related one.

Requirement
-----------

c++11 (or later): Required to compile

lastz (https://github.com/lastz/lastz.git)

lastz_analysis (https://github.com/sohnjangil/lastz_analysis.git)

samtools (https://github.com/samtools/samtools.git)

BWA (https://github.com/lh3/bwa.git)

Alternatively, BOWTIE (for short-reads), NGMLR, or BLASR (for long-reads) can be used.

Install
-------

Just do
```
make
```
You can find executable files in bin directory.

If you want, add a line in ~/.bashrc (or .bash_profile)
```
export PATH=/path-to/SRATOR/bin:$PATH
```

How to use it
-------------

0) Prepare

Assembled scaffolds (or contigs)

Reference genome (same or closely related species' genome)

Sequencing reads (Mate-pair (MP) or long-read (such as PacBio))

1) Mapping reads to scaffolds

Example
```
bwa index scaffolds.fa
bwa mem scaffolds.fa MP_1.fq MP_2.fq > MP.sam
bwa mem -x pacbio scaffolds.fa PacBio.fa(or .fq) > PacBio.sam
```
You can use bowtie if only short-reads are used.
You may use other mapper such as ngmlr, blasr, or others generating sam files.
Though .sam file(s) is generated in this example, you may convert it to .bam file(s) using samtools.

Note: We do not recommand to sort .bam.

2) Mapping scaffolds to genome

Example
```
lastz genome.fa scaffolds.fa[multiple] --format=general --ambigous=iupac --step=10000 > scaffolds.lastz
```
You can modify options or use other options, but we recommand do not use "--chain" or "--gapped" options, by which options some scaffolds (or contigs) can be missed in lastz mapping.

If there are lots of repeats in assembled scaffolds or reference genome, lastz mapping can take too much time. In the case, you can do in multi-thread-like manner by dividing a scaffold file to several files as follows.

If you want to divide scaffolds.fa to 30 files, do
```
fasta_divider  scaffolds.fa  30
```
It will divide scaffolds.fa to scaffolds.fa.1, scaffolds.fa.2,...,scaffols.fa.30
Then, you can map each file to genome using a simple script.

Example of a shell-script file
```
option="--format=general --ambigous=iupac --step=10000"
for i in {1..30}
do 
   lastz genome.fa scaffolds.fa.${i}[multiple] ${option} > scaffolds.lastz.${i} 2> scaffolds.err.${i} &
done
wait
cat scaffolds.lastz.* > scaffolds.lastz
```

3) Generate ".map" files

Sequencing-reads to scaffolds map file

Example
```
gen_read_scaf_map  MP.sam (or MP.bam)  >  MP2scaf.map
gen_read_scaf_map  PacBio.sam (or PacBio.bam)  >  PacBio2scaf.map
```

Scaffolds to reference map file

Example
```
lastz_analysis -i scaffolds.lastz
find_major_target  scaffolds.lastz.map2target > scaf2target.map
```

4) Make target list file

Make a target list file as follows.

If there are chromosomes 1-12, X, and MT, then 
```
echo 1 2 3 4 5 6 7 8 9 10 11 12 X MT > target.list
```
Choromosome numbers can be separated by either space, tab, or line.

4) Split scaffolds to each group
Example
```
split_scaffold  scaffolds.fasta  scaf2target.map  target.list  scaf2target
```
then you can see files
```
scaf2target.1.fasta
scaf2target.2.fasta
scaf2target.3.fasta
.
.
.
scaf2target.12.fasta
scaf2target.X.fasta
scaf2target.MT.fasta
scaf2target.unmapped.fasta
```

5) Split sequencing-reads to each group

Example
```
split_fastq MP_1.fastq MP2scaf.map  MP2target.map  target.list  MP2target_1
split_fastq MP_2.fastq MP2scaf.map  MP2target.map  target.list  MP2target_2
split_fastq PacBio.fastq PacBio2scaf.map  PacBio2target.map  target.list  PacBio2target
```
Then, you can see files, in case of MP_1,
```
MP2target_1.1.fastq
MP2target_1.2.fastq
.
.
.
MP2target_1.X.fastq
MP2target_1.MP.fastq
MP2target_1.unmapped.fastq
```

If PacBio reads are written in .fasta format, you can use split_fasta.
Example
```
split_fasta PacBio.fasta PacBio2scaf.map  PacBio2target.map  target.list  PacBio2target
```

6) Collecting split-files into each group

Example
```
for i in $(target.list) ; do mkdir ; done
for i in $(target.list) ; do mv *.${i}.fasta *.${i}.fastq ${i} ; done
```

7) Then you can perform scaffolding or gap-filling in each group using any program or tools you want.


Contact
-------

Jang-il Sohn

sohnjangil@gmail.com

Bioinformatics and Genomics Lab.

Hanyang University
Seoul
Korea
