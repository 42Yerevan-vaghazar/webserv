NAME = webserv
CPP = c++
CPP_FLAGS = -I includes/ -std=c++98 #-fsanitize=address -g -Wall -Wextra -Werror
SRC = $(shell find . -name "*.cpp")
OBJ = $(SRC:.cpp=.o)
HEADER = $(shell find . -name "*.hpp")
all : $(NAME)

$(NAME) : $(OBJ)
	$(CPP) $(CPP_FLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp $(HEADER)
	$(CPP) $(CPP_FLAGS) -c $< -o $@

fclean : clean
	rm -rf $(NAME)

clean :
	rm -rf $(OBJ)
re : fclean all

.PHONNY: clean fclean all re
