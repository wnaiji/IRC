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
-TOPIC => OK
-JOIN =>        adapter en fonction de mode
   # verifier que les message lors de la modification de NICK ou lors de l'envoie QUIT ne sois pas double si les clients ce retrouve sur plusieurs channel a la fois
-PRIVMSG => OK
MODE => - + et -l => OK
        - + et -k => OK
        - + et -t => OK
        - + et -o => OK
        - i => coder INVITE
INVITE
PART
KICK

NO
WHO
WHOIS

# DOC

protocol irc:
https://modern.ircdocs.horse/#pass-message


orga:
https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9