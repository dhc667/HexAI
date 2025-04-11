if [ -d wrapper/libgetmove.so ]
then
	rm wrapper/libgetmove.so
fi
g++ ./src/get_move.cpp -o wrapper/libgetmove.so -shared -fPIC -Wextra -Wall -O2
