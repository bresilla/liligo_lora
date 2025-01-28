
build:
	@pio -f -c vim run

upload:
	@pio -f -c vim run -t upload

update:
	@pio project init --ide vim

serial:
	@pio device monitor --baud 115200
