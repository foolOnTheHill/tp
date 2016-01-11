# ipmt
Indexed Pattern Matching Tool. Second text proccessing course project at CIn - UFPE.

Description
-----------

Provides search for patterns in text files using idexed representations of the text, such as Suffix Arrays and Suffix Trees. These indexed representations are compressed and decompressed using Lempel-Ziv-Welch algorithm or LZ77.

Algorithms
----------

For the matching we use:
- Suffix Array matching as default;
- Suffix Tree matching as an option.

For compression/decompression we use:
- Lempel-Ziv-Welch as default;
- LZ77 as an option.

Usage
-----

Indexation mode: ``$ ipmt index [options] textfile``

- Options:
  - ``-a`` : uses a Suffix Array as the representation.
  - ``-t`` : uses a Suffix Array as the representation.
  - ``-compress=lz77`` : uses LZ77 to compress the index file.
  - ``-compress=lzw`` : uses LZW to compress the index file.

Search mode: ``$ ipmt search [options] [pattern] textfile [textfile...]``

- Options:
  - ``-p file`` : makes the matching using each line in 'file' as a pattern.
  - ``-c`` : only shows how many times the pattern matches.

- ``-h`` : shows help.

- We also offer support to wildcards at the textfile parameter.

Authors
-------

- George Oliveira - ghao@cin.ufpe.br
- Horácio Filho - hjcf@cin.ufpe.br
- Lucas Souza - lns2@cin.ufpe.br
