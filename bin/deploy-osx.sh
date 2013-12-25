#!/usr/bin/env bash
#
# Use macdeployqt to copy the required Qt frameworks and resources into the
# app bundle, unless the binary could not be found. Afterwards, the script
# proceeds to copying non-Qt libraries (see $LIBRARIES) into the bundle's
# Frameworks path.
#
# The 3rd-party library paths are parsed from otool output, so otool must be
# available.
#
# NOTE:
# 	This script is expected to run with qtcanvas/bin as the working directory
# 	and AFTER you've generated the app bundle using Qt Creator.
#
# USAGE:
# 	cd /path/to/qtcanvas/bin
# 	./deploy-osx.sh

BUNDLE="./qtCanvas.app"
EXECUTABLE="qtCanvas.app/Contents/MacOS/qtCanvas"
FRAMEWORKS="qtCanvas.app/Contents/Frameworks"
LIBRARIES=( "libcanvas" "libjsoncpp" )
LIBRARY_DIRS=( "/usr/local" "/opt" "/usr" )
QTDEPLOY=`which macdeployqt`

if [ ! -d $BUNDLE ]; then
	echo "qtCanvas.app bundle is either not generated, or the script is not " \
	     "running from its directory. Aborting."
	exit 1
fi

echo "bundling in 3rd-party libraries..."
mkdir -p "${FRAMEWORKS}"

for lib in "${LIBRARIES[@]}"; do
  LIB_FNAME=`otool -L ${EXECUTABLE} | awk '{print $1}' | grep ${lib}`

  echo "  > bundling ${lib} (${LIB_FNAME})"

  if [ -f "${LIB_FNAME}" ]; then
    cp "${LIB_FNAME}" "${FRAMEWORKS}/"
  else

    for lib_dir in "${LIBRARY_DIRS[@]}"; do
      LIB_PNAME="${lib_dir}/lib/${LIB_FNAME}"
      echo "    searching for library at: ${LIB_PNAME}"

      if [ -f "${LIB_PNAME}" ]; then
        cp "${LIB_PNAME}" "${FRAMEWORKS}/"
        break
      fi
    done
  fi
done

if [ -z "${QTDEPLOY}" ]; then
	echo "macdeployqt was not found in PATH, you must link on your own"
else
  echo "running macdeployqt..."
	macdeployqt ${BUNDLE}
fi
