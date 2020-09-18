CXXFLAGS = -Wall -std=c++11 -Wno-deprecated -g0 -gdwarf-2  -Winvalid-pch -Werror -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Wcast-qual -Wundef -Wredundant-decls -Wno-reorder -Wno-unused-parameter #-Winline
INCLUDE=$(BASE_INC) -I../src -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0
LIB=$(BASE_LIB) ../libmongo.a -lbson-1.0 -lmongoc-1.0 

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)

TARGET=testMongo

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ $(LIB) -o $@

-include $(DEPS) 

%.o:%.cpp
	#@mkdir -p .deps/$*; 
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) $(VERSION) $< -o $@ -MMD -MF $*.d  -MP

clean: 
	@rm -f $(TARGET) */*.o */*.d -rf

distclean:
	@rm -f $(TARGET) */*.d -rf

.PHONY: all clean
