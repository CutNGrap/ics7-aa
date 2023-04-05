FLAGS := -Wall  -Wpedantic -Iout
COMPILER := g++

app.exe : main.o func.o
	$(COMPILER) $(FLAGS) $^ -o $@
	
time.exe : time.o func.o
	$(COMPILER) $(FLAGS) $^ -o $@

main.o : src/main.cpp src/func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

func.o : func.cpp func.h
	$(COMPILER) $(FLAGS) $< -c -o $@

time.o : time.cpp func.h
	$(COMPILER) $(FLAGS) $< -c -o $@


.PHONY : clean
.PHONY : time
.PHONY : reset

time : time.exe

clean : 
	rm *.exe out/*.o
	
reset:
	rm docs/*.aux
