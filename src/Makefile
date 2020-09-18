CXXFLAGS = -Wall -std=c++11 -Wno-deprecated -g0 -gdwarf-2  -Winvalid-pch -Werror -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Wcast-qual -Wundef -Wredundant-decls -Wno-reorder -Wno-unused-parameter #-Winline

INCLUDE=$(BASE_INC) -I.. -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0

TARGET=libmongo.a

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
DEPS=$(SRCS:%.cpp=.%.d)

all: $(TARGET)

-include $(DEPS) 

%.o:%.cpp
	@mkdir -p .deps
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) $(VERSION) $< -o $@ -MMD -MF .$*.d -MP 

clean: 
	@rm -f $(TARGET) *.o 

distclean: 
	@rm -f $(TARGET) *.o .*.d

$(TARGET): $(OBJS)
	@rm -rf $(TARGET)
	@ar cru $(TARGET) $(OBJS)
	@ranlib $(TARGET)
	@mv $(TARGET) ..

.PHONY : all clean
