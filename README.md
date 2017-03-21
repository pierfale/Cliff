# Cliff
LR(1) parser and syntax generator

Currently available:
* Basic compiler front-end (reader/lexer/parser) using LR(1) parsing
* Syntax generator: Use a defined syntax (in `syntax.bin`) to parse a new syntax definition file and generate a new `syntax.bin`

## Installation

Use CMake

## Usage

Generate syntax (require a `syntax.bin`)
`./Cliff -gs <syntax_filename>`

A bootstrapped version of syntax.bin with standard Extended Backus–Naur form syntax is provided in `data/ebnf_syntax.bin`




