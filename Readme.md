# binhex

Convert any file into a c-header. Usefull if you have to put arbitary files 
into a small controller for a webserver project, static images for a intro 
or what ever.

# Output-formats (style)

- None, classic

    `const unsigned char bla[]`

- "avr", compile into flash

    `const uint8_t PROGMEM bla[]`

# Usage

```
$ ./binhex -f in.htm -p FILE1 
binhex v2.1, Juni 2009 - 2016, by Stephan Ruloff
converting in.html ...
#define FILE1_NAME "in.html"
#define FILE1_SIZE 7
#define FILE1_DATA in_html
const unsigned char in_html[] = {
0x3C, 0x68, 0x74, 0x6D, 0x6C, 0x3E, 0x0A
};

```

Pipe into a file
```
$ ./binhex -f in.htm -p FILE1 > bla.h
binhex v2.1, Juni 2009 - 2016, by Stephan Ruloff
converting in.html ...
```

Pipe into a file, with no "output"
```
$ ./binhex -f in.htm -p FILE1 -q > bla.h
```

# License

GPLv2
