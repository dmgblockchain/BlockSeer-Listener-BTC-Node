# CUSTOM BLOCKSEER NODE

## Installing

The Blockseer Custom Node is a fork of the bitcoin source code. The forked repo can be found at https://github.com/dmgblockchain/bitcoin.

After cloning the repository, make sure to check the file `bitcoin.conf` in the root directory. You can find the rpc user and password for connecting to the node and making rpc queries. Temporary username and password are included there but should not used in production. 

In `src/validation.cpp` on line 1047 you must add the mysql credentials for the blockseer label database. The node will not build without it. You can find the credentials on AWS Secret Manager.

### Local installation - Ignore this step if using Docker

The size of the bitcoin blockchain is constantly growing and at the time of writing it is about 300GB+, so ensure you have enough disk space before continuing. 

After cloning the repository, run `apt-get install build-essential autoconf libtool pkg-config libboost-all-dev libssl-dev libprotobuf-dev protobuf-compiler libevent-dev libqt4-dev libcanberra-gtk-module libdb-dev libdb++-dev bsdmainutils libmysqlcppconn-dev -y` to install the dependencies. 

From the root directory run the following 4 commands in order to build the bitcoin node.

    1) `./autogen.sh`
    2) `./configure`
    3) `make`
    4) `sudo make install`

After the final command has completed you will be able to start the bitcoin node by running `bitcoind` or `bitcoind --daemon` if you want it run in the background.

You can run RPC commands to test the connection by running `bitcoin-cli getinfo` or `bitcoin-cli getblockcount`.

If bitcoind or bitcoin-cli are not working after build and install, try `./src/bitcoind` or `./src/bitcoin-cli`.

### Docker Installation

To install the docker instance of the custom node, navigate to the root directory and run `docker-compose build` or `docker-compose build --no-cache`. This will take a few minutes. After it is complete you can run `docker-compose up` or `docker-compose up -d` to run the docker containter.

## Dependencies

In order to query the Blockseer DB the mysql C++ dependency must be added on line 590 in src/Makefile.am:
`bitcoind_LDFLAGS = $(bitcoin_bin_ldflags) $(MYSQL_LDFLAGS) -L/usr/lib/mysqlcppconn -L/usr/lib -lmysqlcppconn -lmysqlcppconn-static`  

## Validation

The validation of transactions is the core additional functionality to this bitcoin node. As the network attempts to add new transactions to the mempool, the Blockseer node, checks each address associated with that transaction, either as an input or an output, against the Blockseer label database. If any of those addresses exist in one of the risky categories, the entire transaction will not be accepted to the mempool. 

Validation is done in src/validation.cpp in the `AcceptSingleTransaction` function on line 1049. Credentials for the db connection can be found on AWS Secret Manager.

When a transaction is being added to the mempool, we first receive a reference to that transaction in the `AcceptSingleTransaction` function (variable named `ptx`). The hash of the transaction is grabbed and used to decode the entire transaction. The decoded transaction will be returned as a JSON value. Along with other data, the decoded transaction contains a `vin` key and a `vout` key, these are the inputs and the outputs.

The outputs have the addresses directly in the value of `vout` (`["vout"][i]["scriptPubKey"]["addresses"]`) and can be used to look up the addresses directly.

The input addresses are not as obvious. Instead for each input in `vin` there will be a transaction id (`txid`) and a output number (`vout` - different than the `vout` above). This refers to the previous transaction output that led to this transaction input. In order to find the input addresses, we need to decode the `txid` found in `vin` and check the outputs from that transaction. There could be a number of outputs in the previous transaction and the `vout` number will tell you which element in the previous transaction output array relates to the input of the transaction we are trying to validate. After finding that output, you will discover the addresses.

All the addresses are added to an array and used to search the Blockseer label db. If they are considered risky we `return false;` and block the transaction from entering our mempool.

An example of a decoded transaction can be seen here: https://chainquery.com/bitcoin-cli/decoderawtransaction



Bitcoin Core integration/staging tree
=====================================

https://bitcoincore.org

What is Bitcoin?
----------------

Bitcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Bitcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Bitcoin Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately usable, binary version of
the Bitcoin Core software, see https://bitcoincore.org/en/download/, or read the
[original whitepaper](https://bitcoincore.org/bitcoin.pdf).

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
in Python, that are run automatically on the build server.
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

Translators should also subscribe to the [mailing list](https://groups.google.com/forum/#!forum/bitcoin-translators).
