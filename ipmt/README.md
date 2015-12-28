# ipmt
Indexed Pattern Matching Tool. Second text proccessing course project at CIn - UFPE.

Description
-----------

Provides search for patterns in text files using idexed representations of the text, such as Suffix Arrays and Suffix Trees. These indexed representations are compressed and decompressed using Lempel-Ziv-Welch algorithm.

Algorithms
----------

For the matching we use:
- Suffix Tree matching as default;
- Suffix Array matching as a option.

For compression/decompression we use Lempel-Ziv-Welch.

Usage
-----

``$ ipmt index textfile``
``$ ipmt [options] [pattern] textfile [textfile...]``


- Options:
  - ``-h, --help`` : shows help.
  - ``-p, --pattern file`` : makes the matching using each line in 'file' as a pattern.
  - ``-c, --count`` : only shows how many times the pattern matches.
  - ``-a, --array`` : uses a Suffix Array as the representation.

- We also offer support to wildcards at the textfile parameter.

Authors
-------

- George Oliveira - ghao@cin.ufpe.br
- Horácio Filho - hjcf@cin.ufpe.br
- Lucas Souza - lns2@cin.ufpe.br
