#pragma once

#include <array>

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

    public:
        QPushButton newGameButton;
        std::array<std::array<QPushButton*, SIZE>, SIZE> buttons{};

        View(int argc, char *argv[]) : app(argc, argv) {
            window.setWindowTitle("Fifteen Puzzle");
            //window.resize(320, 360);
            window.setWindowIcon(QIcon("../assets/15.png"));

            // mainLayout.setContentsMargins(0, 0, 0, 0);
            // mainLayout.setSpacing(0);

            mainLayout.addLayout(&menuLayout, 0, 0);
            mainLayout.addLayout(&fieldLayout, 1, 0);

            window.setLayout(&mainLayout);

            menuLayout.addWidget(&newGameButton, 0, 0);
            menuLayout.addWidget(&pauseButton, 0, 1);
            menuLayout.addWidget(&statusLabel, 0, 2);
            menuLayout.addWidget(&timeLabel, 0, 3);

            newGameButton.setText("New Game");
            statusLabel.setAlignment(Qt::AlignCenter);
            timeLabel.setAlignment(Qt::AlignCenter);
            reset();

            fieldLayout.setContentsMargins(0, 0, 0, 0);
            fieldLayout.setSpacing(0);
            initField();

            QObject::connect(&timer, &QTimer::timeout, [this]() {
                this->updateTime();
            });

            QObject::connect(&pauseButton, &QPushButton::clicked, [this]() {
                this->pause();
            });

            timer.start(1000);
        }

        void initField() {
            for (int y = 0; y < SIZE; y++) {
                fieldLayout.setColumnStretch(y, 1);
                fieldLayout.setRowStretch(y, 1);

                for (int x = 0; x < SIZE; x++) {
                    QPushButton *button = new QPushButton(&window);
                    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    button->setStyleSheet("margin: 0; padding: auto; font-size: 18px; min-width: 32px; min-height: 32px");
                    buttons[y][x] = button;
                    fieldLayout.addWidget(button, y, x);
                }
            }
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

        void reset() {
            statusLabel.setText("Good luck!");
            pauseButton.setText("Pause");
            timeLabel.setText("00:00");
            elapsedTimer.start();
            timer.start();
            inGame = true;
            onPause = false;
        }

        void win() {
            inGame = false;
            timer.stop();
            statusLabel.setText("You won!");
        }

        void updateTime() {
            const auto deltaTime = (accumulatedTime + elapsedTimer.elapsed()) / 1000;
            const int minutes = deltaTime / 60;
            const int seconds = deltaTime % 60;
            const QString timeStr = QString("%1:%2")
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));

            timeLabel.setText(timeStr);
        }

        void pause() {
            if (!inGame) return;

            if (onPause) {
                pauseButton.setText("Pause");
                timer.start(1000);
                elapsedTimer.start();
            }
            else {
                pauseButton.setText("Continue");
                timer.stop();
                accumulatedTime += elapsedTimer.elapsed();
            }

            onPause = !onPause;
        }

        int run(const Field &field) {
            drawField(field);
            window.show();
            return app.exec();
        }
    };
}
