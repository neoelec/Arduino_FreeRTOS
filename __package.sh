#!/bin/bash
# SPDX-License-Identifier: GPL-2.0+
# Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)

prj_dir=$(dirname $(realpath $0))

ZIP=${1}
ARCH=${2}

heap_file=heap_4.c

if [ "${ARCH}" == "AVR" ]; then
    heap_file=heap_3.c
fi

orig_dir=$(basename ${ZIP} .zip)
version=$(basename ${ZIP} .zip | perl -pe 's/^.+V(\d+\.\d+\.\d+.*)$/$1/')
pkg_dir=out/Arduino_FreeRTOS-${ARCH}-v${version}
src_dir=${pkg_dir}/src
lib_dir=${src_dir}/lib/FreeRTOS-Kernel-v${version}
portable_dir=${src_dir}/portable

function __prepare_directories()
{
    rm -rf ${pkg_dir}
    mkdir -p ${lib_dir}
    mkdir -p ${portable_dir}
}

function __copy_freertos_source()
{
    unzip ${ZIP} > /dev/null
    if [ ! -d ${orig_dir} ]; then
        mv $(ls -1d FreeRTOS-Kernel* | sed -e '/.zip$/d') ${orig_dir}
    fi

    cp ${orig_dir}/*.c ${lib_dir}/
    cp ${orig_dir}/History.txt ${lib_dir}/
    cp ${orig_dir}/LICENSE.md ${lib_dir}/

    cp ${orig_dir}/include/*.h ${lib_dir}/
    cp ${orig_dir}/include/stdint.readme ${lib_dir}/

    mkdir -p ${portable_dir}/MemMang
    cp ${orig_dir}/portable/MemMang/${heap_file} ${portable_dir}/MemMang/

    rm -rf ${orig_dir}
}

function __extract_port()
{
    pushd ${pkg_dir} > /dev/null
    tar xf ${prj_dir}/Arduino_FreeRTOS-common.tar.bz2
    tar xf ${prj_dir}/Arduino_FreeRTOS-${ARCH}.tar.bz2
    popd > /dev/null
}

function __pach_rcn()
{
    find ${pkg_dir} -type f -exec fromdos {} \;
    perl -pi -e 's/(#include ")(portmacro.h")/$1..\/..\/portable\/<freertos_arch>\/$2/' \
            ${lib_dir}/portable.h
    perl -pi -e 's/(#include ")(FreeRTOS.h")/$1..\/..\/Arduino_$2/' \
            ${portable_dir}/MemMang/${heap_file}
    perl -pi -e 's/^#include "task\.h"\n//' \
            ${portable_dir}/MemMang/${heap_file}

    pushd ${pkg_dir} > /dev/null
    find -type f -exec sed -i \
            -e "s/<freertos_version>/${version}/g" \
            -e "s/<freertos_arch>/${ARCH}/g" \
            {} \;

    your_name=$(git config get user.name)
    your_email_address=$(git config get user.email)
    find -name 'library.properties' -exec sed -i \
            -e "s/YOUR NAME/${your_name}/" \
            -e "s/your@email.address/${your_email_address}/" \
            {} \;
    popd > /dev/null
}

__prepare_directories
__copy_freertos_source
__extract_port
__pach_rcn
