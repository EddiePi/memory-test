#!/bin/bash
GLIBC="/home/eddie/glibc-build"

GCONV_PATH=${GLIBC}/iconvdata LC_ALL=C     \
${GLIBC}/elf/ld-linux-x86-64.so.2 --library-path \
${GLIBC}:\
${GLIBC}/math:\
${GLIBC}/elf:\
${GLIBC}/dlfcn:\
${GLIBC}/nss:\
${GLIBC}/nis:\
${GLIBC}/rt:\
${GLIBC}/resolv:\
${GLIBC}/crypt:\
${GLIBC}/nptl:\
${GLIBC}/dfp \
"$@"
