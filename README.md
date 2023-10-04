# LuaUnlocker

Enables the Lua VScripting language, now as a MetaMod plugin!

Build instructions (Windows)
```bash
git clone https://github.com/Source2ZE/LuaUnlocker/ && cd LuaUnlocker

setx MMSOURCE112=/path/to/metamod/
setx HL2SDKCS2=/path/to/sdk/submodule

mkdir build && cd build
python ../configure.py -s cs2
ambuild
```

Build instructions (Linux)
```bash
git clone https://github.com/Source2ZE/LuaUnlocker/ && cd LuaUnlocker

export MMSOURCE112=/path/to/metamod/
export HL2SDKCS2=/path/to/sdk/submodule

mkdir build && cd build
python3 ../configure.py -s cs2
ambuild
```
