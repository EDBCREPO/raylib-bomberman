export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

#---------------------------------------------------------------------#

FLAG="-lssl -lcrypto -lz -lraylib -L./lib -I./include -Ofast" #-Ofast
LIST=( main )

#---------------------------------------------------------------------#

if [ ! -d "build" ]; then
    mkdir "build"
fi

#---------------------------------------------------------------------#

echo -e "compilling ungine"
for ITEM in "${LIST[@]}" ; do

    if [ -f "./build/$ITEM" ]; then
        echo "-> compiled   $ITEM"; continue
    fi; echo "-> compilling $ITEM"

    g++ -o ./build/$ITEM ./service/$ITEM.cpp $FLAG

    if [ ! $? -eq 0 ]; then
        echo "exit error"; exit;
    fi

done

#---------------------------------------------------------------------#

echo -e "running ungine" ; ./build/main
