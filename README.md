Customized DMG Bitcoin Node
===========================

### Compile Instructions

> Bitcoin is a monolithic architecture, and it's build system is tightly coupled,
> read carefully and don't skip a step.
> Bitcoins compilation flags default to "-g -O2" and target c++11,
> customize our compilation options, strip out the debug symbols, resulting in a executable that is 90% smaller
> for deployment, and compile with the highest optimization flag that is still considered safe.
> NOTE: '-Ofast' is non-compliant via g++ standard program procedure, thus unsafe

```bash
# depends directory

$ cd depends

# bitcoin has a built-in Arch/OS specific configuration generator for the build process.
# Here we want to be as efficient as possible, run make with 6 jobs to speed up the compilation,
# NOTE: make -j if given no arguments and left to its own predilections will consume all cpu power
# and crash the machine (An unforgiving mistress indeed), 6 jobs is the recommended value.
# Ommit the QT framework, QR etc.
# This will build a bitcoin core binary for the machine it is being compiled on. 
# No cross platform bloat, or uneccessary libraries built in

$ make -j 6 NO_QT=1 NO_QR=1 NO_WALLET=1 NO_BDB=1

# This will generata a x86_64-pc-linux-gnu foler holding our custom compilation environment
# cd .. to the root and run autogen.sh to further configure dependencies

$ ./autogen.sh

# We need to point the conifigure script to our custom compilation folder
# the default flags compile with wallet support on and debug symbols on with -g -O2 and c++11
# Compile ours with optimization level 3, target c++ 17, and omit debug symbols, statically link the sql library on configure

# If in fish shell
$ ./configure CXXFLAGS="-O3 -std=c++17" LDFLAGS="-L/usr/lib/mysqlcppconn -L/usr/lib -lmysqlcppconn -lmysqlcppconn-static" --prefix=(pwd)/depends/x86_64-pc-linux-gnu/

# if bash or zsh etc
$ ./configure CXXFLAGS="-O3 -std=c++17" LDFLAGS="-L/usr/lib/mysqlcppconn -L/usr/lib -lmysqlcppconn -lmysqlcppconn-static" --prefix=`pwd`/depends/x86_64-pc-linux-gnu/

# now just make and make install

# NOTE: -j stands for the number of jobs, The rule of thumb is $(cpu_cores) * 1.5 give or take.
# WARNING: running the make -j option with no integer argument will consume all cpu and crash your machine
$ make -j 6 && sudo make install
```


Bitcoin Core integration/staging tree
-------------------------------------

https://bitcoincore.org

For an immediately usable, binary version of the Bitcoin Core software, see
https://bitcoincore.org/en/download/.

Further information about Bitcoin Core is available in the [doc folder](/doc).

What is Bitcoin?
----------------

Bitcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Bitcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Bitcoin Core is the name of open source
software which enables the use of this currency.

For more information read the original Bitcoin whitepaper.

License
-------

Bitcoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built (see `doc/build-*.md` for instructions) and tested, but it is not guaranteed to be
completely stable. [Tags](https://github.com/bitcoin/bitcoin/tags) are created
regularly from release branches to indicate new official, stable release versions of Bitcoin Core.

The https://github.com/bitcoin-core/gui repository is used exclusively for the
development of the GUI. Its master branch is identical in all monotree
repositories. Release branches and tags do not exist, so please do not fork
that repository unless it is for development reasons.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The CI (Continuous Integration) systems make sure that every pull request is built for Windows, Linux, and macOS,
and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Bitcoin Core's Transifex page](https://www.transifex.com/bitcoin/bitcoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.
