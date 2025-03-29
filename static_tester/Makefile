
INCLUDE_PATH := ../src # Path to your headers

CXXFLAGS ?= -Wall -Werror -Wextra -Wno-deprecated-copy

SRCS_DIR = srcs

# FILES = $(wildcard ${SRCS_DIR}/*.cpp)
FILES = ${SRCS_DIR}/list.cpp ${SRCS_DIR}/vector.cpp ${SRCS_DIR}/map.cpp ${SRCS_DIR}/multimap.cpp ${SRCS_DIR}/set.cpp

CONTAINERS = $(FILES:${SRCS_DIR}/%.cpp=%)

all: ${CONTAINERS}

${CONTAINERS}:
	c++ -fsyntax-only ${CXXFLAGS} -I ${INCLUDE_PATH} -D NAMESPACE=ft ${SRCS_DIR}/$@.cpp
