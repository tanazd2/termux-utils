#!/data/data/com.termux/files/usr/bin/env node

import { spawnSync } from "child_process";
import fs from "fs";
import path from "path";

const PREFIX: string = process.env.PREFIX || "/data/data/com.termux/files/usr";
const INSTALL_BIN: string = path.join(PREFIX, "bin");
const BIN_DIR: string = process.env.TERMUX_UTILS_HOME || process.cwd();

const REPO_URL: string =
  "https://raw.githubusercontent.com/tanazd2/termux-utils/main/index.js";

const invokedName: string = path.basename(process.argv[1]);
const args: string[] = process.argv.slice(2);

// ================= DOWNLOAD =================
function downloadScript(): string {
  let res = spawnSync("curl", ["-sL", REPO_URL], { encoding: "utf-8" });

  if (!res.stdout) {
    console.log("[*] curl failed, trying wget...");
    res = spawnSync("wget", ["-qO-", REPO_URL], { encoding: "utf-8" });
  }

  if (!res.stdout) {
    throw new Error("Download failed");
  }

  return res.stdout;
}

// ================= COMMAND SCAN =================
function getCommands(): string[] {
  return fs.readdirSync(BIN_DIR).filter((file: string) => {
    const full = path.join(BIN_DIR, file);

    if (
      file.endsWith(".js") ||
      file.endsWith(".md") ||
      file.endsWith(".json") ||
      file.endsWith(".h") ||
      file === "node_modules"
    ) {
      return false;
    }

    return fs.statSync(full).isFile();
  });
}

// ================= RELINK =================
function relink(): void {
  const cmds = getCommands();

  cmds.forEach((cmd: string) => {
    const link = path.join(INSTALL_BIN, cmd);

    try {
      if (fs.existsSync(link)) fs.unlinkSync(link);
      fs.symlinkSync("termux-utils", link);
    } catch {
      // ignore
    }
  });

  console.log("[✔] Relinked");
}

// ================= INSTALL =================
function install(): void {
  console.log("[*] Installing...");

  const target = path.join(INSTALL_BIN, "termux-utils");
  const alias = path.join(INSTALL_BIN, "tu");

  try {
    let source: string;

    if (process.argv[1].startsWith("/dev/fd")) {
      console.log("[*] Pipe detected → downloading...");
      source = downloadScript();
      fs.writeFileSync(target, source);
    } else {
      fs.copyFileSync(process.argv[1], target);
    }

    fs.chmodSync(target, 0o755);

    if (fs.existsSync(alias)) fs.unlinkSync(alias);
    fs.symlinkSync("termux-utils", alias);

    relink();
  } catch (err) {
    console.error("[!] Install failed:", err);
    process.exit(1);
  }
}

// Run install automatically (if this is main script)
install();
