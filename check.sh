g++ @opcjeCpp.txt main.cpp worki.cpp -o main
g++ -std=c++23 brute.cpp -o brute
g++ -std=c++23 gen2.cpp -o gen

for((i=0;i<=20000;i++))
do
    echo "test $i"
    ./gen > t
    MallocNanoZone=0 ./main < t > t.m
    ./brute < t > t.b
    diff -b t.m t.b || break
    echo "OK"
    echo ""
done 