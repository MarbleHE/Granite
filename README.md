# Granite

Granite is a framework containing libraries to support converting plaintext code into homomorphic encrypted computations with only little modification and knowledge of homomorphic encryption. It supports many different libraries by using SHEEP as a backend.

## High level design
![High level design](hld.jpg?raw=true "High level design")

Marble contains the main interface, which the user will use to write the plaintext computation. Its job is to step through the code and build an AST.

The middle part, Wool, will take that AST and transform it to a SHEEP circuit.

SHEEP will then be used to evaluate those circuits on any of its supported library.

### Marble

### Wool

### SHEEP

## Installation


