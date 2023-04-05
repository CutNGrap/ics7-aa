FLAGS := -Wall  -Wpedantic -Iout
COMPILER := gcc

app.exe : out/main.o out/func.o out/utilities.o
	$(COMPILER) $(FLAGS) $^ -o $@
	
time.exe : out/func.o out/time.o
	$(COMPILER) $(FLAGS) $^ -o $@

out/main.o : src/main.c src/consts.h src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

out/func.o : src/func.c src/consts.h src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

out/utilities.o : src/utilities.c src/consts.h src/utilities.h
	$(COMPILER) $(FLAGS) $< -c -o $@
	
out/time.o : src/time.c src/consts.h src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

.PHONY : clean
.PHONY : time
.PHONY : reset

time : time.exe
	./time.exe

clean : 
	rm *.exe out/*.o
	
reset:
	rm docs/*.aux
