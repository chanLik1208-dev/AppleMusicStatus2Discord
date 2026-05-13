<div align="center">
  <h1>🎵 AppleMusicStatus2Discord</h1>
  <p><b>自動將您正在聆聽的 Apple Music 歌曲同步到 Discord 個人狀態！</b></p>
  
  [![Build Executables](https://github.com/chanLik1208-dev/AppleMusicStatus2Discord/actions/workflows/build.yml/badge.svg)](https://github.com/chanLik1208-dev/AppleMusicStatus2Discord/actions/workflows/build.yml)
  [![C++ Standard](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
  [![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows-lightgrey.svg)]()
  [![License: ISC](https://img.shields.io/badge/License-ISC-blue.svg)](https://opensource.org/licenses/ISC)
</div>

<br/>

## ✨ 簡介

這是一個使用 **C++** 撰寫的輕量級跨平台桌面應用程式。
它會在背景讀取目前 Apple Music 上的播放狀態，並將其透過 **Discord Rich Presence** 完美呈現給您的朋友！

<br/>

## 🌟 功能特性

- 🎧 **即時同步**：顯示正在播放的歌曲名稱、演唱者及專輯名稱。
- ⏱️ **進度追蹤**：支援動態顯示播放進度條。
- 🛡️ **隱私保護**：當歌曲暫停或關閉時，會自動清除 Discord 狀態，保護您的隱私。
- 💻 **跨平台支援**：完美支援 **Windows 10/11** (透過原生 SMTC API) 與 **macOS** (透過 AppleScript)。
- ⚙️ **開箱即用**：內建預設的 Apple Music Client ID，無須繁瑣設定即可直接使用。

<br/>

## 🚀 快速開始

### 取得應用程式
您可以直接到 [GitHub Actions](https://github.com/chanLik1208-dev/AppleMusicStatus2Discord/actions) 下載自動編譯好的最新版本 (Artifacts)，或者自行編譯。

### 執行方式
只需在終端機中執行編譯好的程式即可啟動背景同步：

```bash
# macOS 或 Linux
./AppleMusicStatus2Discord

# Windows
AppleMusicStatus2Discord.exe
```

### 停止同步
在終端機視窗中按下 `Ctrl + C` 即可安全關閉程式。

---

### 🔧 進階設定 (自訂 Discord Client ID)
如果你有自己的 Discord Developer Application，可以在執行時將您的 Client ID 作為參數傳入：

```bash
./AppleMusicStatus2Discord <your_client_id_here>
```

<br/>

## 🛠️ 開發與建置

### 前置需求
- **C++17** 或更高版本編譯器。
- **CMake** (3.16 以上)。

### 編譯步驟

1. **建立建置目錄**：
```bash
mkdir build
cd build
```

2. **執行 CMake**：
```bash
cmake ..
```
> **Windows 注意：** 如果是使用 Visual Studio，建議指定生成器：
> `cmake .. -G "Visual Studio 17 2022" -A x64`

3. **編譯專案**：
```bash
cmake --build . --config Release
```

<br/>

## 🤝 貢獻
歡迎提出 Issues 或發送 Pull Requests！任何形式的貢獻都非常感謝。

## 📜 授權
本專案採用 [ISC License](LICENSE) 授權。