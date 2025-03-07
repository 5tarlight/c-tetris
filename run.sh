if [ ! -d "build" ]; then
  mkdir build
fi

cd build
cmake ..
make
cd ..

if [ -f "./build/tetris" ]; then
  echo "Build successful"
  echo ""

  ./build/tetris
else
  echo "Build failed"
  exit 1
fi
