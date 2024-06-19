INC_DIR			= include/
OBJ_DIR			= obj/
SRC_DIR			= src/

CC				= c++

RM				= rm -f

NAME			= ircserv

CFLAGS			= -Wall -Wextra -Werror -std=c++98

SRCS			= $(addprefix $(SRC_DIR), \
					ft_irc.cpp \
					Server.cpp)

OBJS			= $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.cpp=.o)))

DEPS			= Makefile $(addprefix $(INC_DIR), \
					ft_irc_error.hpp \
					ft_irc.hpp \
					Server.hpp)

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp $(DEPS) | $(OBJ_DIR)
				$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

clean:
				$(RM) $(OBJS)
				$(RM) -d $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
