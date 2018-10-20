#!/bin/sh
load="LD_PRELOAD=./libmalloc.so"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;36m'
NC='\033[0m;'
calltest()
{
    printf "\n"
    echo "--  COMMAND "$i"  --"
    mymalloc=$(LD_PRELOAD=./libmalloc.so $1) 
    malloc=$($1) 
    printf "\n"
    echo -e " ${YELLOW}My times${NC} "
    mytime=$(time (LD_PRELOAD=./libmalloc.so $1))
    printf "\n"
    echo -e " ${YELLOW}Normal times${NC} "
    normaltime=$(time ($1) 1>/dev/null)
    if [ "$mymalloc" = "$malloc" ]; then 
        printf "\n"
        echo -e "${GREEN}Test $1 : OK ${NC}"
    else
        printf "\n"
        echo -e " $1 : ${RED}YOUR MALLOC FAILED${NC} "
    fi
}
echo " ------TEST WITH FACTOR----- "
printf "\n"

declare -a cmd=( "factor 20" "factor 30" "factor 40" "factor 50" )
for i in "${cmd[@]}"
do
    calltest "$i" 
done


echo " ------TEST WITH LS----- "
printf "\n"

declare -a cmd=( "ls" "ls -l" "ls -la" "ls -R" )
for i in "${cmd[@]}"
do
    calltest "$i" 
done

echo " ------TEST WITH TAR----- "
printf "\n"


calltest "tar -cf malloc.tar libmalloc.so"



echo " ------TEST WITH FIND----- "
printf "\n"

declare -a cmd=( "find" "find -L" "find ../" "find ../.." "find -P" )
for i in "${cmd[@]}"
do
    calltest "$i" 
done


echo " ------TEST WITH TREE----- "
printf "\n"

declare -a cmd=( "tree" "tree ../" "tree ../.." )
for i in "${cmd[@]}"
do
    calltest "$i" 
done
