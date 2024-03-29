# SuperBDL

THIS PROJECT IS CURRENTLY UNFINISHED
If you are interested in continuing this project, you are more than welcome to do so.

## Building

### Linux

Run `make`.

### Windows

If you have not done so already, install vcpkg:

```
mkdir C:\src
cd C:\src
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
```

Next, integrate vcpkg for use with MSBuild/Visual Studio. (You may be prompted for administrator privileges.)

```
vcpkg integrate install
```

Finally, install [json-c](https://github.com/json-c/json-c) and [assimp](https://www.assimp.org/):

```
vcpkg install json-c assimp
```

Once these dependencies are satisfied, open `SuperBDL.sln` in Visual Studio and build the solution (Build -> Build SuperBDL or Ctrl-B).

## Usage
