# IRC
This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.


# CMD

-CAP
-PASS
-NICK
-USER
-PING
PONG
QUIT
JOIN
PART
TOPIC
KICK
INVITE

    this->_FunLst["PING"] = &server::pingFun;
    this->_FunLst["NICK"] = &server::nickFun;
    this->_FunLst["USER"] = &server::userFun;
    this->_FunLst["PASS"] = &server::passFun;
    this->_FunLst["CAP"] =  &server::capFun;
    this->_FunLst["PRIVMSG"] = &server::privmsgFun;
    this->_FunLst["JOIN"] =  &server::joinFun;
    this->_FunLst["MODE"] = &server::modeFun;
    this->_FunLst["QUIT"] =  &server::quitFun;
    this->_FunLst["PART"] = &server::partFun;
    this->_FunLst["NOTICE"] = &server::noticeFun;
    this->_FunLst["WHO"] = &server::whoFun;
    this->_FunLst["WHOIS"] = &server::whoFun;
    this->_FunLst["TOPIC"] = &server::topicFun;
    this->_FunLst["INVITE"] = &server::inviteFun;
    this->_FunLst["KICK"] = &server::kickFun;

# DOC

protocol irc:
https://modern.ircdocs.horse/#pass-message


orga:
https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9