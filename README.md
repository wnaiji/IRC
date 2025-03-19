# ft_irc

Un **serveur IRC** en C++ (norme C++98) permettant la communication instantanée via le protocole **Internet Relay Chat**.  
Le projet vise à implémenter les fonctionnalités clés d’un serveur IRC tout en restant compatible avec un client IRC standard.

## Fonctionnalités Principales
- **Gestion de multiples clients simultanés** sans blocage (non-blocking I/O).
- **Identification** avec mot de passe, définition d’un pseudo (**nickname**) et d’un nom d’utilisateur (**username**).
- **Rejoint de canaux** et **échanges de messages** entre les utilisateurs d’un même canal.
- **Opérateurs** : implémente les commandes IRC essentielles (`KICK`, `INVITE`, `TOPIC`, `MODE`, etc.).
- **Communication en TCP/IP** (v4 ou v6) via un seul appel à `poll()` ou équivalent (pas de `fork()`).
