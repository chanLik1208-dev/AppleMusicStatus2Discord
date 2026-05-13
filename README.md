# AppleMusicStatus2Discord

## 簡介

這是一個使用 C++ 撰寫的桌面應用程式，它會讀取目前正在 Apple Music 上播放的歌曲資訊，並將狀態同步顯示在 Discord 的個人檔案上（Rich Presence）。

## 功能特性

- 顯示正在播放的**歌曲名稱**、**演唱者**及**專輯名稱**。
- 顯示**播放進度條**（可選）。
- 當歌曲暫停或停止時，自動清除 Discord 狀態，恢復隱私。
- **跨平台支援**： Windows 與 macOS 均可編譯與執行。
- 可在啟動時傳入不同的 Discord Client ID。

## 使用方法

### 預設使用

只需在終端機中執行編譯好的程式即可（預設使用 Apple Music 的通用 Client ID）：

```bash
./AppleMusicStatus2Discord
```

### 自訂 Discord Client ID

如果你有自己的 Discord Bot Client ID，可以在執行時作為參數傳入：

```bash
./AppleMusicStatus2Discord <your_client_id_here>
```

### 停止程式

在終端機視窗中按下 `Ctrl + C` 即可安全關閉程式。

## 編譯與建置

### 前置需求

- **C++17** 或更高版本編譯器。
- **CMake** (3.16 以上)。
- **Discord Game SDK**。
    - 已包含於專案中，位置：`libs/discord_game_sdk`。

### 編譯步驟

1. **建立建置目錄**：

```bash
mkdir build
cd build
```

2. **執行 CMake**（視需要調整路徑）：

```bash
cmake ..
```

   *Windows 注意：* 如果是使用 Visual Studio，建議指定生成器：
   ```bash
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

3. **編譯專案**：

```bash
cmake --build .
```

   *Windows 注意：* 如果是 Visual Studio，預設會編譯 Release 版本。

### GitHub Actions (CI/CD)

本專案已設定 GitHub Actions，可自動編譯及打包 for Windows 和 macOS。
每次推送到 `main` 分支或建立 `Release` 時，皆會在 Actions 中自動產生可供下載的執行檔（Artifacts）。

## 貢獻

歡迎提出 Issues 或 Pull Requests。

## 授權

ISC License