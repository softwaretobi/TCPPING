Client TCP Ping en C
Description

Ce projet implémente un client de ping TCP simple, écrit en C. Il tente d'établir une connexion TCP à un hôte et un port spécifiés, à des intervalles d'une seconde, en mesurant les temps de connexion et en collectant des statistiques, telles que :

    Tentatives totales
    Connexions réussies
    Connexions échouées
    Temps de connexion minimum, maximum et moyen

Le script fonctionne de manière similaire à un ping ICMP, mais utilise des connexions TCP, ce qui le rend utile pour vérifier la disponibilité de services fonctionnant sur des ports TCP (comme des serveurs web ou des bases de données).
Fonctionnalités

    Tente de se connecter à un hôte et un port spécifiés via le protocole TCP.
    Affiche les temps de connexion en millisecondes.
    Suivi des statistiques sur les tentatives de connexion réussies et échouées.
    Affiche les temps de connexion minimum, maximum et moyen.
    Gère le signal SIGINT (Ctrl+C) pour afficher un rapport des statistiques avant de quitter.

Comment ça fonctionne

    Le programme prend deux arguments en ligne de commande :
        host : Le nom de domaine ou l'adresse IP de la cible (par exemple, google.com ou 192.168.1.1).
        port : Le port TCP à tester (par exemple, 80 pour HTTP, 443 pour HTTPS).

    Une fois lancé, le programme tente de se connecter à l'hôte et au port donnés toutes les secondes.

    Il enregistre si la connexion a réussi ou échoué et mesure le temps de connexion en millisecondes.

    Lorsque l'utilisateur presse Ctrl+C, le programme s'arrête et affiche un rapport récapitulatif des statistiques.

Compilation

Pour compiler le script, vous aurez besoin d'un compilateur C tel que gcc.

## Installation 
- `git clone https://github.com/softwaretobi/TCPPING.git`
- `gcc ping_client.c -o ping_client`

## Utilisation

Après la compilation, exécutez le programme avec le format suivant :

`./tcp <host> <port>`

