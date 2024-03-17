#include "surakarta_rule_manager.h"
#include <iostream>

void SurakartaRuleManager::OnUpdateBoard() {
    // TODO:
    // Every time the board and game_info is updated to the next round version, this function will be called.
    // You don't need to implement this function if you don't need it.
    // A more delicate way is to use Qt's signal and slot mechanism, but that's the advanced part.
}

SurakartaIllegalMoveReason SurakartaRuleManager::JudgeMove(const SurakartaMove& move) {
    // TODO: Implement this function.

    // 获取移动的起始位置和目标位置
    SurakartaPosition from = move.from;
    SurakartaPosition to = move.to;

    // 判断移动是否在棋盘内
    if ((int)from.x < 0 || from.x >= board_size_ || (int)from.y < 0 || from.y >= board_size_ ||
        (int)to.x < 0 || to.x >= board_size_ || (int)to.y < 0 || to.y >= board_size_) {
        return SurakartaIllegalMoveReason::OUT_OF_BOARD;
    }
    if ((*board_)[from.x][from.y]->GetColor() == SurakartaPlayer::NONE) {
        return SurakartaIllegalMoveReason::NOT_PIECE;
    }

    SurakartaPlayer current_player = game_info_->current_player_;

    if ((*board_)[from.x][from.y]->GetColor() != current_player) {
        return SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
    }

    PieceColor to_piece_color = (*board_)[move.to.x][move.to.y]->GetColor();
    // PieceColor from_piece_color = (*board_)[move.from.x][move.from.y]->GetColor();
    if (to_piece_color == PieceColor::NONE) {
        if (abs(move.to.x - move.from.x) <= 1 && abs(move.to.y - move.from.y) <= 1) {
            return SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
        } else {
            return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
        }
    }
    if (to_piece_color == current_player) {
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }

    int legal_capture_[100][3] = {0};
    int legal_capture_num = 0;
    for (unsigned int time = 1; time <= ((board_size_ / 2) - 1); time++) {
        struct line {
            unsigned int Line[100][3] = {0};
            int num = 0;
        } line[4];
        int p, q;
        p = time;
        for (q = 0; q < (int)board_size_; q++) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[0].Line[line[0].num][0] = 1;
                line[0].Line[line[0].num][1] = p;
                line[0].Line[line[0].num][2] = q;
                line[0].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[0].Line[line[0].num][0] = 2;
                line[0].Line[line[0].num][1] = p;
                line[0].Line[line[0].num][2] = q;
                line[0].num++;
            }
        }
        q = board_size_ - time - 1;
        for (p = 0; p < (int)board_size_; p++) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[1].Line[line[1].num][0] = 1;
                line[1].Line[line[1].num][1] = p;
                line[1].Line[line[1].num][2] = q;
                line[1].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[1].Line[line[1].num][0] = 2;
                line[1].Line[line[1].num][1] = p;
                line[1].Line[line[1].num][2] = q;
                line[1].num++;
            }
        }
        p = board_size_ - time - 1;
        for (q = board_size_ - 1; q >= 0; q--) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[2].Line[line[2].num][0] = 1;
                line[2].Line[line[2].num][1] = p;
                line[2].Line[line[2].num][2] = q;
                line[2].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[2].Line[line[2].num][0] = 2;
                line[2].Line[line[2].num][1] = p;
                line[2].Line[line[2].num][2] = q;
                line[2].num++;
            }
        }
        q = time;
        for (p = board_size_ - 1; p >= 0; p--) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[3].Line[line[3].num][0] = 1;
                line[3].Line[line[3].num][1] = p;
                line[3].Line[line[3].num][2] = q;
                line[3].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[3].Line[line[3].num][0] = 2;
                line[3].Line[line[3].num][1] = p;
                line[3].Line[line[3].num][2] = q;
                line[3].num++;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (line[i].num == 0) {
                continue;
            }
            int j = (i + 1) % 4;
            while (line[j].num == 0) {
                j = (j + 1) % 4;
            }
            if (line[i].Line[line[i].num - 1][0] != line[j].Line[0][0]) {
                legal_capture_[legal_capture_num][0] = line[i].Line[line[i].num - 1][0];
                legal_capture_[legal_capture_num][1] = line[i].Line[line[i].num - 1][1];
                legal_capture_[legal_capture_num][2] = line[i].Line[line[i].num - 1][2];
                legal_capture_[legal_capture_num + 1][0] = line[j].Line[0][0];
                legal_capture_[legal_capture_num + 1][1] = line[j].Line[0][1];
                legal_capture_[legal_capture_num + 1][2] = line[j].Line[0][2];
                legal_capture_num += 2;
                legal_capture_[legal_capture_num][0] = line[j].Line[0][0];
                legal_capture_[legal_capture_num][1] = line[j].Line[0][1];
                legal_capture_[legal_capture_num][2] = line[j].Line[0][2];
                legal_capture_[legal_capture_num + 1][0] = line[i].Line[line[i].num - 1][0];
                legal_capture_[legal_capture_num + 1][1] = line[i].Line[line[i].num - 1][1];
                legal_capture_[legal_capture_num + 1][2] = line[i].Line[line[i].num - 1][2];
                legal_capture_num += 2;

            } else if (line[i].Line[line[i].num - 1][1] == line[j].Line[0][1] && line[i].Line[line[i].num - 1][2] == line[j].Line[0][2]) {
                if (line[j].num >= 2) {
                    if (line[i].Line[line[i].num - 1][0] != line[j].Line[1][0]) {
                        legal_capture_[legal_capture_num][0] = line[i].Line[line[i].num - 1][0];
                        legal_capture_[legal_capture_num][1] = line[i].Line[line[i].num - 1][1];
                        legal_capture_[legal_capture_num][2] = line[i].Line[line[i].num - 1][2];
                        legal_capture_[legal_capture_num + 1][0] = line[j].Line[1][0];
                        legal_capture_[legal_capture_num + 1][1] = line[j].Line[1][1];
                        legal_capture_[legal_capture_num + 1][2] = line[j].Line[1][2];
                        legal_capture_num += 2;
                    }
                } else if (line[i].num >= 2) {
                    if (line[i].Line[line[i].num - 2][0] != line[j].Line[0][0]) {
                        legal_capture_[legal_capture_num][0] = line[j].Line[0][0];
                        legal_capture_[legal_capture_num][1] = line[j].Line[0][1];
                        legal_capture_[legal_capture_num][2] = line[j].Line[0][2];
                        legal_capture_[legal_capture_num + 1][0] = line[i].Line[line[i].num - 2][0];
                        legal_capture_[legal_capture_num + 1][1] = line[i].Line[line[i].num - 2][1];
                        legal_capture_[legal_capture_num + 1][2] = line[i].Line[line[i].num - 2][2];
                        legal_capture_num += 2;
                    }
                }
            }
        }
    }
    for (unsigned int time = 1; time <= ((board_size_ / 2) - 1); time++) {
        struct line {
            unsigned int Line[100][3] = {0};
            int num = 0;
        } line[4];
        int p, q;
        q = time;
        for (p = 0; p < (int)board_size_; p++) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[0].Line[line[0].num][0] = 1;
                line[0].Line[line[0].num][1] = p;
                line[0].Line[line[0].num][2] = q;
                line[0].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[0].Line[line[0].num][0] = 2;
                line[0].Line[line[0].num][1] = p;
                line[0].Line[line[0].num][2] = q;
                line[0].num++;
            }
        }
        p = board_size_ - time - 1;
        for (q = 0; q < (int)board_size_; q++) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[1].Line[line[1].num][0] = 1;
                line[1].Line[line[1].num][1] = p;
                line[1].Line[line[1].num][2] = q;
                line[1].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[1].Line[line[1].num][0] = 2;
                line[1].Line[line[1].num][1] = p;
                line[1].Line[line[1].num][2] = q;
                line[1].num++;
            }
        }
        q = board_size_ - time - 1;
        for (p = board_size_ - 1; p >= 0; p--) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[2].Line[line[2].num][0] = 1;
                line[2].Line[line[2].num][1] = p;
                line[2].Line[line[2].num][2] = q;
                line[2].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[2].Line[line[2].num][0] = 2;
                line[2].Line[line[2].num][1] = p;
                line[2].Line[line[2].num][2] = q;
                line[2].num++;
            }
        }
        p = time;
        for (q = board_size_ - 1; q >= 0; q--) {
            PieceColor _color = (*board_)[p][q]->GetColor();
            if (_color == PieceColor::WHITE) {
                line[3].Line[line[3].num][0] = 1;
                line[3].Line[line[3].num][1] = p;
                line[3].Line[line[3].num][2] = q;
                line[3].num++;
            }
            if (_color == PieceColor::BLACK) {
                line[3].Line[line[3].num][0] = 2;
                line[3].Line[line[3].num][1] = p;
                line[3].Line[line[3].num][2] = q;
                line[3].num++;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (line[i].num == 0) {
                continue;
            }
            int j = (i + 1) % 4;
            while (line[j].num == 0) {
                j = (j + 1) % 4;
            }
            if (line[i].Line[line[i].num - 1][0] != line[j].Line[0][0]) {
                legal_capture_[legal_capture_num][0] = line[i].Line[line[i].num - 1][0];
                legal_capture_[legal_capture_num][1] = line[i].Line[line[i].num - 1][1];
                legal_capture_[legal_capture_num][2] = line[i].Line[line[i].num - 1][2];
                legal_capture_[legal_capture_num + 1][0] = line[j].Line[0][0];
                legal_capture_[legal_capture_num + 1][1] = line[j].Line[0][1];
                legal_capture_[legal_capture_num + 1][2] = line[j].Line[0][2];
                legal_capture_num += 2;
                legal_capture_[legal_capture_num][0] = line[j].Line[0][0];
                legal_capture_[legal_capture_num][1] = line[j].Line[0][1];
                legal_capture_[legal_capture_num][2] = line[j].Line[0][2];
                legal_capture_[legal_capture_num + 1][0] = line[i].Line[line[i].num - 1][0];
                legal_capture_[legal_capture_num + 1][1] = line[i].Line[line[i].num - 1][1];
                legal_capture_[legal_capture_num + 1][2] = line[i].Line[line[i].num - 1][2];
                legal_capture_num += 2;

            } else if (line[i].Line[line[i].num - 1][1] == line[j].Line[0][1] && line[i].Line[line[i].num - 1][2] == line[j].Line[0][2]) {
                if (line[j].num >= 2) {
                    if (line[i].Line[line[i].num - 1][0] != line[j].Line[1][0]) {
                        legal_capture_[legal_capture_num][0] = line[i].Line[line[i].num - 1][0];
                        legal_capture_[legal_capture_num][1] = line[i].Line[line[i].num - 1][1];
                        legal_capture_[legal_capture_num][2] = line[i].Line[line[i].num - 1][2];
                        legal_capture_[legal_capture_num + 1][0] = line[j].Line[1][0];
                        legal_capture_[legal_capture_num + 1][1] = line[j].Line[1][1];
                        legal_capture_[legal_capture_num + 1][2] = line[j].Line[1][2];
                        legal_capture_num += 2;
                    }
                } else if (line[i].num >= 2) {
                    if (line[i].Line[line[i].num - 2][0] != line[j].Line[0][0]) {
                        legal_capture_[legal_capture_num][0] = line[j].Line[0][0];
                        legal_capture_[legal_capture_num][1] = line[j].Line[0][1];
                        legal_capture_[legal_capture_num][2] = line[j].Line[0][2];
                        legal_capture_[legal_capture_num + 1][0] = line[i].Line[line[i].num - 2][0];
                        legal_capture_[legal_capture_num + 1][1] = line[i].Line[line[i].num - 2][1];
                        legal_capture_[legal_capture_num + 1][2] = line[i].Line[line[i].num - 2][2];
                        legal_capture_num += 2;
                    }
                }
            }
        }
    }
    if (to_piece_color != PieceColor::NONE && legal_capture_num == 0) {
        return SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
    }

    for (int i = 0; i < legal_capture_num;) {
        if (legal_capture_[i][1] == (int)move.from.x && legal_capture_[i][2] == (int)move.from.y && legal_capture_[i + 1][1] == (int)move.to.x && legal_capture_[i + 1][2] == (int)move.to.y) {
            return SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
        } else {
            i += 2;
        }
    }
    return SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;

    return SurakartaIllegalMoveReason::LEGAL;
}

std::pair<SurakartaEndReason, SurakartaPlayer> SurakartaRuleManager::JudgeEnd(const SurakartaIllegalMoveReason reason) {
    // TODO: Implement this function.
    // Note that at this point, the board and game_info have not been updated yet.
    if (!IsLegalMoveReason(reason)) {
        SurakartaPlayer winner = ReverseColor(game_info_->current_player_);
        return std::make_pair(SurakartaEndReason::ILLIGAL_MOVE, winner);
    }

    int black_count = 0, white_count = 0;
    for (unsigned int i = 0; i < board_size_; i++) {
        for (unsigned int j = 0; j < board_size_; j++) {
            PieceColor color = (*board_)[i][j]->GetColor();
            if (color == PieceColor::BLACK) {
                black_count++;
                // i,j is the position of a black piece
            } else if (color == PieceColor::WHITE) {
                white_count++;
                // i,j is the position of a white piece
            } else {
                // i,j is an empty position
            }
        }
    }

    if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE && game_info_->current_player_ == SurakartaPlayer::BLACK && white_count == 1) {
        return std::make_pair(SurakartaEndReason::CHECKMATE, SurakartaPlayer::BLACK);
    }
    if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE && game_info_->current_player_ == SurakartaPlayer::WHITE && black_count == 1) {
        return std::make_pair(SurakartaEndReason::CHECKMATE, SurakartaPlayer::WHITE);
    }
    // 获取当前回合数
    // int current_round = game_info_->num_round_;

    // 检查是否达到最大回合数限制
    if (game_info_->num_round_ - game_info_->last_captured_round_ >=
            game_info_->max_no_capture_round_ &&
        reason != SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        if (white_count == black_count) {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::NONE);
        }
        if (white_count > black_count) {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::WHITE);
        }
        if (white_count < black_count) {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::BLACK);
        }
    }
    return std::make_pair(SurakartaEndReason::NONE, SurakartaPlayer::NONE);
}

std::unique_ptr<std::vector<SurakartaPosition>> SurakartaRuleManager::GetAllLegalTarget(const SurakartaPosition postion) {
    // TODO:
    // We don't test this function, you don't need to implement this function if you don't need it.
    return std::make_unique<std::vector<SurakartaPosition>>();
}

void SurakartaRuleManager::HelloWorld() {
    std::cout << "Hello World!" << std::endl;
}