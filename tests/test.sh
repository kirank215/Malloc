#!/bin/sh
load="LD_PRELOAD=./libmalloc.so"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;36m'
NC='\033[0m;'
calltest()
{
    printf "\n"
    echo "--  COMMAND "$1"  --"
    #mymalloc=$(LD_PRELOAD=./libmalloc.so $1) 
    #malloc=$($1) 
    #printf "\n"
    echo -e " ${YELLOW}My times${NC} "
    mytime=$(time (LD_PRELOAD=./libmalloc.so $1))
    error=$?
    printf "\n"
    echo -e " ${YELLOW}Normal times${NC} "
    normaltime=$(time ($1) 1>/dev/null)
    if [ $error != 0 ]; then
        printf "\n"
        echo -e " $1 : ${RED}YOUR MALLOC FAILED${NC} "
        printf "\n"
    else
        printf "\n"
        echo -e "${GREEN}Test $1 : OK ${NC}"
        printf "\n"
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

echo " ------TEST WITH 'ip'----- "
printf "\n"

calltest "ip a"
printf "\n"


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

echo " ------TEST WITH git----- "
printf "\n"

declare -a cmd=( "git status" "git log" )
for i in "${cmd[@]}"
do
    calltest "$i" 
done
printf "\n"
    
echo " ------TEST WITH cat----- "
printf "\n"

calltest "cat Makefile"

printf "\n"

echo " ------TEST WITH od library----- "
printf "\n"

calltest "od libmalloc.so"


echo " ------TEST WITH 'LESS'----- "
printf "\n"

calltest "less Makefile"
printf "\n"


echo " ------TEST WITH 'SELF COMPILE'----- "
printf "\n"

calltest "make all"
printf "\n"

echo " ------TEST WITH gimp----- "
printf "\n"

calltest "gimp"
printf "\n"

echo " ------TEST WITH Chromium----- "
printf "\n"

calltest "chromium"
printf "\n"
