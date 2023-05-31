#!/bin/bash

# Hard coded path
SRCDIR="...../Delphes-3.5.0"
OUTDIR="...../output"
SFT_SETUP_PATH="/cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh"
NEVENTS=10000

# Variable signal parameters
INPUT_LHEFILE="${1:-BlackMaxLHArecord.lhe}"
if [ ! -f "${INPUT_LHEFILE}" ]; then
  echo "Input LHE file does not exists. INPUT_LHEFILE:${INPUT_LHEFILE}"
  exit
fi
OUTPUT_ROOTFILE="$(basename "$INPUT_LHEFILE")"
OUTPUT_ROOTFILE="${OUTPUT_ROOTFILE/.lhe/.root}"

if [ ! -d "${SRCDIR}" ]; then
  echo "source directory does not exists. SRCIDR:${SRCDIR}"
  exit
fi

if [ -e "${OUTDIR}/${OUTPUT_ROOTFILE}" ]; then
  echo "File already exists. skip."
  exit
fi

# Create working directory
if [ ! -d "${TMPDIR}" ]; then
  mkdir -p "${TMPDIR}"
  echo "Created temporary directory (${TMPDIR})."
fi
wdir=$(mktemp -d)
ret=$?
if [ ${ret} -ne 0 ]; then
  echo "mktemp failed. Please check tmp directory."
  exit
fi
cd "${wdir}" || exit

# shellcheck disable=SC1090
source "${SFT_SETUP_PATH}"

# Setup LHAPDF
lhapdflib=$(lhapdf-config --libdir)
lhapdfdata=$(lhapdf-config --datadir)
export LD_LIBRARY_PATH=${lhapdflib}:${LD_LIBRARY_PATH}
export LHAPATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
export LHAPDF_DATA_PATH=${lhapdfdata}:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current

cp -r "${SRCDIR}"/cards/FCC .

# Setup Pythia8 config
cat << EOF > setup.cmnd
Main:numberOfEvents       = ${NEVENTS}
Beams:frameType           = 4
Beams:LHEF                = ${INPUT_LHEFILE}
PDF:pSet                  = LHAPDF6:cteq6l1
EOF

"${SRCDIR}"/DelphesPythia8 FCC/FCChh.tcl setup.cmnd "${OUTPUT_ROOTFILE}" >& stdouterr."${OUTPUT_ROOTFILE}".dat

mkdir -p "${OUTDIR}"
mv "${OUTPUT_ROOTFILE}" "${OUTDIR}"/
mv stdouterr."${OUTPUT_ROOTFILE}".dat "${OUTDIR}"/

rm -rf "${wdir}"

cd - || exit
