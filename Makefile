CXX ?= g++
CFLAGS += -O2 -std=c++11 -Wall -Wextra -Werror -Iinclude/
LDFLAGS += -lcurl -lprotobuf
TESTBENCH = testbench

SRCS = src/client.cc src/status.cc src/transport.cc src/plaid.pb.cc

test:
	$(CXX) $(CFLAGS) testbench.cc $(SRCS) $(LDFLAGS) -o $(TESTBENCH)
clean:
	$(RM) $(TESTBENCH)