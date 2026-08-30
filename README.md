# Job Applications Tracker

Ovo je desktop aplikacija koju sam napravila da bih lakse pratila prijave za posao i pitanja za intervjue.

Ideja je da na jednom mestu imam kompanije, pozicije, status prijave i pitanja koja zelim da vezbam pre intervjua. Projekat je pisan u C++-u, sa Qt Widgets interfejsom i lokalnom SQLite bazom.

## Sta aplikacija radi

- prikazuje osnovnu statistiku prijava
- cuva prijave za posao
- omogucava dodavanje, izmenu i brisanje prijava
- pretrazuje prijave po kompaniji ili poziciji
- filtrira prijave po statusu
- cuva interview pitanja za svaku prijavu
- ima poseban pregled svih pitanja
- oznacava da li je pitanje vezbano
- ima mock interview opciju sa nasumicnim pitanjima

## Tehnologije

Koristila sam:

- C++17
- Qt 6 Widgets
- Qt SQL
- SQLite
- CMake
- Ninja

## Pokretanje na Windows-u

Potrebno je imati MSYS2 i instalirane pakete za Qt, CMake i Ninja:

```bash
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-qt6-base mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

Build iz PowerShell-a:

```powershell
cd "C:\Users\ivaci\Desktop\JobApplications"
cmake -S . -B build -G Ninja
cmake --build build
.\build\JobApplicationsTracker.exe
```

Ako je projekat vec buildovan, dovoljno je samo:

```powershell
cmake --build build
.\build\JobApplicationsTracker.exe
```

Ako se pojavi poruka `ninja: no work to do.`, to nije greska. Samo znaci da nema novih izmena za kompajliranje.

## Pokretanje na Linux-u

Primer za Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build qt6-base-dev libqt6sql6-sqlite
cmake -S . -B build -G Ninja
cmake --build build
./build/JobApplicationsTracker
```

## Struktura projekta

```text
database/schema.sql
scripts/seed_demo_data.py
src/main.cpp
src/models/
src/database/
src/repositories/
src/ui/
```

Ukratko:

- `models` sadrzi klase za prijave i pitanja
- `database` sadrzi konekciju i inicijalizaciju baze
- `repositories` rade SQL operacije
- `ui` sadrzi glavne prozore i dijaloge

Baza ima dve glavne tabele: `applications` i `questions`.
Pitanja su povezana sa prijavama, tako da se pitanja za neku prijavu brisu zajedno sa tom prijavom.

## Demo podaci

Za ubacivanje demo podataka moze da se pokrene:

```bash
python scripts/seed_demo_data.py
```

Script dodaje nekoliko primera prijava i pitanja, cisto da aplikacija ne bude prazna pri testiranju.
