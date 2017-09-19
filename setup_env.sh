OLD_PWD=$(pwd)
cd $(dirname ${BASH_SOURCE[0]})

export KERNEL=kernel7

export HOSTUNAME=$(uname -s)
export PLATFORMUNAME=$(uname -m)

export NEXMON_ROOT=$(pwd)

if [ $HOSTUNAME == "Darwin" ]; then
    export CC=$NEXMON_ROOT/buildtools/arc_gnu_bin/bin/arc-elf32-
    export CCPLUGIN=$NEXMON_ROOT/buildtools/gcc-nexmon-plugin-osx/nexmon.so
    export ZLIBFLATE="openssl zlib"
else if [ $HOSTUNAME == "Linux" ] && [ $PLATFORMUNAME == "x86_64" ]; then
    export CC=$NEXMON_ROOT/buildtools/arc_gnu_bin/bin/arc-elf32-
    export CCPLUGIN=$NEXMON_ROOT/buildtools/gcc-nexmon-plugin/nexmon.so
    export ZLIBFLATE="zlib-flate -compress"
else
    echo "Platform not supported!"
fi
fi

export Q=@
export NEXMON_SETUP_ENV=1

cd "$OLD_PWD"
