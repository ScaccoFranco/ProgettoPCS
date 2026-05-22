# ProgettoPCS
Progetto di programmazione e calcolo scientifico
# Git & GitHub — Guida per lavori di gruppo (3 persone)

## Concetti base da tenere a mente

- **Repository remota**: il codice condiviso su GitHub
- **Repository locale**: la copia sul tuo PC
- **Branch**: una "linea di sviluppo" separata dal ramo principale
- **Commit**: uno snapshot del codice in un dato momento
- **Push/Pull**: inviare/ricevere modifiche dalla repository remota

---

## Setup iniziale (una volta sola)

### Chi crea il repository (una persona del gruppo)

```bash
# Crea la repo su GitHub dal sito, poi collega quella locale:
git init
git remote add origin https://github.com/utente/nome-repo.git
git branch -M main
git push -u origin main
```

### Gli altri due si clonano la repo

```bash
git clone https://github.com/utente/nome-repo.git
cd nome-repo
```

### Configurare il proprio nome (tutti e tre)

```bash
git config --global user.name "Il Tuo Nome"
git config --global user.email "tua@email.com"
```

---

## Flusso di lavoro quotidiano

### 1. Prima di iniziare a lavorare — aggiornati sempre

```bash
git pull origin main
```

Scarica le ultime modifiche degli altri. **Fallo ogni volta che apri il progetto.**

### 2. Crea un branch per la tua feature/parte

```bash
git checkout -b nome-branch
# Esempio: git checkout -b feature/login
# Esempio: git checkout -b fix/calcolo-media
```

Lavora **sempre su un branch separato**, mai direttamente su `main`.

### 3. Salva le modifiche con un commit

```bash
git add .                          # aggiunge tutti i file modificati
git add src/miofile.cpp            # oppure aggiungi solo un file specifico

git commit -m "Aggiunge funzione di login"
```

Scrivi messaggi di commit **chiari e brevi** (cosa hai fatto, non come).

### 4. Manda il branch su GitHub

```bash
git push origin nome-branch
```

### 5. Apri una Pull Request su GitHub

Dal sito GitHub: **"Compare & pull request"** → scrivi una breve descrizione → chiedi la review a un compagno → merge su `main`.

### 6. Dopo il merge, aggiorna il tuo main locale

```bash
git checkout main
git pull origin main
```

---

## Comandi utili di tutti i giorni

| Comando | Cosa fa |
|--------|---------|
| `git status` | Mostra cosa è stato modificato/aggiunto |
| `git log --oneline` | Mostra la storia dei commit in modo compatto |
| `git diff` | Mostra le differenze non ancora committate |
| `git branch` | Elenca i branch locali |
| `git branch -a` | Elenca tutti i branch (anche remoti) |
| `git checkout main` | Torna sul branch main |
| `git stash` | Mette da parte le modifiche temporaneamente |
| `git stash pop` | Ripristina le modifiche messe da parte |

---

## Gestire i conflitti

Succede quando due persone modificano **la stessa riga** dello stesso file.

```bash
# Git segnalerà il conflitto così nel file:
<<<<<<< HEAD
tua versione
=======
versione dell'altro
>>>>>>> nome-branch
```

**Come risolverlo:**

1. Apri il file, scegli quale versione tenere (o unisci le due)
2. Rimuovi le righe con `<<<<`, `====`, `>>>>`
3. Salva, poi:

```bash
git add file-in-conflitto
git commit -m "Risolve conflitto in file-in-conflitto"
```

**Prevenire i conflitti:** dividetevi bene i file su cui lavorare e fate `git pull` spesso.

---

## Comandi per la collaborazione su GitHub (CLI `gh`)

Se hai installato la GitHub CLI:

```bash
gh auth login                      # autenticati
gh repo clone utente/nome-repo     # clona una repo
gh pr create                       # crea una pull request dal terminale
gh pr list                         # elenca le PR aperte
gh pr merge                        # fai il merge di una PR
```

---

## Workflow consigliato per 3 persone

```
main  ──────●──────────────────●──────────────────●──
            │                  ↑                  ↑
feature/A   └──●──●──●─────────┘                  │
                                                   │
feature/B           └──●──●──●────────────────────┘
```

- `main`: solo codice stabile e funzionante
- Ogni persona lavora su un branch separato
- Si fa merge su `main` solo dopo review (anche veloce) di un altro

---

## Errori comuni e come correggerli

### Ho committato sul branch sbagliato

```bash
# Sposta l'ultimo commit su un nuovo branch
git branch nuovo-branch
git reset HEAD~1 --hard
git checkout nuovo-branch
```

### Ho scritto il messaggio di commit sbagliato (non ancora pushato)

```bash
git commit --amend -m "Messaggio corretto"
```

### Voglio annullare le modifiche a un file (non ancora committato)

```bash
git checkout -- nomefile
```

### Voglio tornare a un commit precedente (per vedere com'era)

```bash
git checkout <hash-commit>   # hash visibile con git log --oneline
git checkout main            # per tornare alla versione corrente
```

---

## .gitignore — file da non tracciare

Crea un file `.gitignore` nella root del progetto per escludere file inutili:

```
# Esempi comuni
build/
*.o
*.exe
.vscode/
__pycache__/
*.pyc
.env
node_modules/
```

Aggiungi e committa il `.gitignore` all'inizio del progetto.

---

## Riassunto del ciclo tipico

```bash
git pull origin main           # aggiornati
git checkout -b mia-feature    # crea branch
# ... lavora ...
git add .
git commit -m "Descrizione"
git push origin mia-feature    # manda su GitHub
# apri Pull Request su GitHub, fai merge
git checkout main
git pull origin main           # risincronizza
```
