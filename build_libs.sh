OS=`uname | awk '{ print tolower($0) }'`
if [[ $OS == 'darwin' ]]; then
    echo "Building .bundle lib..."
    rm -rf _builds/mac
    mkdir -p _builds/mac
    conan install . -if _builds/mac
    cmake -DCMAKE_BUILD_TYPE=Release -H. -B_builds/mac -DBUILD_SHARED_LIBS=ON -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/mac
    cmake --build _builds/mac
    echo "Copying built .bundle lib..."
    cp _builds/mac/lib/libmaestro.dylib maestro-unity/Plugins/libmaestro.bundle
    echo "Done"
else
    echo "Not running in OSX, skipping .bundle lib build"
fi

echo "Building .so lib..."
echo "Building image..."
docker build -f Dockerfile-builder -t maestro-example-builder:latest .

echo "Running Docker commands..."
docker run -v $(pwd):/app-src maestro-example-builder:latest -c "cmake --version && conan --version && rm -rf _builds/linux && mkdir -p _builds/linux && conan install . -if _builds/linux --build missing && cmake -H. -B_builds/linux -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=../deps/restclient-cpp/linux && cmake --build _builds/linux"

echo "Copying built .so lib..."
cp _builds/linux/lib/libmaestro.so maestro-unity/Plugins/libmaestro.so

echo "Done"
