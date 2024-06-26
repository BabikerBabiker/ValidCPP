PROJECT_NAME="ValidCPP"
BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi
cd "$BUILD_DIR" || exit
cmake ..
cmake --build .
if [ $? -eq 0 ]; then
  ./"$PROJECT_NAME"
else
  echo "Build failed. Please check the errors above."
fi
cd ..