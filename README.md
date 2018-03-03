# NINI

This parser aims to be the smallest possible while supporting a sufficient subset of the INI format.
The parser is meant for use in embedded applications.

When compiled for a bare metal Cortex-M0, **the whole parser fits in 336 bytes** of Flash and 30 (+buffers) bytes of RAM.

## Features

- Basic INI format support
- Accepts both Unix and DOS newlines
- Minimal memory footprint
- The input file can be loaded in multiple pieces of any size.
- Buffer sizes (section, key, value) can be configured in the header file.
- Custom data `void *` for maintaining user context, passed to the callback

## Sypported syntax

Any whitespace, except inside a value, is discarded.

- **Sections** - `[section.name]`
- **Key-value pairs** - `key.foo-bar_baz123 = value lorem ipsum 123`
  - Value can contain whitespace, leading and trailing whitespace is removed.
  - Ends with either `\r` or `\n`
- **Comment** `# comment ...`
  - Ends with either `\r` or `\n`

## Limitations

- Not re-entrant, uses a static state variable
- No checks for invalid syntax
- Whitespace inside keys and section names is removed
- Quoted strings and escape sequences are not supported, will be collected as plain text
- Value can't be followed by an inline comment

See the file `test.c` for an example of the most basic usage; see the header file for more details on the API.
