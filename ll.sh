printf "\033c\033[43;30m\n"
clang -S -emit-llvm -o $1.ll $1
 