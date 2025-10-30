# ft_irc

**ft_irc** est un serveur IRC entièrement développé en **C++98** utilisant des sockets TCP/IP et un multiplexage I/O via `poll()`.  
Il permet à plusieurs clients IRC de se connecter simultanément, de communiquer dans des channels et d’utiliser des commandes IRC classiques.

---

## 🧠 Objectifs du projet

- Comprendre et implémenter des **protocoles réseau** (IRC RFC 1459 inspiré)
- Gérer **plusieurs clients simultanément** sans blocage
- Utiliser des **socket non-bloquantes** et un seul `poll()`
- Implémenter un **parsing robuste** et un système d’événements réseau
- Approfondir la POO en C++98

---

## 🚀 Fonctionnalités obligatoires

✅ Connexion au serveur avec mot de passe  
✅ Authentification : `PASS` → `NICK` → `USER`  
✅ Gestion complète de plusieurs clients en parallèle  
✅ Channels avec messagerie broadcast  
✅ Opérateurs et permissions  
✅ Envoi de messages privés (`PRIVMSG`)  
✅ Commandes IRC supportées :

| Commande | Description |
|----------|-------------|
| `JOIN` | Rejoindre un channel |
| `PART` | Quitter un channel |
| `PRIVMSG` | Message privé ou channel |
| `KICK` | Éjecter un utilisateur |
| `INVITE` | Inviter un utilisateur |
| `TOPIC` | Changer / afficher le topic |
| `MODE` | Gérer les modes du channel |

Modes implémentés :

- `i` : Invite-only
- `t` : Restriction `TOPIC` aux opérateurs
- `k` : Channel protégé par mot de passe
- `o` : Ajout / retrait d’opérateur
- `l` : Limite d’utilisateurs

---

## 🧩 Architecture technique

*(À adapter avec tes noms de classes réels)*

- `Server` → socket, poll, routing des événements
- `Client` → état d’authentification, nickname, FD…
- `Channel` → membres, opérateurs, modes, topic
- `Parser` → découpe et interprète les messages IRC
- `CommandHandler` → exécution des commandes

📌 Aucun `fork()`, aucun thread → **serveur 100% event-driven**

---

## 🛠 Compilation & Exécution

```sh
make
./ircserv <port> <password>

./ircserv 6667 mypass

nc -C 127.0.0.1 6667
PASS mypass
NICK jen
USER jen 0 * :Jennifer
JOIN #42
PRIVMSG #42 :Hello from my IRC server!
