/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef THEME_H
#define THEME_H

#include <QApplication>
#include <QColor>
#include <QGuiApplication>
#include <QPalette>
#include <QStyle>
#include <QStyleHints>

namespace Theme {

enum class Mode { System, Light, Dark };

// Maps the Settings theme string ("system"/"light"/"dark") to a Mode.
inline Mode modeFromSetting(const QString &key)
{
    if (key == QLatin1String("light")) return Mode::Light;
    if (key == QLatin1String("dark"))  return Mode::Dark;
    return Mode::System;
}

inline QPalette darkPalette()
{
    QPalette p;
    p.setColor(QPalette::Window,          QColor(0x2b, 0x2b, 0x2b));
    p.setColor(QPalette::WindowText,      Qt::white);
    p.setColor(QPalette::Base,            QColor(0x1e, 0x1e, 0x1e));
    p.setColor(QPalette::AlternateBase,   QColor(0x35, 0x35, 0x35));
    p.setColor(QPalette::ToolTipBase,     QColor(0x2b, 0x2b, 0x2b));
    p.setColor(QPalette::ToolTipText,     Qt::white);
    p.setColor(QPalette::Text,            Qt::white);
    p.setColor(QPalette::Button,          QColor(0x3c, 0x3c, 0x3c));
    p.setColor(QPalette::ButtonText,      Qt::white);
    p.setColor(QPalette::BrightText,      Qt::red);
    p.setColor(QPalette::Link,            QColor(0x42, 0x85, 0xf4));
    p.setColor(QPalette::Highlight,       QColor(0x42, 0x85, 0xf4));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::Disabled, QPalette::Text,       QColor(0x60, 0x60, 0x60));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x60, 0x60, 0x60));
    return p;
}

inline QPalette lightPalette()
{
    QPalette p;
    p.setColor(QPalette::Window,          QColor(0xf0, 0xf0, 0xf0));
    p.setColor(QPalette::WindowText,      QColor(0x20, 0x20, 0x20));
    p.setColor(QPalette::Base,            Qt::white);
    p.setColor(QPalette::AlternateBase,   QColor(0xe9, 0xe9, 0xe9));
    p.setColor(QPalette::ToolTipBase,     QColor(0xff, 0xff, 0xdc));
    p.setColor(QPalette::ToolTipText,     Qt::black);
    p.setColor(QPalette::Text,            QColor(0x20, 0x20, 0x20));
    p.setColor(QPalette::Button,          QColor(0xe0, 0xe0, 0xe0));
    p.setColor(QPalette::ButtonText,      QColor(0x20, 0x20, 0x20));
    p.setColor(QPalette::BrightText,      Qt::red);
    p.setColor(QPalette::Link,            QColor(0x18, 0x62, 0xd1));
    p.setColor(QPalette::Highlight,       QColor(0x42, 0x85, 0xf4));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::Disabled, QPalette::Text,       QColor(0xa0, 0xa0, 0xa0));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0xa0, 0xa0, 0xa0));
    return p;
}

// True when the UI resolves to a dark appearance for the given mode.
inline bool isDark(Mode mode)
{
    if (mode == Mode::Dark)  return true;
    if (mode == Mode::Light) return false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    return qApp->palette().color(QPalette::Window).lightness() < 128;
#endif
}

// Safe to call at startup or at runtime; Qt broadcasts a palette change to all widgets.
inline void apply(Mode mode)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto *hints = QGuiApplication::styleHints();
    switch (mode) {
        case Mode::Light:  hints->setColorScheme(Qt::ColorScheme::Light); break;
        case Mode::Dark:   hints->setColorScheme(Qt::ColorScheme::Dark);  break;
        case Mode::System: hints->unsetColorScheme();                    break;
    }
#else
    // No native colour-scheme override before Qt 6.8; force Fusion + a manual palette.
    QApplication::setStyle(QStringLiteral("Fusion"));
    if (mode == Mode::System)
        QApplication::setPalette(QApplication::style()->standardPalette());
    else
        QApplication::setPalette(mode == Mode::Dark ? darkPalette() : lightPalette());
#endif
}

} // namespace Theme

#endif // THEME_H
