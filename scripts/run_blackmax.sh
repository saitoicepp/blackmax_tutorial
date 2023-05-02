#!/bin/bash

# Hard coded path
SRCDIR="...../BlackMax/"
OUTDIR="...../output/"
INPUT_TEMPLATE="...../template_parameter.txt"
SFT_SETUP_PATH="/cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc11-opt/setup.sh"

# Variable signal parameters
CM_ENERGY="${1:-13000}"
M_PL="${2:-1000}"
M_BHMIN="${3:-1000}"
EVO_TYPE="${4:-1}"
N_EXDIM="${5:-6}"
PDFname="${6:-CTEQ6L1}"
SEED="${7:-100000001}"

# Hard coded signal parameters
NEVENTS=10000
FLAG_GRAV=1
EXDIM_SIZE=10 # (1/Mpl)
BRANE_SIZE=1.0 # (1/Mpl)
M_BHMAX=${CM_ENERGY} # GeV
M_STRING=1000 # (GeV)
G_STRING=0.8 # (string coupling)
N_SPLITDIM=0
# loss factors
F_MASSLOSS=0.0
F_MOMLOSS=0.0
F_AMLOSS=0.0

# Output directory name
params="${CM_ENERGY}_${M_PL}_${M_BHMIN}_${EVO_TYPE}_${N_EXDIM}_${PDFname}"
OUTDIR="${OUTDIR}/${params}"

if [ ! -d "${SRCDIR}" ]; then
  echo "source directory does not exists. SRCIDR:${SRCDIR}"
  exit
fi

if [ -e "${OUTDIR}"/BlackMaxLHArecord.lhe ]; then
  echo "File already exists. skip."
  exit
fi

wdir=$(mktemp -d)
cd "${wdir}" || exit

#### PDF
if [ "${PDFname}" == "CT10" ]; then
    PDFID=10800
elif [ "${PDFname}" == "CTEQ6m" ]; then
    PDFID=200
elif [ "${PDFname}" == "CTEQ6L1" ]; then
    PDFID=10042
else
    return
fi

# shellcheck disable=SC1090
source "${SFT_SETUP_PATH}"

# Setup LHAPDF
lhapdflib=$(lhapdf-config --libdir)
lhapdfdata=$(lhapdf-config --datadir)
export LD_LIBRARY_PATH=${lhapdflib}:${LD_LIBRARY_PATH}
export LHAPATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current
export LHAPDF_DATA_PATH=${lhapdfdata}:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current

cp -r ${SRCDIR}/* .

INPUT_TXT="parameter.txt"

sed \
"s#__NEVENTS__#${NEVENTS}#g; \
s#__CME__#${CM_ENERGY}#g; \
s#__M_pl__#${M_PL}#g; \
s#__EVOLUTION_TYPE__#${EVO_TYPE}#g; \
s#__ND__#${N_EXDIM}#g; \
s#__LHAPDFID__#${PDFID}#g; \
s#__ND_SPLIT__#${N_SPLITDIM}#g; \
s#__EXDIM_SIZE__#${EXDIM_SIZE}#g; \
s#__BRANE_SIZE__#${BRANE_SIZE}#g; \
s#__M_BHMIN__#${M_BHMIN}#g; \
s#__M_BHMAX__#${M_BHMAX}#g; \
s#__M_STRING__#${M_STRING}#g; \
s#__G_STRING__#${G_STRING}#g; \
s#__FLAG_GRAV__#${FLAG_GRAV}#g; \
s#__F_MASSLOSS__#${F_MASSLOSS}#g; \
s#__F_MOMLOSS__#${F_MOMLOSS}#g; \
s#__F_AMLOSS__#${F_AMLOSS}#g; \
s#__SEED__#${SEED}#g \
" ${INPUT_TEMPLATE} > ${INPUT_TXT}

./BlackMax >& stdouterr.dat

mkdir -p "${OUTDIR}"
mv BlackMaxLHArecord.lhe "${OUTDIR}"/
mv stdouterr.dat "${OUTDIR}"/
mv output.txt "${OUTDIR}"/

rm -rf "${wdir}"

cd - || exit

