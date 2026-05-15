# 🎮 Two-Player Strategy Game (C)

Petit jeu de stratégie au tour par tour développé en C dans le cadre d’une SAÉ.

---

## 🧠 Concept

Le jeu se joue à **deux joueurs** sur un plateau **6×6**.

Chaque joueur contrôle **6 pièces de tailles différentes**.  
L’objectif est simple :

👉 Amener **une pièce jusqu’au côté opposé du plateau** pour gagner.

Les joueurs jouent chacun leur tour en déplaçant leurs pièces stratégiquement.

---

## ⚙️ Compilation et lancement

Pour compiler et exécuter le jeu :

```bash
gcc -Wall -c board.c -o board.o
gcc -Wall board.o -o game
./game
```

---

## 🎲 Déroulement du jeu

### 1. Mise en place

Au début de la partie, chaque joueur place ses pièces alternativement :

- 2 pièces de chaque taille
- Placement par colonne (1 à 6)
- Le jeu choisit automatiquement la ligne côté joueur

🟢 Taille 1 → déplacement de 1 case  
🟡 Taille 2 → déplacement de 2 cases  
🔴 Taille 3 → déplacement de 3 cases  

---

### 2. Tour de jeu

À chaque tour, un joueur :

1. Sélectionne une pièce (ligne + colonne)
2. Choisit une direction possible :
   - ↑ Nord  
   - ↓ Sud  
   - → Est  
   - ← Ouest  
3. Déplace la pièce selon sa taille (nombre de cases max)

Commandes spéciales :
- `A` → annuler le mouvement
- `R` → annuler le dernier déplacement

---

### 3. Déplacement et règles

- Une pièce ne peut se déplacer que si le mouvement est valide
- Le jeu vérifie automatiquement les collisions et limites
- Les erreurs de déplacement sont refusées

---

### 4. Swap (échange spécial)

Si une pièce termine son mouvement sur une autre pièce :

- Le joueur peut choisir d’effectuer un **swap**
- Confirmation :
  - `O` → oui
  - `N` → non
- Puis sélection de la pièce à échanger

---

### 5. Fin de partie

Le jeu se termine lorsqu’un joueur atteint le côté opposé du plateau avec une pièce.

🎉 Un message de victoire est affiché.

---

## 🛠️ Structure du projet

- `main.c` : boucle principale du jeu
- `board.c` : logique du plateau et des règles
- `board.h` : définitions et structures
- `game` : exécutable généré

---

## 💡 Conseils

- Anticiper les mouvements adverses
- Bien gérer les pièces de grande taille
- Utiliser `A` et `R` en cas d’erreur
- Contrôler les positions clés du plateau

---

## 🚀 Objectif du projet

Ce projet a été réalisé pour pratiquer :

- la programmation en C
- la gestion de structures et de logique de jeu
- la manipulation de plateau
- la compilation avec GCC
