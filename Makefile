INC_DIR         = include/
OBJ_DIR         = obj/
SRC_DIR         = src/

CC              = c++

RM              = rm -f

NAME            = ircserv

CFLAGS          = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

SRCS            = $(addprefix $(SRC_DIR), \
                    ft_irc.cpp \
                    Server.cpp \
                    Client.cpp \
                    SendMsg.cpp \
                    manageCmd.cpp \
                    Channel.cpp \
                    Cmd/cap.cpp \
                    Cmd/join.cpp \
                    Cmd/mode.cpp \
                    Cmd/nick.cpp \
                    Cmd/pass.cpp \
                    Cmd/ping.cpp \
                    Cmd/pong.cpp \
                    Cmd/privmsg.cpp \
                    Cmd/quit.cpp \
                    Cmd/topic.cpp \
                    Cmd/user.cpp \
                    Cmd/invite.cpp \
                    Cmd/part.cpp)

OBJS            = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.cpp=.o)))

DEPS            = $(addprefix $(INC_DIR), \
                    ft_irc.hpp \
                    Server.hpp \
                    Client.hpp \
                    SendMsg.hpp \
                    Channel.hpp )

all:            $(NAME)

$(NAME):        $(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o:  $(SRC_DIR)Cmd/%.cpp $(DEPS) | $(OBJ_DIR) $(OBJ_DIR)Cmd
				$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR)%.o:  $(SRC_DIR)%.cpp $(DEPS) | $(OBJ_DIR)
				$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

$(OBJ_DIR)Cmd:
				mkdir -p $(OBJ_DIR)Cmd

clean:
				$(RM) $(OBJS)
				$(RM) -r $(OBJ_DIR)

fclean:         clean
				$(RM) $(NAME)

re:             fclean all

.PHONY:         all clean fclean re

