#!/bin/sh

BINUTIL_PKG=binutils-2.22.tar.gz
BINUTIL_DIR=`basename $BINUTIL_PKG .tar.gz`
GMP_PKG=gmp-5.0.4.tar.bz2
GMP_DIR=`basename $GMP_PKG .tar.bz2`
MPFR_PKG=mpfr-3.1.0.tar.gz
MPFR_DIR=`basename $MPFR_PKG .tar.gz`
MPC_PKG=mpc-0.9.tar.gz
MPC_DIR=`basename $MPC_PKG .tar.gz`
GCC_PKG=gcc-4.6.2.tar.gz
GCC_DIR=`basename $GCC_PKG .tar.gz`
LIBC_PKG=avr-libc-1.8.0.tar.bz2
LIBC_DIR=`basename $LIBC_PKG .tar.bz2`

TARGET=avr

# set bin dir.
echo start devtool build
cd `dirname $0`
DEVTOOLDIR=`pwd`
echo DEVTOOLDIR=$DEVTOOLDIR
PATH=$DEVTOOLDIR/bin:$PATH
export PATH

LD_RUN_PATH=$DEVTOOLDIR/lib
export LD_RUN_PATH

SRCDIR=`pwd`/src
echo SRCDIR=$SRCDIR

WORKDIR=`pwd`/obj
echo WORKDIR=$WORKDIR

echo BINUTIL_PKG=$BINUTIL_PKG
echo BINUTIL_DIR=$BINUTIL_DIR
echo GMP_PKG=$GMP_PKG
echo GMP_DIR=$GMP_DIR
echo MPFR_PKG=$MPFR_PKG
echo MPFR_DIR=$MPFR_DIR
echo MPC_PKG=$MPC_PKG
echo MPC_DIR=$MPC_DIR
echo GCC_PKG=$GCC_PKG
echo GCC_DIR=$GCC_DIR
echo LIBC_PKG=$LIBC_PKG
echo LIBC_DIR=$LIBC_DIR

# unpack
if [ ! -d $SRCDIR ]; then
  echo get packages

  if [ ! -r $BINUTIL_PKG ]; then
    wget http://ftp.tsukuba.wide.ad.jp/software/binutils/binutils-2.22.tar.gz
  fi
  if [ ! -r $GMP_PKG ]; then
    wget http://ftp.tsukuba.wide.ad.jp/software/gmp/gmp-5.0.4.tar.bz2
  fi
  if [ ! -r $MPFR_PKG ]; then
    wget http://ftp.tsukuba.wide.ad.jp/software/mpfr/mpfr-3.1.0.tar.gz
  fi
  if [ ! -r $MPC_PKG ]; then
    wget http://ftp.tsukuba.wide.ad.jp/software/mpc/mpc-0.9.tar.gz
  fi
  if [ ! -r $GCC_PKG ]; then
    wget http://ftp.tsukuba.wide.ad.jp/software/gcc/releases/gcc-4.6.2/gcc-4.6.2.tar.gz
  fi
  if [ ! -r $LIBC_PKG ]; then
    wget http://download.savannah.gnu.org/releases/avr-libc/avr-libc-1.8.0.tar.bz2
  fi

  echo check packages
  md5sum --check md5 || exit

  echo Unpacking packages
  mkdir -p $SRCDIR
  cd $SRCDIR

  tar xf $DEVTOOLDIR/$BINUTIL_PKG
  tar xf $DEVTOOLDIR/$GMP_PKG
  tar xf $DEVTOOLDIR/$MPFR_PKG
  tar xf $DEVTOOLDIR/$MPC_PKG
  tar xf $DEVTOOLDIR/$GMP_PKG
  tar xf $DEVTOOLDIR/$GCC_PKG
  tar xf $DEVTOOLDIR/$LIBC_PKG
fi

echo Start build
rm -rf $WORKDIR
mkdir -p $WORKDIR

# build binutils
mkdir -p $WORKDIR/$BINUTIL_DIR
cd $WORKDIR/$BINUTIL_DIR

$SRCDIR/$BINUTIL_DIR/configure \
  --prefix=$DEVTOOLDIR \
  --target=$TARGET \
  --disable-nls \
  || exit

make || exit
make install || exit

# build gmp

mkdir -p $WORKDIR/$GMP_DIR
cd $WORKDIR/$GMP_DIR

$SRCDIR/$GMP_DIR/configure \
  --prefix=$DEVTOOLDIR \
  || exit

make || exit
make check || exit
make install || exit

# build fpfr

mkdir -p $WORKDIR/$MPFR_DIR
cd $WORKDIR/$MPFR_DIR

$SRCDIR/$MPFR_DIR/configure \
  --prefix=$DEVTOOLDIR \
  --with-gmp=$DEVTOOLDIR \
  || exit

make || exit
make check || exit
make install || exit

# build mpc

mkdir -p $WORKDIR/$MPC_DIR
cd $WORKDIR/$MPC_DIR

$SRCDIR/$MPC_DIR/configure \
  --prefix=$DEVTOOLDIR \
  --with-gmp=$DEVTOOLDIR \
  || exit

make || exit
make check || exit
make install || exit

# build gcc

mkdir -p $WORKDIR/$GCC_DIR
cd $WORKDIR/$GCC_DIR

$SRCDIR/$GCC_DIR/configure \
  --prefix=$DEVTOOLDIR \
  --with-mpfr=$DEVTOOLDIR \
  --with-gmp=$DEVTOOLDIR \
  --target=$TARGET \
  --enable-languages=c \
  --enable-multilib \
  --disable-nls \
  --disable-zlib \
  || exit

make || exit
make check || exit
make install || exit

# build gcc

mkdir -p $WORKDIR/$LIBC_DIR
cd $WORKDIR/$LIBC_DIR

$SRCDIR/$LIBC_DIR/configure \
  --prefix=$DEVTOOLDIR \
  --host=$TARGET \
  || exit

make || exit
make check || exit
make install || exit

# clean up

echo Build complete
echo clean up
rm -rf $WORKDIR

