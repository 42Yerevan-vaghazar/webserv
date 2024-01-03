NAME = webserver

UNAME := $(shell uname)

TMP = objs

ifeq ($(UNAME), Linux)
CXX = c++ -std=c++98
else
CXX = c++ -std=c++98
endif

CXXFLAGS = -I./includes -Wall -Wextra -Werror #-fsanitize=address -g 

SRCS = $(wildcard src/*.cpp)

OBJS = $(patsubst src/%.cpp, ./$(TMP)/%.o, $(SRCS))

RM = rm -fr

HEADER = $(wildcard *.hpp)
HEADER += $(wildcard includes/*.hpp)

all: $(NAME)

./$(TMP)/%.o: ./src/%.cpp $(HEADER) Makefile
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(TMP) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(TMP):
	@mkdir $(TMP)

clean:
	$(RM) $(OBJS_DIR)
	$(RM) $(TMP)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re bonus
