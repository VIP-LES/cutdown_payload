## Steps to build
```
mkdir generated
protoc -I=cutdown-packets/protobuf/ --cpp_out=generated cutdown-packets/protobuf/messages.proto
```
