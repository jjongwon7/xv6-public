#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[])
{
    trace(atoi(argv[1]));
    exec(argv[2], &argv[2]);
    wait();
    exit();
}