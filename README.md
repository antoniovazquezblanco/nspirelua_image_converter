nSpire Lua image converter
==========================

This tool is able to convert png images to ASCII code that can be inserted into nSpire Lua programs.

Usage
-----

The following command should output a string that you can directly insert as an string encoding of a image.
```bash
./imgnspire <png file>
```

If you are using Linux and have xclip installed you can also do:
```bash
./imgnspire <png file> | xclip -i
```
