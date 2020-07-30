FROM ubuntu:18.04

RUN apt-get update 
RUN apt-get install build-essential autoconf libtool pkg-config libboost-all-dev libssl-dev libprotobuf-dev protobuf-compiler libevent-dev libqt4-dev libcanberra-gtk-module libdb-dev libdb++-dev bsdmainutils libmysqlcppconn-dev -y
WORKDIR /src/
COPY ./ .
COPY ./bitcoin.conf /root/.bitcoin/bitcoin.conf
# COPY ./bitcoin/ /src/.bitcoin/
EXPOSE 8333
EXPOSE 8332
RUN ./autogen.sh
RUN ./configure --with-incompatible-bdb
RUN make
RUN make install
#RUN mkdir -p /root/bitcoind-simnet/
RUN ls -al ./src/
#CMD ["ls", "./src/"]
CMD ["./src/bitcoind"]
