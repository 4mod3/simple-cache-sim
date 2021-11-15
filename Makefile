build:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean
	rm -f src/cache_sim

run: 
	./cache_sim

.PHONY: clean build run

