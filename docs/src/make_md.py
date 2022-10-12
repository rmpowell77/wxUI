#!/usr/bin/env python3

import fileinput
import sys
import math
import re

"""
make_md is intended to pull the code examples from actual working code into the
documentation in a consistent and easy manner.

The idea is that you would process the docs with this command.  When a line is
encountered that has the form:

{{{ <File> <SnippetName> [Delim] }}}

Then it would open the file and parse select the snippets with that name and
insert with the optional deliminator between them.

So for instance:
{{{ examples/HelloWorld/HelloWorld.cpp Example "    // ..." }}}
would open HelloWorld.cpp and look for these lines:

// snippet Example 
// endsnippet

And grab everything in between, and then print them out with the delimantor.

"""

def get_sentences(lines):
  activeParses = {}
  for line in lines:
    d1 = re.match(".*//\s*snippet\s+(\w+)", line)
    d2 = re.match(".*//\s*endsnippet\s+(\w+)", line)
    if d1:
      activeParses[d1.group(1)] = []
    elif d2:
      yield { d2.group(1): activeParses[d2.group(1)] }
      activeParses.pop(d2.group(1), None)
    else:
      for (_,v) in activeParses.items():
        v.append(line)

def extractSnippets(input):
  """ returns a dictionary of all the snippets in the lines
  >>> extractSnippets("hello")
  {}
  >>> extractSnippets( "// snippet one\\nstuff\\n  // endsnippet one\\n".splitlines() )
  {'one': [['stuff']]}
  >>> extractSnippets( "// snippet one\\nstuff\\n// endsnippet one\\n\\nmore lines\\n".splitlines() )
  {'one': [['stuff']]}
  >>> extractSnippets( "// snippet one\\nstuff\\nmore stuff\\n// endsnippet  one \\n".splitlines() )
  {'one': [['stuff', 'more stuff']]}
  >>> extractSnippets( "// snippet one\\nstuff\\n// endsnippet one\\n// snippet one\\nmore stuff\\n// endsnippet one\\n".splitlines() )
  {'one': [['stuff'], ['more stuff']]}
  >>> extractSnippets( "// snippet one\\nstuff\\n// endsnippet one\\n// snippet two\\nmore stuff\\n// endsnippet two\\n".splitlines() )
  {'one': [['stuff']], 'two': [['more stuff']]}
  >>> extractSnippets( "// snippet one\\nstuff\\n// snippet two\\nmore stuff\\n// endsnippet two\\nmore stuff2\\n// endsnippet one".splitlines() )
  {'two': [['more stuff']], 'one': [['stuff', 'more stuff', 'more stuff2']]}
  >>> extractSnippets( "// snippet one\\nstuff\\n// snippet two\\nmore stuff\\n// endsnippet one\\nmore stuff2\\n// endsnippet two".splitlines() )
  {'one': [['stuff', 'more stuff']], 'two': [['more stuff', 'more stuff2']]}
  """
  result = {}
  for i in get_sentences(input):
    for k,v in i.items():
      if not k in result:
        result[k] = []
      result[k].append(v)
  return result


if __name__ == "__main__":
  import doctest
  doctest.testmod()
  database = {}
  for line in fileinput.input():
    m = re.match("\{\{\{\s*(.+)\s+(\w+)\s+\"(.*)\"\s*\}\}\}", line)
    if m:
      file = m.group(1)
      if not file in database:
        database[file] = extractSnippets(open(file).readlines())
      firsttime = 1
      for snippets in database[file][m.group(2)]:
        if firsttime:
          firsttime = 0
        else:
          print(m.group(3))
        for line in snippets:
          print(line, end='')
      continue
    print(line, end='', file=sys.stdout)
    #printInputTo(fileinput.input(), sys.stdout)
