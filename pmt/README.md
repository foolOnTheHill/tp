# pmt
Pattern Matching Tool. First text proccessing course project at CIn - UFPE.

Description
-----------

Provides search for patterns in text files, returning every line and position where a match occurs. It also supports approximate matching.

Algorithms
----------

For exact matching we use:
- Boyer-Moore (to match only one pattern);
- Aho-Corasick (to match multiple patterns at once).

For approximate matching we use:
- Wu-Manber;
- or Sellers (depending on the maximum edit distance provided by the user and the pattern length).

Usage
-----

``$ pmt [options] [pattern] textfile1 [textfile2...]``

- Options:
  - ``-h, --help`` : shows help.
  - ``-e, --edit d`` : makes an approximate matching using 'd' as maximum distance.
  - ``-p, --pattern file`` : makes the matching using each line in 'file' as a pattern.
  - ``-c`` : only shows how many times the pattern matches in each line.

- We also offer support to wildcards at the textfile parameter.

- The output has the following format for each match: ``textfile:line:col: pattern``

Authors
-------

- George Oliveira - ghao@cin.ufpe.br
- Horácio Filho - hjcf@cin.ufpe.br
- Lucas Souza - lns2@cin.ufpe.br
