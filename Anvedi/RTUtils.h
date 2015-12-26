#pragma once
#include <vector>
#include <utility>
#include <QString>

class SignalData;

using DataToReplay = std::vector<std::pair<QString, QVector<qreal>>>;

DataToReplay PrepareReplay(const QString& file, SignalData& data);