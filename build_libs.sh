OS=`uname | awk '{ print tolower($0) }'`
if [[ $OS == 'darwin' ]]; then
    echo "Building .bundle lib..."
    rm -f CMakeCache.txt
    cmake .
    make
    echo "Copying built .bundle lib..."
    cp lib/libmaestro.dylib maestro-unity/Plugins/libmaestro.bundle
    echo "Done"
else
    echo "Not running in OSX, skipping .bundle lib build"
fi

echo "Building .so lib..."
echo "Building image..."
docker build -f Dockerfile-builder -t maestro-example-builder .
echo "Running commands..."
docker run -v $(pwd):/app-src maestro-example-builder:latest -c "rm -f CMakeCache.txt && cmake . && make"
echo "Copying built .so lib..."
cp lib/libmaestro.so maestro-unity/Plugins/libmaestro.so
echo "Done"
