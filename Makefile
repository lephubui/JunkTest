all: remove shell

shell:
	gcc setkv.c hashmap.c -o alias

remove:
	rm alias