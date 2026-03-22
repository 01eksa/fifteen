#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <format>
#include <random>


namespace Fifteen {

    constexpr int SIZE = 4;
    using Field = std::array<std::array<int32_t, SIZE>, SIZE>;

    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    inline std::uniform_int_distribution randCoord(0, SIZE - 1);
    inline std::uniform_int_distribution randReps(128, 512);
    inline std::uniform_int_distribution randBool(0, 1);

    struct Coords {
        uint32_t x;
        uint32_t y;

        bool operator==(const Coords &coords) const {
            return x == coords.x && y == coords.y;
        }

        bool operator!=(const Coords &coords) const {
            return x != coords.x || y != coords.y;
        }
    };

    class Model {
        Field win_field{
            {
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12},
                {13, 14, 15, 0}
            }
        };
        Field field = win_field;
        Coords empty_coords{3, 3};

    public:
        Model() {
            shuffle();
        }

        Field getField() const {
            return field;
        }

        void shuffle() {
            for (int i = 0; i < randReps(gen); i++) {
                auto coords = empty_coords;

                if (randBool(gen))
                    coords.x = randCoord(gen);
                else
                    coords.y = randCoord(gen);

                move(coords);
            }
        }

        bool move(Coords coords) {
            if (coords == empty_coords) return false;
            if (coords.x != empty_coords.x && coords.y != empty_coords.y) return false;

            if (coords.x != empty_coords.x) {
                const auto step_x = empty_coords.x < coords.x ? 1 : -1;

                for (auto x = empty_coords.x; x != coords.x; x += step_x) {
                    field[coords.y][x] = field[coords.y][x + step_x];
                }
            } else if (coords.y != empty_coords.y) {
                const auto step_y = empty_coords.y < coords.y ? 1 : -1;

                for (auto y = empty_coords.y; y != coords.y; y += step_y) {
                    field[y][coords.x] = field[y + step_y][coords.x];
                }
            }

            field[coords.y][coords.x] = 0;
            empty_coords = coords;

            return true;
        }

        [[nodiscard]] bool checkWin() const {
            return field == win_field;
        }

        [[nodiscard]] std::string toString() const {
            std::string result;

            for (auto y = 0; y < SIZE; y++) {
                for (auto x = 0; x < SIZE; x++) {
                    result += std::format("{:<2} ", field[y][x]);
                }
                result += "\n";
            }

            return result;
        }
    };
}
