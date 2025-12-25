TARGET	:=	oil

all: $(TARGET)

install:
	install ./$(TARGET) /usr/local/bin/

$(TARGET): main.cpp
	clang++ main.cpp -std=c++20 -o $(TARGET)
