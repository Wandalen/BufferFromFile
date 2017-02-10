
all:
	npm update && node-gyp configure && node-gyp rebuild

clean:
	rm -rf build
