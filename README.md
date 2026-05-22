# Git & GitHub — Guida per ProgettoPCS
> Repository: [ScaccoFranco/ProgettoPCS](https://github.com/ScaccoFranco/ProgettoPCS)
> Progetto di Programmazione e Calcolo Scientifico — gruppo di 3

---

## Setup iniziale (una volta sola, per chi non ha ancora clonato)

Il repository esiste già su GitHub. Gli altri due membri del gruppo devono solo clonarlo:

```bash
git clone https://github.com/ScaccoFranco/ProgettoPCS.git
cd ProgettoPCS
```

Poi configurate il vostro nome (se non l'avete già fatto):

```bash
git config --global user.name "Nome Cognome"
git config --global user.email "tua@email.com"
```

Per usare SSH invece di HTTPS (evita di reinserire la password ogni volta):

```bash
# Genera la chiave se non ce l'hai
ssh-keygen -t ed25519 -C "tua@email.com"
# Copia la chiave pubblica e aggiungila su GitHub → Settings → SSH Keys
cat ~/.ssh/id_ed25519.pub
# Poi cambia il remote sulla repo locale
git remote set-url origin git@github.com:ScaccoFranco/ProgettoPCS.git
```

---

## .gitignore — da aggiungere subito

Prima di iniziare a scrivere codice, aggiungete un `.gitignore` adatto a C++/CMake:

```
# Build
build/
cmake-build-*/
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
Makefile

# Binari e oggetti
*.o
*.a
*.so
*.exe
*.out

# Editor
.vscode/
.idea/
*.user

# Sistema
.DS_Store
Thumbs.db
```

```bash
# Creare e committare il .gitignore
touch .gitignore
# ... incollate il contenuto sopra ...
git add .gitignore
git commit -m "Aggiunge .gitignore per C++/CMake"
git push origin main
```

---

## Flusso di lavoro quotidiano

### 1. Prima di iniziare — sempre

```bash
git pull origin main
```

Scarica le ultime modifiche degli altri. **Obbligatorio ogni volta che aprite il progetto.**

### 2. Crea un branch per la tua parte

```bash
git checkout -b nome-branch
```

Esempi concreti per ProgettoPCS:

```bash
git checkout -b feature/struttura-dati
git checkout -b feature/algoritmo-principale
git checkout -b feature/test-unitari
git checkout -b fix/bug-lettura-input
```

Lavorate **sempre su un branch separato**, mai direttamente su `main`.

### 3. Salva le modifiche con un commit

```bash
git status                         # controlla cosa è cambiato
git add .                          # aggiunge tutto
git add src/miofile.cpp            # oppure file per file

git commit -m "Implementa classe Mesh con lettura da file"
```

Messaggi di commit utili: scrivi **cosa hai fatto**, non come. Esempi:

```
✓ "Aggiunge funzione di calcolo dell'area del triangolo"
✓ "Fix: corregge out-of-bounds nel ciclo su celle"
✗ "modifiche"
✗ "aggiornato file"
```

### 4. Manda il branch su GitHub

```bash
git push origin nome-branch
```

La prima volta Git potrebbe chiederti di impostare l'upstream — accetta con:

```bash
git push --set-upstream origin nome-branch
```

### 5. Apri una Pull Request su GitHub

1. Vai su [github.com/ScaccoFranco/ProgettoPCS](https://github.com/ScaccoFranco/ProgettoPCS)
2. Clicca **"Compare & pull request"** (appare dopo il push)
3. Scrivi una breve descrizione di cosa hai fatto
4. Assegna la review a un compagno del gruppo
5. Dopo la review → **"Merge pull request"** → **"Delete branch"**

### 6. Dopo il merge, aggiorna il tuo main locale

```bash
git checkout main
git pull origin main
```

---

## Comandi utili di tutti i giorni

| Comando | Cosa fa |
|--------|---------|
| `git status` | Mostra i file modificati/aggiunti/non tracciati |
| `git log --oneline` | Storia dei commit in modo compatto |
| `git log --oneline --graph --all` | Grafico visivo di tutti i branch |
| `git diff` | Differenze non ancora committate |
| `git diff HEAD~1` | Differenze rispetto all'ultimo commit |
| `git branch` | Elenca i branch locali |
| `git branch -a` | Tutti i branch, anche remoti |
| `git checkout main` | Torna sul branch main |
| `git stash` | Mette da parte le modifiche temporaneamente |
| `git stash pop` | Ripristina le modifiche messe da parte |

---

## Gestire i conflitti

Succede quando due persone modificano **la stessa riga** dello stesso file. Git lo segnala così:

```
<<<<<<< HEAD
// tua versione
double calcola_area(Triangolo t) { return t.base * t.altezza / 2; }
=======
// versione dell'altro
double calcola_area(Triangolo t) { return 0.5 * t.base * t.altezza; }
>>>>>>> feature/algoritmo-principale
```

**Come risolverlo:**

1. Apri il file nel tuo editor, scegli quale versione tenere (o combinale)
2. Rimuovi le righe marcatori: `<<<<<<<`, `=======`, `>>>>>>>`
3. Salva, poi:

```bash
git add src/file-in-conflitto.cpp
git commit -m "Risolve conflitto in calcola_area"
```

**Come prevenirli:** dividetevi i file su cui lavorare e fate `git pull` spesso.

---

## Errori comuni e come correggerli

### Ho committato sul branch sbagliato (es. su main invece che sul mio branch)

```bash
git branch mio-branch          # crea il branch con il commit sbagliato
git reset HEAD~1 --hard        # rimuove il commit da main
git checkout mio-branch        # passa al branch corretto
```

### Ho scritto male il messaggio del commit (non ancora pushato)

```bash
git commit --amend -m "Messaggio corretto"
```

### Voglio annullare le modifiche a un file (non ancora committato)

```bash
git checkout -- src/nomefile.cpp
```

### Ho pushato qualcosa che non dovevo

```bash
# NON usare git push --force su main (cancella la storia degli altri)
# Invece: crea un commit che annulla le modifiche
git revert <hash-commit>
git push origin main
```

### Voglio vedere com'era il codice in un commit precedente

```bash
git log --oneline              # trova l'hash del commit
git checkout <hash>            # entra in modalità "detached HEAD"
# ... guarda il codice ...
git checkout main              # torna alla versione corrente
```

---

## Workflow consigliato per ProgettoPCS

```
main  ──●──────────────●──────────────────●──────────────●──
        │              ↑                  ↑              ↑
        │  feature/    │    feature/       │   fix/       │
        └──struttura───┘    algoritmo──────┘   bug────────┘
```

- `main`: solo codice **che compila e funziona**
- Ogni membro lavora sul proprio branch
- Merge su `main` solo dopo una rapida review di almeno un altro

---

## Riassunto del ciclo tipico

```bash
# Inizio sessione di lavoro
git pull origin main
git checkout -b feature/mia-parte

# ... lavora su src/, include/, test/, CMakeLists.txt ...

# Salva il lavoro
git add .
git commit -m "Descrizione chiara di cosa ho fatto"
git push origin feature/mia-parte

# Su GitHub: apri Pull Request → review → merge

# Fine: risincronizza il main
git checkout main
git pull origin main
```
