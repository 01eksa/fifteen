#include "controller.h"

int main(int argc, char *argv[]) {
    Fifteen::Controller controller(argc, argv);
    return controller.run();
}
