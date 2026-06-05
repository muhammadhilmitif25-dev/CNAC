# 📝 Program CNAC — Console Notepad Application in C

Aplikasi notepad berbasis terminal yang dibangun menggunakan bahasa C, dikembangkan sebagai project mata kuliah **CKA**.

---

## 👥 Anggota Kelompok

| NIM       | Nama                | GitHub   |
|-----------|---------------------|----------|
| [NIM kamu] | [Nama kamu]        | [GitHub] |

**Tim:** CNAC  
**Dosen Penanggung Jawab:** Yudi Widhiyasana, S.Si., MT

---

## 🖥️ Sistem Operasi yang Didukung

- Windows

---

## 🛠️ Setup Environment

Sebelum project dimulai, pastikan tools berikut sudah terinstall:

| Tool               | Keterangan                                         | Link Download                  |
|--------------------|----------------------------------------------------|--------------------------------|
| Git                | Digunakan untuk clone repository                   | https://git-scm.com/           |
| MSYS2 / MinGW64    | Environment dan compiler untuk menjalankan program | https://www.msys2.org/         |
| Visual Studio Code | Code editor untuk menulis dan mengedit kode        | https://code.visualstudio.com/ |

---

## ✅ Cek Instalasi (via CMD)

Buka **Command Prompt (CMD)**, lalu jalankan perintah berikut untuk memastikan semua tools sudah terpasang:

```bash
git --version
where gcc
code --version
```

---

## 📥 Clone Repository

1. Buka CMD / Terminal
2. Jalankan perintah berikut:

```bash
git clone https://github.com/username/nama-repo.git
```

3. Masuk ke folder project:

```bash
cd nama-repo
```

4. Buka di Visual Studio Code:

```bash
code .
```

---

## ▶️ Build & Run Program

### 1. Kompilasi — Membuat `program.exe`

Pastikan kamu sudah berada di dalam folder project, lalu jalankan perintah GCC berikut di CMD:

```cmd
gcc main.c buffer.c cursor.c render.c newpaper.c searchfile.c fileio.c -o program.exe
```

> [!IMPORTANT]
> **File yang wajib ada di folder yang sama:**
> `main.c`, `buffer.c`, `cursor.c`, `render.c`, `newpaper.c`, `searchfile.c`, `fileio.c`
>
> Jika kamu menambah file `.c` baru, tambahkan juga ke dalam perintah kompilasi di atas.

### 2. Jalankan Program

Setelah kompilasi berhasil (tidak ada error), jalankan aplikasinya:

```cmd
program.exe
```

---

## 🔄 Workflow Pengembangan

Gunakan alur berikut setiap kali melakukan perubahan pada kode:

1. **Simpan perubahan** — Tekan `Ctrl + S` di VS Code untuk semua file yang diedit
2. **Re-compile** — Jalankan kembali perintah `gcc` di atas. Jika muncul error, periksa titik koma (`;`) atau typo pada kode
3. **Jalankan** — Ketik `program.exe` untuk menjalankan hasil kompilasi terbaru