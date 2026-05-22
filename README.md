# Istruzioni Git — ProgettoPCS

---

## 1. Scaricare il progetto (solo la prima volta)

```bash
git clone https://github.com/ScaccoFranco/ProgettoPCS.git
cd ProgettoPCS
```

---

## 2. Aggiornare la repo locale (ogni volta che apri il progetto)

```bash
git checkout main
git pull origin main
```

---

## 3. Caricare le proprie modifiche su GitHub

```bash
# 1. Crea il tuo branch (solo la prima volta per ogni nuova funzionalità)
git checkout -b nome-branch

# 2. Aggiungi i file modificati
git add .

# 3. Salva le modifiche con un messaggio
git commit -m "Descrivi cosa hai fatto"

# 4. Manda il branch su GitHub
git push origin nome-branch
```

Poi vai su [github.com/ScaccoFranco/ProgettoPCS](https://github.com/ScaccoFranco/ProgettoPCS),
clicca **"Compare & pull request"** e apri la PR.

---

> **Regola d'oro:** fai sempre `git pull origin main` prima di iniziare a lavorare.