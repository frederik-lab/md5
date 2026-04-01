#!/bin/bash
COUNT=10000
MSG="testmsg"

# Your md5
START=$(date +%s%N)
for i in $(seq 1 $COUNT); do
    ./bin/app "$MSG" > /dev/null
done
END=$(date +%s%N)
SECONDS_ELAPSED=$(echo "scale=9; ($END - $START) / 1000000000" | bc)
echo -n "Your md5: "
echo "scale=6; $COUNT / $SECONDS_ELAPSED / 1000000" | bc
echo "MH/s"

# md5sum
START=$(date +%s%N)
for i in $(seq 1 $COUNT); do
    echo -n "$MSG" | md5sum > /dev/null
done
END=$(date +%s%N)
SECONDS_ELAPSED=$(echo "scale=9; ($END - $START) / 1000000000" | bc)
echo -n "md5sum: "
echo "scale=6; $COUNT / $SECONDS_ELAPSED / 1000000" | bc
echo "MH/s"