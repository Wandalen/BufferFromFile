
all:
	npm update && node-gyp configure && node-gyp rebuild --msvs_version=auto

clean:
	rm -rf build
