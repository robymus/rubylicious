# How to compile ruby for rubylicious?

The project uses precompiled static libraries for ruby, both for linux and windows.

These are both produced on linux, with the windows version crosscompiled.

The below instructions are based on the official instruction at https://github.com/ruby/ruby#how-to-compile-and-install

The 'make install' phase is optional, as we are only interested in the header files and static library (.a)

## Preparation

Download release source code of ruby 2.7.0 from https://github.com/ruby/ruby/releases/tag/v2_7_0
Other versions might also work


## Compiling for linux

```
autoconf
./configure --disable-dln --with-static-linked-ext --prefix=/tmp/rubylicious-linux --target=x86_64-linux 
touch revision.h
make -j 8
make check
```

Note: due to some magical issue, revision.h doesn't exists and make doesn't create it empty, as it should.

Note: for built in modules to work, development package dependencies might be needed.

Then copy libruby-static.a and header files ruby.h, ruby/*.h, ruby/backward/*.h

## Compiling for win32

This was done on Fedora, might be different on others.

Install cross-compiler toolchain:

```
sudo dnf install mingw32-gcc mingw32-gcc-c++ mingw32-readline mingw32-openssl

```

Install the same version of ruby (eg. 2.7.0) on the host machine as being compiled.

Note: we disable gdbm, dbm, and fiddle modules, just in case.

```
autoconf
mingw32-configure --disable-dln --with-static-linked-ext --prefix=/tmp/rubylicious-win32 --target=i686-w32-mingw32 --with-out-exts=fiddle,gdbm,dbm
touch revision.h
make -j 8
```

Then copy libruby-static.a and header files ruby.h, ruby/*.h, ruby/backward/*.h

Note: I couldn't get the fiddle gem (libffi) and dbm gem working, but it's not necessary.