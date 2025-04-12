if [ -d wrapper/libgetmove.so ]
then
	rm wrapper/libgetmove.so
fi
g++ ./src/get_move.cpp -o wrapper/libgetmove.so -shared -fPIC  -O2

cd wrapper
b2sum libgetmove.so > libgetmove.so.sum
