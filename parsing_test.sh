#!/bin/sh

DIALOG=./d4web

export D4PHEIGHT=0
export D4PMINHEIGHT=5
export D4PWIDTH=80
export D4PFULLSCREEN="n"
export D4PALIGNCENTER="n"
export D4PASCIILINES="y"

export PKGNAME="Options for atata-21_1.0.7"

export PKGHELP=`pwd`/PKGHELP

export PORT_OPTIONS="FreeBSD Cc_1 W R11 R32 GRA MM1 MM2 MM4 R12"
export NEW_OPTIONS="Cc_1 MM3 R22"

# Test for DEFINE/ALL_OPTIONS
export ALL_OPTIONS="FreeBSD BbBbb Cc_1"
export FreeBSD_DESC="FreeBSD is an advanced operating system for modern server, desktop, and embedded computer platforms"
export BbBbb_DESC="This is B description line"
export Cc_1_DESC="This is C description line"

# Test for RADIO
export OPTIONS_RADIO="R1 R2 R3"
export OPTIONS_RADIO_R1="R11 R12"
export OPTIONS_RADIO_R2="R21 R22"
export OPTIONS_RADIO_R3="R31 R32"
export R1_DESC="Separate line R1"
export R11_DESC="This is R11 description line"
export R12_DESC="This is R12 description line"
export R2_DESC="Separate line R2"
export R21_DESC="R21"
export R22_DESC="R22"
export R3_DESC="Separate line R3"
export R31_DESC="R31"
export R32_DESC="R32"

# Test for SINGLE
export OPTIONS_SINGLE="S"
export OPTIONS_SINGLE_S="T W"
export S_DESC="Separate line S"
export T_DESC="This is T description line"
export W_DESC="This is W description line"

# Test for GROUP
export OPTIONS_GROUP="G1"
export OPTIONS_GROUP_G1="GRA GRO"
export G1_DESC="Separate line G1"
export GRA_DESC="This is GRA description line"
export GRO_DESC="This is GRO description line"

# Test for MULTI
export OPTIONS_MULTI="M1 M2"
export OPTIONS_MULTI_M1="MM1 MM2"
export OPTIONS_MULTI_M2="MM3 MM4 MM5"
export M1_DESC="sep M1 text"
export M2_DESC="M2 sep text"
export MM1_DESC="text MM1 text"
export MM2_DESC="MM2 text text"
export MM3_DESC="text MM3 text"
export MM4_DESC="MM4 text text"
export MM5_DESC="MM5 text text"
${DIALOG}
