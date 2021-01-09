Hello!

This is a project that hits the magic intersection of small / interesting so
well, that I feel compelled to share on the interwebs.

This is an implementation of a number map, which takes an input number and
returns an output number of the same byte size.  For uint16_t byte size and
above an array lookup implementation will occupy 65K+ of space.  Typically much 
less is needed as maps are fairly sparse.  This project's genesis was from
working on TrueType fonts: The character mappings need to convert from a
character encoding to a glyph index.

There are a couple of interesting things about this project:

1. numap_compile16 will assign machine code from the input map to memory.  If
the permissions are set correctly, this can then be jmp'd into and executed. 
An additionally input return jmp address will return execution.
This is self modifying code, which I personally find fascinating.

2. the big O analysis for numap_map16, is O(n-) as it will break out early if
it finds the mapping and the numap_compile16 is O(n) as it always searches the
entire map, however due to very technical details of CPU processing the compile
method ends up being faster.

Format for a numap is:

uint{8,16,32,64}_t [] = { N, S0, C0, S1, C1, ..., SN-1, CN-1, SN }

Where N is the number of partitions in the map.
C0, ... CN-1 are the number of cuts
S0, ... SN are the output mappings.

For example: uint8_t is_numap[] = { 3, 0, '0', 1, '9' + 1, 0 }
  [0  , '0') -> 0
  ['0', '9'] -> 1
  ('9', UINT8_MAX] -> 0

test1 contains a fairly trivial toupper implementation to demonstrate the
method and speedup.  On my setup it shows a 2x improvement in speed.

test2 contains a more involved character mapping for a font called "Anonymous
Pro Minus.ttf" designed by Mark Simonson.  On my setup it shows a 5x
improvement in speed.

Released with a ISC license.

Freedom.
