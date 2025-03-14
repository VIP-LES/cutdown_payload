## Steps to build


__From project directory:__
```
python nanopb/generator/nanopb_generator.py cutdown-packets/messages.proto
sed -i '' 's|<pb\.h>|"../nanopb/pb.h"|g' cutdown-packets/*.h
```
