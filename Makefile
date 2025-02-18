CXX = g++
CXXFLAGS = -Wall -std=c++20
EXENAME = exec
SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(SRCS:%.cpp=%.d)

all: $(EXENAME)

$(EXENAME) : $(OBJS)
	$(CXX) $^ -o $@

-include $(DEPS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -rf *.o *~ *.d

cleanall : clean
	rm -f $(EXENAME)
