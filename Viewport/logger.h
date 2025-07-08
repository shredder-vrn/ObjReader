#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

inline void LogDebug(const QString& msg) {
    qDebug() << "[DEBUG]" << msg;
}

inline void LogInfo(const QString& msg) {
    qInfo() << "[INFO]" << msg;
}

inline void LogWarning(const QString& msg) {
    qWarning() << "[WARNING]" << msg;
}

inline void LogError(const QString& msg) {
    qCritical() << "[ERROR]" << msg;
}
#endif // LOGGER_H
