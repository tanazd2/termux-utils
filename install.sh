#!/data/data/com.termux/files/usr/bin/bash

echo "[*] Installing termux-miscellaneous..."

BIN_DIR="$PREFIX/bin"
BASE_URL="https://raw.githubusercontent.com/tanazd1/termux-miscellaneous/main"

CMDS="c cdt ns su tc tct x"

for cmd in $CMDS; do
    echo "[*] Installing $cmd..."

    if curl -fsSL "$BASE_URL/$cmd" -o "$BIN_DIR/$cmd"; then
        chmod +x "$BIN_DIR/$cmd"
        echo "[✓] Installed $cmd"
    else
        echo "[✗] Failed to download $cmd"
    fi
done

echo "[✓] Done!"
