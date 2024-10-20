[ -z "$1" ] && exit 1

NAME=$1
shift

make $NAME > /dev/null
make -C fifodiff > /dev/null

./fifodiff/fifodiff "./ft_$NAME $*" "./std_$NAME $*"
