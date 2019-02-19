# Parametric CRC in C
Something that came about after being somewhat anoyed at the various CRC "CCITT" implementations and otherwise generally hard-coded ("not the one I want") variants that are out there. This is a fully parametric implementation that allows any type of CRC to be produced, while generating no unnecessary code.

# Features
* Variable length (8-, 16-, 32- and 64 bit)
* Arbitrary polynomial
* Table- or non-table driven
  * Auto-generated table or user-provided
* Reflected- or non-reflected input
* Reflected- or non-reflected output
* Code-size depends on set parameters; no unnecessary code is generated

# Credit
"CRC series" found at <https://barrgroup.com/>. Good starting point and general intro.
For all your bit-twiddling needs <https://graphics.stanford.edu/~seander/bithacks.html>
