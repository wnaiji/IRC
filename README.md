# IRC
This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.


# CMD

//sortie du server proprement

-CAP => OK
-PASS => OK
-NICK => OK
-USER => OK
-PING => OK
-PONG => OK
-QUIT => OK
-TOPIC => verifier l'otorisation avant
-JOIN =>        -quand un nouveau client rentre dans le channel il ne voie pas
                        la vrai liste de opperator
                -le nombre de client dans le channel
                -etablir le mode a la creation
                -faire un vector<string> de nameAdmin
                -3em arg => topic
-PRIVMSG
        verifier pour toute les commande si le client est bien enregistre
   # verifier que les message lors de la modification de NICK ou lors de l'envoie QUIT ne sois pas double si les clients ce retrouve sur plusieurs channel a la fois
MODE
PART
INVITE
KICK

NO
WHO
WHOIS

# DOC

protocol irc:
https://modern.ircdocs.horse/#pass-message


orga:
https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9