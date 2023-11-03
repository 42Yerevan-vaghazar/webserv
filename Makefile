NAME = webserver

UNAME := $(shell uname)

TMP = objs

ifeq ($(UNAME), Linux)
CXX = c++ -std=c++0x
else
CXX = c++ -std=c++98
endif

CXXFLAGS = -I./includes  #-Wall -Wextra #-Werror

SRCS = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, ./$(TMP)/%.o, $(SRCS))

RM = rm -fr

HEADER = $(wildcard *.hpp)

all: $(NAME)

./$(TMP)/%.o: %.cpp $(HEADER) Makefile
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
