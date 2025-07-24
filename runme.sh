#!/bin/bash
# SPDX-License-Identifier: GPL-2.0+
# Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)

prj_dir=$(dirname $(realpath $0))

ZIP=${1}
if [ "${1}" == "" ]; then
    echo "[FAIL] Usage: "$(basename $0)" <FreeRTOS-Kernel.ZIP>"
    exit 1
fi

for d in $(ls -d ${prj_dir}/skel/*); do
    arch=$(basename ${d})
    common_skel=${prj_dir}/Arduino_FreeRTOS-common.tar.bz2
    pushd common > /dev/null
    tar cjf ${common_skel} *
    popd > /dev/null
    arch_skel=${prj_dir}/Arduino_FreeRTOS-${arch}.tar.bz2
    pushd ${d} > /dev/null
    tar cjf ${arch_skel} *
    popd > /dev/null
    ${prj_dir}/__package.sh ${1} ${arch}
    rm -f ${arch_skel}
    rm -f ${common_skel}
done
