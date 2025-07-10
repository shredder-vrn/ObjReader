#ifndef VIEWER_LOGGER_H
#define VIEWER_LOGGER_H

#include <QDebug>

namespace Viewer {

//! REIVIEW: нейминг не по кодстайлу + не используется кажэется
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

}
#endif // VIEWER_LOGGER_H
