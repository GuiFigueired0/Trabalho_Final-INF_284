all: ts aco

run_all: run_ts run_aco
 
ts:
	g++ ts.cpp classes/gcp.cpp classes/ts.cpp -o bin/ts

aco:
	g++ aco.cpp classes/gcp.cpp classes/aco.cpp -o bin/aco

run_ts:
	./bin/ts
	
run_aco:
	./bin/aco
