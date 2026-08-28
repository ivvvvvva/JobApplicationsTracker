# Job Applications Tracker

Desktop aplikacija za pracenje prijava za posao i pripremu za intervjue.

Projekat je napravljen kao C++ projekat. Koristi Qt Widgets za graficki interfejs, SQLite za lokalno cuvanje podataka i CMake za build.

## Funkcionalnosti

- Dashboard sa osnovnom statistikom
- Dodavanje, izmena, brisanje i pregled prijava za posao
- Pretraga prijava po kompaniji i poziciji
- Filtriranje prijava po statusu
- Cuvanje interview pitanja za svaku prijavu
- Question Bank sa pretragom i filterima
- Oznacavanje pitanja kao practiced/not practiced
- Mock Interview sa nasumicno izabranim pitanjima

## Tehnologije

- C++17
- Qt 6 Widgets
- Qt SQL
- SQLite
- CMake
- Ninja

## Pokretanje Na Windows-u

Potrebno je instalirati MSYS2 i pakete:

```bash
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-qt6-base mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

Build i pokretanje iz PowerShell-a:

```powershell
cd "C:\Users\ivaci\Desktop\JobApplications"
cmake -S . -B build -G Ninja
cmake --build build
.\build\JobApplicationsTracker.exe
```

Ako je projekat vec buildovan, dovoljno je:

```powershell
cmake --build build
.\build\JobApplicationsTracker.exe
```

Poruka `ninja: no work to do.` nije greska. Znaci da nema novih izmena za kompajliranje.

## Pokretanje Na Linux-u

Primer za Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build qt6-base-dev libqt6sql6-sqlite
cmake -S . -B build -G Ninja
cmake --build build
./build/JobApplicationsTracker
```

## Struktura Projekta

```text
database/schema.sql
scripts/seed_demo_data.py
src/main.cpp
src/models/
src/database/
src/repositories/
src/ui/
```

## Arhitektura

Projekat ima jednostavnu strukturu:

- `models` sadrzi klase `JobApplication` i `InterviewQuestion`
- `database` sadrzi `DatabaseManager`
- `repositories` sadrzi SQL operacije nad prijavama i pitanjima
- `ui` sadrzi Qt prozore i dijaloge

SQLite baza ima dve glavne tabele: `applications` i `questions`. Tabela `questions` ima foreign key prema tabeli `applications`, uz `ON DELETE CASCADE`.

## Demo Podaci

Za ubacivanje demo podataka:

```bash
python scripts/seed_demo_data.py
```

Script dodaje nekoliko demo prijava i pitanja iz oblasti data science/data engineering.
