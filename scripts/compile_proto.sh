#!/bin/bash

# This small script compiles the plaid.proto file and moves files.
# We also re-write an include path in plaid.pb.cc.

protoc -I . --cpp_out=. plaid.proto
sed -i "s/#include \"plaid.pb.h\"/#include \"plaid\/plaid.pb.h\"/g" plaid.pb.cc
mv plaid.pb.cc src/plaid.pb.cc
mv plaid.pb.h include/plaid/plaid.pb.h

