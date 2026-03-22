#pragma once

#include "model.h"
#include "view.h"

namespace Fifteen {

    class Controller {
        Model model;
        View view;

    public:
        Controller(int argc, char *argv[]) : view(argc, argv) {
            QObject::connect(&view.newGameButton, &QPushButton::clicked, [this] {
                model.shuffle();
                view.reset();
                view.drawField(model.getField());
            });

            for (auto y = 0u; y < SIZE; ++y) {
                for (auto x = 0u; x < SIZE; ++x) {
                    auto button = view.buttons[y][x];
                    QObject::connect(button, &QPushButton::clicked, [this, x, y] {
                        model.move(Coords(x, y));
                        view.drawField(model.getField());
                        if (model.checkWin()) view.win();
                    });
                }
            }
        }

        int run() {
            return view.run(model.getField());
        }
    };
}
