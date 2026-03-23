#pragma once

#include "model.h"
#include "view.h"

namespace Fifteen {

    class Controller {
        Model model;
        View view;

    public:
        Controller(int argc, char *argv[]) : view(argc, argv) {
            view.connectNewGameButton([this] {
                model.shuffle();
                view.reset();
                view.drawField(model.getField());
            });

            view.connectField([this] (int x, int y) {
                model.move(Coords(x, y));
                view.drawField(model.getField());
                if (model.checkWin()) view.win();
            });
        }

        int run() {
            return view.run(model.getField());
        }
    };
}
