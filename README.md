# アーチャー物語 (Archer Story)
アーチャー伝説のパクリゲー

このGitHubリポジトリはGitというよりはファイルのクラウドとして使ってますので、
いろいろ適当です()

## ver1.2完成！
./アーチャー物語_ver1.2.zip をDLして解凍すれば遊べます。

## Recent Improvements (Code Refactoring)

The codebase has been comprehensively refactored to improve:
- **Code Quality**: English documentation, better naming conventions
- **Memory Safety**: Fixed memory leaks, improved RAII principles
- **Modern C++**: Using C++17 features, const correctness, STL algorithms
- **Maintainability**: Replaced magic numbers with named constants
- **Build System**: Added CMake support

See [REFACTORING.md](REFACTORING.md) for detailed information.

## Building from Source

### Requirements
- C++17 compatible compiler
- DxLib (Windows)
- CMake 3.10+ (optional, for CMake build)

### Manual Build (Windows)
Compile all `.cpp` files with DxLib and link against the DxLib library.

### CMake Build (Recommended)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### <使用ライブラリ (Libraries Used)>
- ＤＸライブラリ(DxLib) - Japanese game development library
- nlohmann-json - JSON parsing
- strconv - <https://qiita.com/javacommons/items/9ea0c8fd43b61b01a8da>

## License
See [LICENSE](LICENSE) file.
