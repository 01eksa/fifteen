#pragma once

#include <array>
#include <functional>

#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QElapsedTimer>
#include <QTimer>

#include "model.h"

namespace Fifteen {

    class View {
        QApplication app;
        QWidget window;

        QGridLayout mainLayout;
        QGridLayout menuLayout;
        QGridLayout fieldLayout;

        QLabel statusLabel;
        QLabel timeLabel;

        QTimer timer;
        QElapsedTimer elapsedTimer;
        qint64 accumulatedTime = 0;

        QPushButton pauseButton;
        bool onPause = false;
        bool inGame = false;

        QPushButton newGameButton;
        std::array<std::array<QPushButton*, SIZE>, SIZE> buttons{};

    public:

        View(int argc, char *argv[]) : app(argc, argv) {
            QApplication::setStyle("Fusion");

            window.setWindowTitle("Fifteen Puzzle");
            window.setWindowIcon(QIcon(":/15.png"));
            window.setLayout(&mainLayout);

            initLayouts();
            initMenu();
            initField();
            initConnects();

            reset();
        }

        void connectNewGameButton(std::function<void()> func) {
            QObject::connect(&newGameButton, &QPushButton::clicked, func);
        }

        void connectField(std::function<void(int, int)> func) {
            for (auto y = 0u; y < SIZE; ++y) {
                for (auto x = 0u; x < SIZE; ++x) {
                    auto button = buttons[y][x];
                    QObject::connect(button, &QPushButton::clicked, [func, x, y] {
                        func(x, y);
                    });
                }
            }
        }

        void reset() {
            statusLabel.setText("Good luck!");
            pauseButton.setText("Pause");
            timeLabel.setText("00:00");

            accumulatedTime = 0;
            inGame = true;
            onPause = false;

            enableField();
            elapsedTimer.start();
            timer.start();
        }

        void drawField(const Field &field) const {
            for (int y = 0; y < SIZE; y++) {
                for (int x = 0; x < SIZE; x++) {
                    if (const auto value = field[y][x]) {
                        buttons[y][x]->setText(QString::number(value));
                        buttons[y][x]->show();
                    }
                    else {
                        buttons[y][x]->clearFocus();
                        buttons[y][x]->hide();
                    }
                }
            }
        }

        void win() {
            inGame = false;
            timer.stop();
            statusLabel.setText("You won!");
            disableField();
        }

        int run(const Field &field) {
            drawField(field);
            window.show();
            return QApplication::exec();
        }

    private:

        void initLayouts() {
            mainLayout.addLayout(&menuLayout, 0, 0);
            mainLayout.addLayout(&fieldLayout, 1, 0);
        }

        void initMenu() {
            menuLayout.addWidget(&newGameButton, 0, 0);
            menuLayout.addWidget(&pauseButton, 0, 1);
            menuLayout.addWidget(&statusLabel, 0, 2);
            menuLayout.addWidget(&timeLabel, 0, 3);
            menuLayout.setSpacing(0);

            newGameButton.setText("New Game");
            newGameButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            newGameButton.setStyleSheet("margin: 0; padding: 0; font-size: 12px; min-width: 32px; min-height: 24px; max-height: 32px");
            pauseButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            pauseButton.setStyleSheet("margin: 0; padding: 0; font-size: 12px; min-width: 32px; min-height: 24px; max-height: 32px");

            statusLabel.setAlignment(Qt::AlignCenter);
            statusLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            timeLabel.setAlignment(Qt::AlignCenter);
            timeLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        }

        void initField() {
            fieldLayout.setContentsMargins(0, 0, 0, 0);
            fieldLayout.setSpacing(0);

            for (int y = 0; y < SIZE; y++) {
                fieldLayout.setColumnStretch(y, 1);
                fieldLayout.setRowStretch(y, 1);

                for (int x = 0; x < SIZE; x++) {
                    auto *button = new QPushButton(&window);
                    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    button->setStyleSheet("margin: 0; padding: auto; font-size: 18px; min-width: 32px; min-height: 32px");
                    buttons[y][x] = button;
                    fieldLayout.addWidget(button, y, x);
                }
            }
        }

        void initConnects() {
            QObject::connect(&timer, &QTimer::timeout, [this]() {
                this->updateTime();
            });

            QObject::connect(&pauseButton, &QPushButton::clicked, [this]() {
                this->pause();
            });
        }

        void disableField() const {
            for (auto row : buttons) {
                for (auto button : row)
                    button->setEnabled(false);
            }
        }

        void enableField() const {
            for (auto row : buttons) {
                for (auto button : row)
                    button->setEnabled(true);
            }
        }

        void updateTime() {
            const auto deltaTime = (accumulatedTime + elapsedTimer.elapsed()) / 1000;
            const auto minutes = deltaTime / 60;
            const auto seconds = deltaTime % 60;
            const QString timeStr = QString("%1:%2")
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));

            timeLabel.setText(timeStr);
        }

        void pause() {
            if (!inGame) return;

            if (onPause) {
                pauseButton.setText("Pause");
                enableField();
                timer.start(1000);
                elapsedTimer.start();
            }
            else {
                pauseButton.setText("Continue");
                timer.stop();
                disableField();
                accumulatedTime += elapsedTimer.elapsed();
            }

            onPause = !onPause;
        }
    };
}
