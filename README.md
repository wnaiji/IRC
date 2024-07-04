# IRC
This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.


# CMD

-CAP => OK
-PASS => OK
-NICK => OK
-USER => OK
-PING => OK
-PONG => OK
-QUIT => OK
-TOPIC => tester TOPIC avec nc et irssi
-JOIN => -le nombre de client dans le channel
-PRIVMSG
        verifier pour toute les commande si le client est bien enregistre
   # Reprendre a partir de MODE est adapter le code en fonction de 
MODE
PART
INVITE
KICK

NOTICE
WHO
WHOIS

# DOC

protocol irc:
https://modern.ircdocs.horse/#pass-message


orga:
https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9