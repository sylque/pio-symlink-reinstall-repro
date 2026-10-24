# pioarduino symlink:// reinstall loop reproduction

This project reproduces [pioarduino/pioarduino-vscode-ide#105](https://github.com/pioarduino/pioarduino-vscode-ide/issues/105) on Windows. Each automatic IntelliSense index rebuild reinstalls the `symlink://` library Button2 and rewrites its `.pio-link` file, the extension sees that write as a library change, and it starts another rebuild. The other `symlink://` libraries are not reinstalled.

## Environment

| Item | Version |
|------|---------|
| OS | Windows 11 |
| VS Code | 1.128.0 |
| pioarduino IDE extension | 1.4.4 |
| PlatformIO Core | 6.1.19 |
| Platform | pioarduino platform-espressif32 55.03.312-1 |

## Layout

```
apps/App/platformio.ini           lib_deps = symlink://../../libs/LibA, lib_ldf_mode = off
apps/App/.vscode/settings.json    autoRebuildAutocompleteIndex = true, intelliSenseEngine = clangd
libs/LibA/library.json            depends on symlink://../../libs/LibB and symlink://../../libs/Io
libs/LibB/library.json            no dependencies
libs/Io/library.json              depends on symlink://../../libs/third-party/Button2
libs/third-party/Button2          unmodified copy of Button2 2.7.0 (MIT), including the .piopm file PlatformIO wrote when it installed Button2 from the registry
```

The dependency paths in the `library.json` files are relative to the project directory `apps/App`, because that is how PlatformIO resolves them.

## Steps

1. Clone the repository on Windows.
2. In VS Code, choose File > New Window, then File > Open Folder and select `apps/App`. Do not open it through a `.code-workspace` file: the extension reads `autoRebuildAutocompleteIndex` without a folder scope, so the project's `.vscode/settings.json` is ignored in that case.
3. Make sure no user setting forces `platformio-ide.autoRebuildAutocompleteIndex` to `false`.
4. Wait, and watch the "pioarduino Project Configuration" output channel.

## Observed

Rebuilds follow each other without any user action. Each one starts with:

```
Processing debug (platform: https://github.com/pioarduino/platform-espressif32/releases/download/55.03.312-1/platform-espressif32.zip; board: esp32-s3-devkitc-1; framework: arduino)
--------------------------------------------------------------------------------
Library Manager: Installing symlink://../../libs/third-party/Button2
Library Manager: Button2@2.7.0 has been installed!
```

and ends with:

```
Building compilation database compile_commands.json
========================= [SUCCESS] Took 4.62 seconds =========================
```

After each rebuild, `.pio/libdeps/debug/Button2.pio-link` has a new modification time, while `LibA.pio-link`, `LibB.pio-link` and `Io.pio-link` keep the time of their first installation.

## Expected

The index rebuilds once after the project opens, no `.pio-link` file is rewritten when its target is unchanged, and the extension stays quiet until a real input changes.
