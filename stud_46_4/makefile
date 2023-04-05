FLAGS := -Wall  -Wpedantic -Iout
COMPILER := gcc

app.exe : out/main.o out/func.o
	$(COMPILER) $(FLAGS) $^ -o $@
	
time.exe : out/time.o out/func.o
	$(COMPILER) $(FLAGS) $^ -o $@

out/main.o : src/main.c src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

out/func.o : src/func.c src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

out/time.o : src/time.c src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@


.PHONY : clean
.PHONY : time
.PHONY : reset

time : time.exe

clean : 
	rm *.exe out/*.o
	
reset:
	rm docs/*.aux
