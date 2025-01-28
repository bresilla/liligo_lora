
build:
	pio -f -c vim run

upload:
	pio -f -c vim run -t upload

gen:
	pio project init --ide vim
